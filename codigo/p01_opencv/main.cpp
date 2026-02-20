#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    // Pasar la ruta de una imagen como argumento
    if (argc != 2){
        cout << "Se debe proporcionar la ruta de una imagen." << endl;
        return -1;
    }

    // Cargar la imagen
    Mat imagen = imread(argv[1]);

    // Verificar si la imagen se cargó correctamente
    if (imagen.empty()) {
        cout << "No se pudo cargar la imagen." << endl;
        return -1;
    }

    // Mostrar la imagen en una ventana
    namedWindow("Imagen Cargada", WINDOW_AUTOSIZE);
    imshow("Imagen Cargada", imagen);
    waitKey(0); // Esperar hasta que se presione una tecla

    destroyAllWindows(); // Cerrar todas las ventanas abiertas

    
    return 0;
}