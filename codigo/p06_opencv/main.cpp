#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

Mat convertirGris(Mat img){
    Mat gris = Mat::zeros(img.size(), CV_8UC1);

    Vec3b pixel;
    int pixelg;

    for (int i = 0; i < img.rows; i++){
        for (int j = 0; j < img.cols; j++){
            pixel = img.at<Vec3b>(i, j);
            pixelg = (pixel[0] + pixel[1] + pixel[2]) / 3; // BGR a Gris simple
            gris.at<uchar>(i, j) = pixelg;
        }
    }

    return gris;

}

int main() {

    VideoCapture video(0);
    if (!video.isOpened()) {
        cout << "Error: No se pudo abrir la cámara" << endl;
        return -1;
    }

    namedWindow("Cámara Web", WINDOW_AUTOSIZE);
    namedWindow("Cámara Web - Gris", WINDOW_AUTOSIZE);
    namedWindow("Cámara Web - Gris (Color)", WINDOW_AUTOSIZE);
    namedWindow("Cámara Web - Diferencia", WINDOW_AUTOSIZE);

    Mat frame;
    Mat gris;
    Mat grisO;

    Mat anterior;
    Mat resta;

    while (true){
        video >> frame;
        flip(frame, frame, 1); // Espejar la imagen horizontalmente

        gris = convertirGris(frame);
        cvtColor(gris, grisO, COLOR_GRAY2BGR); // Convertir gris a BGR para mostrar ambos en color

        
        if (anterior.empty()){
            anterior = gris.clone();
        }

        absdiff(gris, anterior, resta);
        anterior = gris.clone();

        imshow("Cámara Web", frame);
        imshow("Cámara Web - Gris", gris);
        imshow("Cámara Web - Gris (Color)", grisO);
        imshow("Cámara Web - Diferencia", resta);

        if (waitKey(23) == 27){
            break;
        }
    }

    video.release();
    destroyAllWindows();
    
    return 0;
}

// Android studio y opencv sdk
