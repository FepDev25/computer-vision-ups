#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    // **** 1. Imágenes a blanco y negro ****

    // Lllena de 0 es negro, 1 es blanco
    // CV_8UC1: 8 bits, unsigned, 1 canal
    Mat blancoNegro = Mat::zeros(Size(800, 600), CV_8UC1); 
    for (int i = 0; i < blancoNegro.rows; i++){
        for (int j=0; j < blancoNegro.cols; j++){
            // at: método para acceder s los píxeles, metodo generico
            if (i == j){
                blancoNegro.at<uchar>(i,j) = 255;
            }
            
        }
    }
    

    // **** 2. Imagen gris ****
    Mat gris = Mat(Size(800, 600), CV_8UC1, Scalar(255)); 
    for (int i = 0, cont = 0; i < gris.rows; i++){
        for (int j=0; j < gris.cols; j++){
            if (i == j){
                gris.at<uchar>(i,j) = cont;
                cont++;
                if (cont > 255){
                    cont = 0;
                }
            }
            
        }
    }

    // **** 3. Imagen a color ****
    Mat color = Mat(Size(800, 600), CV_8UC3, Scalar(255, 255, 255)); // En c++ es BGR
    for (int i = 0, cont = 0; i < color.rows; i++){
        for (int j=0; j < color.cols; j++){
            if (i == j){
                color.at<Vec3b>(i,j) = Vec3b(0, 233, 233); // BGR
                cont++;
                if (cont > 255){
                    cont = 0;
                }
            }
            
        }
    }

    // **** 4. Manipulación de pixeles ****
    // Divida la imagen en 4 cuadrantes y extraer los pixeles del cuadrante superior izquierdo 
    // y generar una nueva imagen
    Mat imagen = imread(argv[1]);
    int cx = imagen.cols / 2;
    int cy = imagen.rows / 2;
    Mat cuadrante = Mat::zeros(Size(cx, cy), imagen.type());

    for (int i = 0; i < cy; i++) {
        for (int j = 0; j < cx; j++) {
            cuadrante.at<Vec3b>(i, j) = imagen.at<Vec3b>(i, j);
        }
    }

    imwrite("cuadrante_superior_izquierdo.jpg", cuadrante);

    // Suponga que debe filmar un video de seguridad de 2 minutos de una camara que esta
    // en el patio de una casa. la camara tiene una capacidad para capturar un video con
    // imagenes de 2048 x 2048, cuanto pesa el video si se usa un formato sin compresion y guardamos
    // las imagenes con 3 canales y una profundidad de color de 2 bits. Considere que la camara
    // graba a 30 fotogramas por segundo.
    // tamanio = 2048 * 2048 * 3 *2
    // tamanio = 25165824 bytes
    // tiempo = 2 * 60 = 120
    // tamanio_final = tamanio * tiempo
    // tamanio_final = 25165824 * 120 = 3019898880 bytes

    // Resolucion: cantidad de pixeles
    // Profundidad de color: cuanta informacion tiene cada pixel

    // blanco y negro: 0 o 1, 0 o 255
    // 3 tipos de objetos relación a la luz: reflejar, absorve, traslucido
    // sensores: simple, lineal, CCD
    // cuantiazar

    // nivel gris:

    // 4 vecinos: cruz al rededor de un pixel
    // vecino: los 8 pixeles que están en las posiciones (x-1,y-1), (x-1,y), (x-1,y+1), (x,y-1), (x,y+1), (x+1,y-1), (x+1,y), (x+1,y+1)
    

    imshow("1. Blanco y Negro - Diagonal", blancoNegro);
    imshow("2. Escala de Grises - Diagonal", gris);
    imshow("3. Color - Diagonal", color);
    imshow("4. Cuadrante Superior Izquierdo", cuadrante);
    waitKey(0); 

    return 0;
}