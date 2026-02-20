#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    string nombreImagen = (argc > 1) ? argv[1] : "Estudiante_19_VarD_EXAMEN.png";
    Mat imagen = imread(nombreImagen);

    if (imagen.empty()) {
        cout << "Error al cargar la imagen" << endl;
        return -1;
    }

    Mat resultado = imagen.clone();
    int filas = imagen.rows;
    int columnas = imagen.cols;

    // calcular el ancho dinamico de la franja
    // obtener el valor de una fila segura para medir la franja.
    int filaRef = filas - 4; // se evita la última fila por si hay ruido
    int xInicioFranja = -1;
    int xFinFranja = -1;

    for (int x = 0; x < columnas; x++) {
        Vec3b pixel = imagen.at<Vec3b>(filaRef, x);
        int b = pixel[0];
        int g = pixel[1];
        int r = pixel[2];

        // detectar la franja basada en los colores y purezas obtendidas
        // Verde Pureza: 40 | Mag Pureza: 49
        bool esFranja = false;

        // detección Verde (g gana a r y b por 40)
        if (g > r + 40 && g > b + 40) esFranja = true;
        
        // detección Magenta (r y b ganan a g por 49)
        if (r > g + 49 && b > g + 49) esFranja = true;

        if (esFranja) {
            if (xInicioFranja == -1){
                xInicioFranja = x;
            };

            xFinFranja = x;
        }
    }

    int ancho = xFinFranja - xInicioFranja;
    
    // validar el ancho
    if (ancho <= 0) ancho = 75; 
    
    // procesar la imagen, negro hasta el final del borde
    for (int y = 0; y < filas; y++) {
        int limiteCorte = 0; 

        for (int x = 0; x < columnas; x++) {
            Vec3b pixel = imagen.at<Vec3b>(y, x);
            int b = pixel[0];
            int g = pixel[1];
            int r = pixel[2];

            bool esBordeIzquierdo = false;

            // deteccion del borde en cada fila
            // verde
            if (g > 50 && (g - r) > 40 && (g - b) > 40) {
                esBordeIzquierdo = true;
            }

            // magneta
            if ((r > 50 || b > 50) && (r - g) > 49 && (b - g) > 49) {
                esBordeIzquierdo = true;
            }

            // con el ancho calculado, se debe pintar desde el borde izquierdo hasta el final de la franja
            if (esBordeIzquierdo) {
                limiteCorte = x + ancho;
                limiteCorte += 2; 
                break;
            }
        }

        // pintar de negro
        if (limiteCorte > 0) {

            if (limiteCorte > columnas) {
                limiteCorte = columnas;
            };

            for (int x = 0; x < limiteCorte; x++) {
                resultado.at<Vec3b>(y, x) = Vec3b(0, 0, 0);
            }
        }
    }

    imshow("Imagen Original", imagen);
    imshow("Resultado - Estudiante 19", resultado);
    imwrite("Estudiante_19_Resultado.png", resultado);

    waitKey(0);
    return 0;
}