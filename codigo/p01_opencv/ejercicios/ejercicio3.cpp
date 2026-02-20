
//  EJERCICIO 3: Temporizador automático
 
//  Objetivo: Implementar un temporizador que cierre automáticamente la ventana después de 5 segundos
 
//  Compilación: g++ ejercicio3.cpp -o ejercicio3 `pkg-config --cflags --libs opencv4`
//  Ejecución: ./ejercicio3 <ruta_imagen>
 
//  Nota: Puedes presionar cualquier tecla antes de que termine el tiempo
//        para cerrar la ventana inmediatamente.


#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    if (argc != 2){
        cout << "Debe proporcionar una imagen" << endl;
        return -1;
    }

    Mat imagen = imread(argv[1]);

    if (imagen.empty()){
        cout << "Error: No se pudo cargar la imagen." << endl;
        cout << "Verifica que la ruta sea correcta: " << argv[1] << endl;
        return -1;
    }

    cout << "Imagen cargada" << endl;
    cout << "Imagen: " << argv[1] << endl;
    cout << "Dimensiones: " << imagen.cols << " x " << imagen.rows << endl;
    
    namedWindow("Imagen");
    imshow("Imagen", imagen);

    // ========== NUEVO: Temporizador automático de 5 segundos ==========
    const int TIEMPO_ESPERA = 5000; // 5000 milisegundos = 5 segundos
    
    cout << "\nLa ventana se cerrará automáticamente en 5 segundos..." << endl;
    cout << "   (O presiona cualquier tecla para cerrar antes)" << endl;
    
    // Contador regresivo visual
    for (int i = 5; i > 0; i--) {
        cout << "   " << i << "..." << flush;
        
        // waitKey espera 1000ms (1 segundo) y retorna -1 si no se presiona ninguna tecla
        int tecla = waitKey(1000);
        
        // Si se presionó alguna tecla (tecla != -1), salir del loop
        if (tecla != -1) {
            cout << "\n\n✓ Tecla presionada. Cerrando ventana..." << endl;
            destroyAllWindows();
            return 0;
        }
    }
    
    cout << "\n\n¡Tiempo agotado! Cerrando ventana automáticamente..." << endl;
    // =============================================================

    destroyAllWindows(); // Cerrar todas las ventanas abiertas
    
    cout << "Programa terminado." << endl;
    return 0;
}
