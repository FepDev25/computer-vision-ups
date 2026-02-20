#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Uso: ./programa <nombre_imagen>" << endl;
        return -1;
    }

    Mat image = imread(argv[1]);
    if (image.empty()) {
        cout << "No se pudo cargar la imagen." << endl;
        return -1;
    }

    // Parametros
    int cx = 541;
    int cy = 360;

    double a = 7.0;         
    double fase = 0.0;     
    double sentido = 1.0;

    int end_x = 803;
    int end_y = 350;

    // calcular la distancia desde el centro hasta ese punto final, este será el radio maximo y si se pasa, se para
    double max_radius = sqrt(pow(end_x - cx, 2) + pow(end_y - cy, 2));

    cout << "Parametros cargados." << endl;
    cout << "Radio limite calculado: " << max_radius << " pixeles." << endl;

    // recorrido y extracción
    vector<Vec3b> pixelesExtraidos;
    double theta = 0.0;
    bool recorriendo = true;
    int prev_x = -1, prev_y = -1;

    cout << "Iniciando recorrido..." << endl;

    while (recorriendo) {
        double r = a * theta;

        // si el radio actual supera la distancia al punto final, se termina.
        if (r >= max_radius) {
            recorriendo = false;
            break;
        }

        // conversión Polar -> Cartesiana
        double angulo = (theta * sentido) + fase;
        int x = (int)(cx + r * cos(angulo));
        int y = (int)(cy + r * sin(angulo));

        // verificación de límites de la imagen, si se sale de la imagen se sale
        if (x >= 0 && x < image.cols && y >= 0 && y < image.rows) {
            // evitar duplicados
            if (x != prev_x || y != prev_y) {
                pixelesExtraidos.push_back(image.at<Vec3b>(y, x));
                prev_x = x;
                prev_y = y;
            }
        } else {
             recorriendo = false;
             break;
        }

        // cálculo del paso dinámico
        double delta = (r < 1.0) ? 0.1 : (1.0 / r);
        theta += delta;
    }

    // reconstrucción de la imagen final
    if (pixelesExtraidos.empty()) return -1;

    int totalPixels = pixelesExtraidos.size();
    int lado = (int)ceil(sqrt((double)totalPixels));
    Mat resultado = Mat::zeros(lado, lado, image.type());

    for (int i = 0; i < totalPixels; i++) {
        resultado.at<Vec3b>(i / lado, i % lado) = pixelesExtraidos[i];
    }

    imshow("Imagen Original", image);
    imshow("Resultado Final (Extendida)", resultado);
    
    imwrite("resultado_final_extendido.jpg", resultado);
    waitKey(0);

    return 0;
}