#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <limits>
#include <opencv2/opencv.hpp>
#include <opencv2/core/utils/logger.hpp>

#define ENABLE_CUDA

#ifdef ENABLE_CUDA
#include <opencv2/cudaimgproc.hpp>
#include <opencv2/cudafilters.hpp>
#include <opencv2/cudaarithm.hpp>
#endif

using namespace std;
using namespace cv;

double getFPS(chrono::time_point<chrono::high_resolution_clock> start) {
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    return 1.0 / diff.count();
}

int main(int argc, char* argv[]) {
    cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_ERROR);

    if (argc < 2) {
        cerr << "Uso: " << argv[0] << " <ruta_video> [salida.avi] [--cpu|--gpu]" << endl;
        return -1;
    }

    bool save_output = false;
    string output_path;
    bool force_cpu = false;
    bool force_gpu = false;

    for (int i = 2; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--cpu") {
            force_cpu = true;
        } else if (arg == "--gpu") {
            force_gpu = true;
        } else if (!save_output) {
            save_output = true;
            output_path = arg;
        } else {
            cerr << "Argumento ignorado: " << arg << endl;
        }
    }

    VideoCapture cap(argv[1]);
    if (!cap.isOpened()) {
        cerr << "Error: No se puede abrir el video: " << argv[1] << endl;
        return -1;
    }

    double input_fps = cap.get(CAP_PROP_FPS);
    if (input_fps <= 1.0) input_fps = 30.0;
    cap.set(CAP_PROP_FRAME_WIDTH, 1920);
    cap.set(CAP_PROP_FRAME_HEIGHT, 1080);

    Mat frame, result_frame;
    Mat gray, blur, hist, eroded;
    VideoWriter writer;
    size_t frame_count = 0;
    double accum_ms = 0.0;
    double max_fps = 0.0;
    double min_fps = std::numeric_limits<double>::max();
    
    bool use_cuda = false;

#ifdef ENABLE_CUDA
    use_cuda = !force_cpu;
    if (force_gpu) use_cuda = true;
#else
    use_cuda = false;
    if (force_gpu) {
        cerr << "CUDA no disponible en la compilacion; se usara CPU." << endl;
    }
#endif

#ifdef ENABLE_CUDA
    cuda::GpuMat d_frame, d_gray, d_blur, d_hist, d_eroded, d_edges;
    Ptr<cuda::Filter> gaussFilter;
    Ptr<cuda::Filter> erodeFilter;
    Ptr<cuda::CannyEdgeDetector> cannyFilter;
#endif

    if (use_cuda) {
#ifdef ENABLE_CUDA
        cout << ">>> MODO: GPU (CUDA) ACTIVADO <<<" << endl;
        gaussFilter = cuda::createGaussianFilter(CV_8UC1, CV_8UC1, Size(5, 5), 1.5);
        Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
        erodeFilter = cuda::createMorphologyFilter(MORPH_ERODE, CV_8UC1, element);
        cannyFilter = cuda::createCannyEdgeDetector(50, 150);
#endif
    } else {
        cout << ">>> modo CPU <<<" << endl;
    }

    while (true) {
        auto start_time = chrono::high_resolution_clock::now();
        
        cap >> frame;
        if (frame.empty()) break;

    if (use_cuda) {
#ifdef ENABLE_CUDA
        // pipeline en GPU
        d_frame.upload(frame);
        cuda::cvtColor(d_frame, d_gray, COLOR_BGR2GRAY);
        gaussFilter->apply(d_gray, d_blur);
        cuda::equalizeHist(d_blur, d_hist);
        erodeFilter->apply(d_hist, d_eroded);
        cannyFilter->detect(d_eroded, d_edges);
        d_edges.download(result_frame);
#endif
    } else {
        // pipeline en CPU
        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blur, Size(5, 5), 1.5);
        equalizeHist(blur, hist);
        erode(hist, eroded, getStructuringElement(MORPH_RECT, Size(3, 3)));
        Canny(eroded, result_frame, 50, 150);
    }
        double fps = getFPS(start_time);
        double elapsed_ms = 1000.0 / fps;

        frame_count++;
        accum_ms += elapsed_ms;
        max_fps = std::max(max_fps, fps);
        min_fps = std::min(min_fps, fps);

        std::ostringstream ms_label;
        ms_label << std::fixed << std::setprecision(2) << elapsed_ms;

        string device_tag = use_cuda ? "GPU (CUDA)" : "CPU";
        putText(result_frame, "Dispositivo: " + device_tag, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255), 2);
        putText(result_frame, "FPS: " + to_string((int)fps), Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255), 2);
        putText(result_frame, "Tiempo: " + ms_label.str() + " ms", Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255), 2);

        if (save_output) {
            if (!writer.isOpened()) {
                int fourcc = VideoWriter::fourcc('M', 'J', 'P', 'G');
                bool ok = writer.open(output_path, fourcc, input_fps, result_frame.size(), false);
                if (!ok) {
                    cerr << "No se pudo abrir el archivo de salida: " << output_path << endl;
                }
            }
            if (writer.isOpened()) {
                writer.write(result_frame);
            }
        }

        imshow("Laboratorio Vision - Pipeline", result_frame);

        if (waitKey(1) == 27) break; // ESC para salir
    }

    if (frame_count > 0) {
        double avg_ms = accum_ms / static_cast<double>(frame_count);
        double avg_fps = 1000.0 / avg_ms;
        cout << "Resumen de ejecucion" << endl;
        cout << "  Frames procesados: " << frame_count << endl;
        cout << "  Promedio: " << fixed << setprecision(2) << avg_ms << " ms/frame (" << setprecision(1) << avg_fps << " fps)" << endl;
        cout << "  Mejor fps: " << fixed << setprecision(1) << max_fps << " | Peor fps: " << min_fps << endl;
    }

    return 0;
}