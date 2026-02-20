#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// Variables globales para el trackbar
int ksize = 3;
const int MAX_KSIZE = 31;
int cannyThreshold1 = 50;
int cannyThreshold2 = 150;
const int MAX_THRESHOLD = 300;

int main() {

    VideoCapture video(0); // 0 para la cámara predeterminada
    
    if(!video.isOpened()) {
        cout << "Error al abrir la cámara" << endl;
        return -1;
    }

    Mat frame;
    Mat gris;
    Mat Gx, Gy;
    Mat absGx, absGy, gradiente;
    Mat laplacian, absLaplacian;
    Mat canny;

    namedWindow("Imagen Original", WINDOW_AUTOSIZE);
    namedWindow("Gradiente X (Gx)", WINDOW_AUTOSIZE);
    namedWindow("Gradiente Y (Gy)", WINDOW_AUTOSIZE);
    namedWindow("Gradiente Sobel", WINDOW_AUTOSIZE);
    namedWindow("Laplaciano", WINDOW_AUTOSIZE);
    namedWindow("Canny", WINDOW_AUTOSIZE);

    createTrackbar("Kernel Size", "Imagen Original", &ksize, MAX_KSIZE);
    createTrackbar("Canny Threshold 1", "Canny", &cannyThreshold1, MAX_THRESHOLD);
    createTrackbar("Canny Threshold 2", "Canny", &cannyThreshold2, MAX_THRESHOLD);

    while(true){
        video >> frame;
        if(frame.empty()) break;

        cvtColor(frame, gris, COLOR_BGR2GRAY);

        // Asegurar que ksize sea impar y al menos 1
        int currentKsize = ksize;
        if(currentKsize < 1) currentKsize = 1;
        if(currentKsize % 2 == 0) currentKsize++;
        if(currentKsize > MAX_KSIZE) currentKsize = MAX_KSIZE;

        // Aplicar el filtro Sobel en la dirección X
        Sobel(gris, Gx, CV_16S, 1, 0, currentKsize);
        convertScaleAbs(Gx, absGx);

        // Aplicar el filtro Sobel en la dirección Y
        Sobel(gris, Gy, CV_16S, 0, 1, currentKsize);
        convertScaleAbs(Gy, absGy);

        // Combinar las dos direcciones
        addWeighted(absGx, 0.5, absGy, 0.5, 0, gradiente);

        // Aplicar el filtro Laplaciano
        Laplacian(gris, laplacian, CV_16S, currentKsize);
        convertScaleAbs(laplacian, absLaplacian);

        // Aplicar el filtro Canny
        Canny(gris, canny, cannyThreshold1, cannyThreshold2);

        imshow("Imagen Original", frame);
        imshow("Gradiente X (Gx)", absGx);
        imshow("Gradiente Y (Gy)", absGy);
        imshow("Gradiente Sobel", gradiente);
        imshow("Laplaciano", absLaplacian);
        imshow("Canny", canny);

        if(waitKey(30) >= 0) break;
    }

    video.release();
    destroyAllWindows();

    return 0;
}