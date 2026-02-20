#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    Mat imagen2 = imread("im2.jpg", IMREAD_GRAYSCALE);
    resize(imagen2, imagen2, Size(), 0.5, 0.5);
    if (imagen2.empty()) {
        cerr << "Error al abrir la imagen" << endl;
        return -1;
    }

    int *histo = new int[256]();
    ofstream file("histograma.csv");

    for (int i = 0; i < imagen2.rows; i++){
        for (int j = 0; j < imagen2.cols; j++){
            int valor = imagen2.at<uchar>(i, j);
            histo[valor]++;
        }
    }

    Mat lienzo = Mat::zeros(Size(1024, 760), CV_8UC3);
    int maximo = 0;
    
    for (int i = 0; i < 256; i++){
        file << i << " " << histo[i] << endl;
        if (histo[i] > maximo) {
            maximo = histo[i];
        }
    }

    double escala = ((double) 768 / (double) maximo);

    for (int i = 0; i < 256; i++) {
        int altura = static_cast<int>(histo[i] * escala);
        line(lienzo, Point(i * 4, 768), Point(i * 4, 768 - altura), Scalar(255, 255, 255), 2);
    }

    Mat ecualizada;
    equalizeHist(imagen2, ecualizada);

    Mat imagenCLAHE;
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->apply(imagen2, imagenCLAHE);

    imshow("Imagen Original", imagen2);
    imshow("Histograma", lienzo);
    imshow("Imagen Ecualizada", ecualizada);
    imshow("Imagen CLAHE", imagenCLAHE);

    waitKey(0);

    file.close();
    delete[] histo;

    VideoCapture video(0);
    if (video.isOpened()){
        namedWindow("Video", WINDOW_NORMAL);
        Mat imagenLab;
        vector<Mat> canales;

        while (true){
            Mat frame;
            video >> frame;
            cvtColor(frame, imagenLab, COLOR_BGR2GRAY);
            split(imagenLab, canales);
            merge(canales, ecualizada);
            cvtColor(ecualizada, ecualizada, COLOR_Lab2BGR);

            imshow("Ecualizada", ecualizada);
            imshow("Video", frame);

            if (waitKey(30) >= 0) {
                break;
            }

        }
        
    }




    return 0;
}

// bitwise, espacios de color, lienzos
