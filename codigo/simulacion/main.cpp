#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

int main() {
    // 1. Cargar la imagen
    string imagePath = "Spiral_San-Basilio.png";
    Mat img = imread(imagePath);
    if (img.empty()) {
        cout << "Error al cargar la imagen: " << imagePath << endl;
        return -1;
    }

    // 2. Definir parámetros de la espiral
    // Centro de la imagen
    int cx = img.cols / 2;
    int cy = img.rows / 2;

    // Parámetros de la espiral de Arquímedes: r = a + b * theta
    // Estimación visual: la espiral cubre la imagen.
    // Asumimos que llega hasta el borde más cercano.
    double radio_max = min(img.rows, img.cols) / 2.0;
    
    // Número de giros estimados visualmente en la imagen (a)
    // Contando las vueltas de color: rojo, amarillo, verde, cyan, azul, magenta... parece haber unas 5-6 vueltas.
    double giros = 6.0; 
    double theta_max = giros * 2 * CV_PI;
    
    // b determina la separación entre los brazos de la espiral
    double b = radio_max / theta_max;
    double a = 0.0; // Empieza en el centro

    vector<Vec3b> spiralPixels;

    // 3. Recorrer la espiral y extraer píxeles
    // Usamos coordenadas polares y las convertimos a cartesianas.
    // Para asegurar que no saltamos píxeles (continuidad), ajustamos el paso d_theta.
    // Longitud de arco ds approx r * d_theta. Queremos ds approx 1 pixel.
    
    double theta = 0.0;
    
    // Límite de seguridad para el bucle
    while (theta < theta_max * 1.5) { // Un poco más por si acaso
        double r = a + b * theta;
        
        // Convertir polar a cartesiano
        int x = cx + static_cast<int>(r * cos(theta));
        int y = cy + static_cast<int>(r * sin(theta));

        // Verificar límites de la imagen
        if (x < 0 || x >= img.cols || y < 0 || y >= img.rows) {
            // Si la espiral sale de la imagen, paramos
            break;
        }

        // Extraer el valor del pixel en (x, y)
        spiralPixels.push_back(img.at<Vec3b>(y, x));

        // Calcular el siguiente paso de theta
        // d_theta = ds / r. Si r es pequeño, d_theta es grande.
        // Limitamos d_theta para no avanzar demasiado rápido cerca del centro.
        double d_theta;
        if (r < 1.0) {
            d_theta = 0.1; // Paso fijo pequeño cerca del origen
        } else {
            d_theta = 1.0 / r; // Paso adaptativo para mantener arco ~1px
        }
        theta += d_theta;
    }

    // 4. Formar la imagen cuadrada (b)
    // Calculamos el tamaño del lado del cuadrado necesario
    int N = spiralPixels.size();
    int side = static_cast<int>(ceil(sqrt(N)));
    
    Mat result = Mat::zeros(side, side, img.type());
    
    // Rellenar la imagen resultante fila a fila
    int idx = 0;
    for (int i = 0; i < result.rows; ++i) {
        for (int j = 0; j < result.cols; ++j) {
            if (idx < N) {
                result.at<Vec3b>(i, j) = spiralPixels[idx];
                idx++;
            }
        }
    }

    // 5. Guardar resultados
    imwrite("resultado_espiral.png", result);
    
    cout << "Proceso finalizado." << endl;
    cout << "Dimensiones imagen original: " << img.cols << "x" << img.rows << endl;
    cout << "Píxeles extraídos: " << N << endl;
    cout << "Imagen resultante generada: resultado_espiral.png (" << side << "x" << side << ")" << endl;

    return 0;
}