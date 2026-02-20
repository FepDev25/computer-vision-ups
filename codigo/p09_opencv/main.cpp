
#include <iostream> // Librería para entrada y salida de datos
#include <cstdlib> // Manejo de funciones de conversión y gestión de memoria
#include <cstring> // Librería para manejo de cadenas de texto

#include <sstream> // Librería para manejo de flujos de texto y conversión
#include <fstream> // Librería para manejo de flujos y archivos

#include <cmath> // Librería para operaciones matemáticas

#include <vector> // Arreglos dinámicos
#include <filesystem> // Listar directorios


#include <opencv2/imgproc/imgproc.hpp> // Procesamiento de imágenes
#include <opencv2/imgcodecs/imgcodecs.hpp> // Permite la apertura de archivos en diferente formato
#include <opencv2/highgui/highgui.hpp> // Creación de GUI
#include <opencv2/video/video.hpp> // Lectura de vídeo
#include <opencv2/videoio/videoio.hpp> // Lectura y escritura de video
#include <opencv2/core/core.hpp> // Definiciones base (matrices e imágenes)


using namespace std;
using namespace filesystem;
using namespace cv;

int mT = 0;
int kSize = 1;


void eventoTrack(int v, void *data){
    mT = getTrackbarPos("T-M", "Video");
    kSize = getTrackbarPos("Kernel-Size", "Video") * 2 + 1;
}

Mat sigmoideo(Mat imagen, double k = 1.3){
    int pixel = 0;
    int opera = 0;
    Mat binaria = Mat::zeros(Size(imagen.cols, imagen.rows), CV_8UC1);

    for(int i=0;i<imagen.rows;i++){
        for(int j=0;j<imagen.cols;j++){
            pixel = imagen.at<uchar>(i,j);

            opera = 255.0/(1.0+exp(-k*(pixel-((double)mT))));
            
            binaria.at<uchar>(i,j) = opera;
        }
    }

    // s = 1/(1+exp(-k*(x-m)))
    return binaria;

}


int main(int argc, char *argv[]){


    VideoCapture video(0);

    if (video.isOpened()){
        namedWindow("Video", WINDOW_AUTOSIZE);
        createTrackbar("T-M", "Video", nullptr, 255, eventoTrack);
        createTrackbar("Kernel-Size", "Video", nullptr, 255, eventoTrack);

        

        namedWindow("BinariaT", WINDOW_AUTOSIZE);
        namedWindow("BinariaCS", WINDOW_AUTOSIZE);
        namedWindow("Mediana", WINDOW_AUTOSIZE);

        Mat frame;
        Mat binariaT;
        Mat binariaCS;

        Mat mediana;
        
        while(3==3){
            video >> frame;
            cvtColor(frame, frame, COLOR_BGR2GRAY);

            threshold(frame, binariaT, mT, 255, THRESH_BINARY_INV);
            binariaCS = sigmoideo(frame);

            medianBlur(frame, mediana, kSize);

            imshow("Video", frame);
            imshow("BinariaT", binariaT);
            imshow("BinariaCS", binariaCS);
            imshow("Mediana", mediana);

            if(waitKey(23)==27)
                break;

        }
        video.release();
        destroyAllWindows();

    }



    return 0;
}