#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {

    Mat image = Mat::zeros(7, 7, CV_8UC1);

    Mat imagenEscalada;
    Mat imagenEscaladaRotada;

    // A en imagen 7x7
    for (int i = 2; i < 7; i++) {
        image.at<uchar>(i, 1) = 255;
        image.at<uchar>(i, 5) = 255;
    }

    image.at<uchar>(1, 2) = 255;
    image.at<uchar>(4, 2) = 255;
    image.at<uchar>(1, 4) = 255;
    image.at<uchar>(4, 4) = 255;
    image.at<uchar>(0, 3) = 255;
    image.at<uchar>(4, 3) = 255;

    // Escalar la A dibujada a 77x77 con vecino más cercano
    resize(image, imagenEscalada, Size(), 77, 77, INTER_NEAREST);

    // Rotar la imagen escalada (p. ej. 45 grados) alrededor de su centro
    Point2f centro(imagenEscalada.cols / 2.0F, imagenEscalada.rows / 2.0F);
    Mat rot = getRotationMatrix2D(centro, 45.0, 1.0);
    warpAffine(imagenEscalada, imagenEscaladaRotada, rot, imagenEscalada.size(), INTER_NEAREST, BORDER_CONSTANT, Scalar(0));
    
    namedWindow("Letra A", WINDOW_AUTOSIZE);
    imshow("Letra A", image);

    namedWindow("Letra A Escalada", WINDOW_AUTOSIZE);
    imshow("Letra A Escalada", imagenEscalada);

    namedWindow("Letra A Rotada", WINDOW_AUTOSIZE);
    imshow("Letra A Rotada", imagenEscaladaRotada);
    
    // Normalizar cada imagen a valores 0-1 para contar píxeles
    Mat image_norm, imagenEscalada_norm, imagenEscaladaRotada_norm;
    image.convertTo(image_norm, CV_32F, 1.0/255.0);
    imagenEscalada.convertTo(imagenEscalada_norm, CV_32F, 1.0/255.0);
    imagenEscaladaRotada.convertTo(imagenEscaladaRotada_norm, CV_32F, 1.0/255.0);

    // Calcular momentos de cada imagen
    Moments m = moments(image_norm, false);
    Moments mEsc = moments(imagenEscalada_norm, false);
    Moments mRot = moments(imagenEscaladaRotada_norm, false);

    auto printMoments = [](const string& label, const Moments& mo) {
        double area = mo.m00;
        double cx = mo.m10 / area;
        double cy = mo.m01 / area;
        cout << label << " -> m00: " << area << ", m10: " << mo.m10
             << ", m01: " << mo.m01 << ", cx: " << cx << ", cy: " << cy << endl;
    };

    printMoments("Original", m);
    printMoments("Escalada", mEsc);
    printMoments("Escalada+Rotada", mRot);
    
    waitKey(0);



    return 0;
}
