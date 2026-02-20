#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// trackbar: ingresar valores para ir probando ese rato la imagen
int lmin = 0, aMin = 0, bMin = 0;
int lmax = 255, aMax = 255, bMax = 255;

void eventoTrackbar(int v, void* /*data*/){
    cout << "Valor trackbar: " << v << " L min: " << lmin << endl;
}

int main() {

    VideoCapture video(0);
    if (!video.isOpened()) {
        cout << "Error: No se pudo abrir la cámara" << endl;
        return -1;
    }

    namedWindow("Video", WINDOW_AUTOSIZE);
    namedWindow("HSV", WINDOW_AUTOSIZE);  // 0 a 180
    namedWindow("LAB", WINDOW_AUTOSIZE); // 0 a 255
    namedWindow("BINARIA", WINDOW_AUTOSIZE);
    namedWindow("MASCARA", WINDOW_AUTOSIZE);
    namedWindow("AND", WINDOW_AUTOSIZE);
    namedWindow("OR", WINDOW_AUTOSIZE);
    namedWindow("NEGATIVO", WINDOW_AUTOSIZE);
    namedWindow("Lienzo", WINDOW_AUTOSIZE);

    createTrackbar("L min", "Video", &lmin, 255, eventoTrackbar, NULL);
    createTrackbar("A min", "Video", &aMin, 255, eventoTrackbar, NULL);
    createTrackbar("B min", "Video", &bMin, 255, eventoTrackbar, NULL);

    createTrackbar("L max", "Video", &lmax, 255, eventoTrackbar, NULL);
    createTrackbar("A max", "Video", &aMax, 255, eventoTrackbar, NULL);
    createTrackbar("B max", "Video", &bMax, 255, eventoTrackbar, NULL);

    Mat frame;
    Mat hsv;
    Mat lab;
    Mat binaria;
    Mat imagenAnd;
    Mat imagenOr;
    Mat negativo;


    int frameHeight = video.get(CAP_PROP_FRAME_HEIGHT);
    int frameWidth = video.get(CAP_PROP_FRAME_WIDTH);

    cout << "Ancho: " << frameWidth << " Alto: " << frameHeight << endl;

    Mat mascaraM = Mat::zeros(Size(frameWidth, frameHeight), CV_8UC1);
    circle(mascaraM, Point(frameWidth / 2, frameHeight / 2), 73, Scalar(255), -1);

    Mat lienzo;
    Mat tmp;

    while (true){
        video >> frame;
        flip(frame, frame, 1);

        cvtColor(frame, hsv, COLOR_BGR2HSV);
        cvtColor(frame, lab, COLOR_BGR2Lab);

        inRange(lab, Scalar(lmin, aMin, bMin), Scalar(lmax, aMax, bMax), binaria);

        bitwise_and(frame, frame, imagenAnd, mascaraM);

        bitwise_or(frame, frame, imagenOr, mascaraM);

        bitwise_not(frame, negativo);

        bitwise_and(frame, frame, tmp, binaria);

        lienzo = Mat::zeros(Size(frameWidth * 2, frameHeight), CV_8UC3);
        tmp.copyTo(lienzo(Rect(frameWidth, 0, frameWidth, frameHeight)));

        cvtColor(binaria, tmp, COLOR_GRAY2BGR);
        tmp.copyTo(lienzo(Rect(0, 0, frameWidth, frameHeight)));

        imshow("Video", frame);
        imshow("HSV", hsv);
        imshow("LAB", lab); 
        imshow("BINARIA", binaria);
        imshow("MASCARA", mascaraM);
        imshow("AND", imagenAnd);
        imshow("OR", imagenOr);
        imshow("NEGATIVO", negativo);
        imshow("Lienzo", lienzo);

        if (waitKey(23) == 27){
            break;
        }
    }

    video.release();
    destroyAllWindows();
    
    return 0;
}

// bitwise, espacios de color, lienzos
