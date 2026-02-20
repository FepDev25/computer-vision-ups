
//   EJERCICIO 1: Mostrar las dimensiones de la imagen
//   Objetivo: Modificar el programa para mostrar las dimensiones de la imagen (ancho x alto)
//   Compilación: g++ ejercicio1.cpp -o ejercicio1 `pkg-config --cflags --libs opencv4`
//   Ejecución: ./ejercicio1 <ruta_imagen>

#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    if (argc != 2){
        cout << "Se debe proporcionar una ruta de imagen." << endl;
        return -1;
    }

    Mat imagen = imread(argv[1]);

    if (imagen.empty()){
        cout << "Error: No se pudo cargar la imagen." << endl;
        cout << "Verifica que la ruta sea correcta: " << argv[1] << endl;
        return -1;
    }

    // ========== Mostrar dimensiones de la imagen ==========
    int ancho = imagen.cols;   // Número de columnas (ancho)
    int alto = imagen.rows;    // Número de filas (alto)
    
    cout << "\n=== INFORMACIÓN DE LA IMAGEN ===" << endl;
    cout << "Archivo: " << argv[1] << endl;
    cout << "Dimensiones: " << ancho << " x " << alto << " píxeles" << endl;
    cout << "Ancho: " << ancho << " píxeles" << endl;
    cout << "Alto: " << alto << " píxeles" << endl;
    cout << "Total de píxeles: " << (ancho * alto) << endl;
    cout << "================================\n" << endl;
    // =============================================================

    // Mostrar la imagen en una ventana
    namedWindow("Imagen Cargada", WINDOW_AUTOSIZE);
    imshow("Imagen Cargada", imagen);
    
    cout << "Presiona cualquier tecla para cerrar la ventana..." << endl;
    waitKey(0); // Esperar hasta que se presione una tecla

    destroyAllWindows(); // Cerrar todas las ventanas abiertas
    
    return 0;
}
