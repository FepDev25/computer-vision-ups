#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int valGMin = 180;
int valGRBMax = 100;
int valPurezaV = 50;

int valRBMin = 180;
int valMGMax = 100;
int valPurezaM = 50;

Mat imagen;

void onTrackbar(int, void*) {
    Mat mascara = Mat::zeros(imagen.size(), CV_8UC1);
    Mat visualizacion = imagen.clone();

    for (int y = 0; y < imagen.rows; y++) {
        for (int x = 0; x < imagen.cols; x++) {
            Vec3b pixel = imagen.at<Vec3b>(y, x);
            int b = pixel[0];
            int g = pixel[1];
            int r = pixel[2];

            bool esVerde = false;
            bool esMagenta = false;

            if (g > valGMin && r < valGRBMax && b < valGRBMax) {
                if ((g - r) > valPurezaV && (g - b) > valPurezaV) {
                    esVerde = true;
                }
            }

            if (r > valRBMin && b > valRBMin && g < valMGMax) {
                if ((r - g) > valPurezaM && (b - g) > valPurezaM) {
                    esMagenta = true;
                }
            }

            if (esVerde || esMagenta) {
                mascara.at<uchar>(y, x) = 255;
                visualizacion.at<Vec3b>(y, x) = Vec3b(0, 255, 0); 
            }
        }
    }

    imshow("Mascara", mascara);
    imshow("Visualizacion", visualizacion);
}

int main(int argc, char** argv) {
    string nombreImagen = (argc > 1) ? argv[1] : "Estudiante_19_VarD_EXAMEN.png";
    imagen = imread(nombreImagen);

    if (imagen.empty()) {
        return -1;
    }

    namedWindow("Controles", WINDOW_AUTOSIZE);
    
    createTrackbar("Verde Min (G)", "Controles", &valGMin, 255, onTrackbar);
    createTrackbar("Verde Ruido Max (RB)", "Controles", &valGRBMax, 255, onTrackbar);
    createTrackbar("Verde Pureza (Diff)", "Controles", &valPurezaV, 150, onTrackbar);
    
    createTrackbar("Mag Min (RB)", "Controles", &valRBMin, 255, onTrackbar);
    createTrackbar("Mag Ruido Max (G)", "Controles", &valMGMax, 255, onTrackbar);
    createTrackbar("Mag Pureza (Diff)", "Controles", &valPurezaM, 150, onTrackbar);

    onTrackbar(0, 0);

    waitKey(0);
    return 0;
}