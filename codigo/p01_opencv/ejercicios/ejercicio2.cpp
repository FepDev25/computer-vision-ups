
//  EJERCICIO 2: Procesar múltiples imágenes
 
//  Objetivo: Agregar la capacidad de procesar múltiples imágenes desde la línea de comandos
 
//  Compilación: g++ ejercicio2.cpp -o ejercicio2 `pkg-config --cflags --libs opencv4`
//  Ejecución: ./ejercicio2 <imagen1> <imagen2> <imagen3> ...
//  Ejemplo: ./ejercicio2 foto1.jpg foto2.png foto3.jpg


#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    // Verificar que se proporcionaron imágenes
    if (argc < 2){
        cout << "Uso: " << argv[0] << " <imagen1> [imagen2] [imagen3] ..." << endl;
        cout << "Se debe proporcionar al menos una ruta de imagen." << endl;
        cout << "\nEjemplo:" << endl;
        cout << "  " << argv[0] << " foto1.jpg foto2.png" << endl;
        return -1;
    }

    // ========== NUEVO: Procesar múltiples imágenes ==========
    int totalImagenes = argc - 1;
    int imagenesValidas = 0;
    int imagenesInvalidas = 0;
    
    cout << "\n=== PROCESANDO " << totalImagenes << " IMAGEN(ES) ===" << endl;
    
    // Vector para almacenar las imágenes cargadas exitosamente
    vector<Mat> imagenes;
    vector<string> nombresArchivos;
    
    // Cargar todas las imágenes
    for (int i = 1; i < argc; i++) {
        cout << "\n[" << i << "/" << totalImagenes << "] Cargando: " << argv[i] << endl;
        
        Mat imagen = imread(argv[i]);
        
        if (imagen.empty()) {
            cout << "  ✗ Error: No se pudo cargar la imagen." << endl;
            imagenesInvalidas++;
        } else {
            cout << "  ✓ Imagen cargada exitosamente" << endl;
            cout << "    Dimensiones: " << imagen.cols << " x " << imagen.rows << endl;
            imagenes.push_back(imagen);
            nombresArchivos.push_back(argv[i]);
            imagenesValidas++;
        }
    }
    
    // Resumen de carga
    cout << "\n=== RESUMEN DE CARGA ===" << endl;
    cout << "Total de imágenes: " << totalImagenes << endl;
    cout << "Cargadas exitosamente: " << imagenesValidas << endl;
    cout << "Fallidas: " << imagenesInvalidas << endl;
    
    // Si no se cargó ninguna imagen, salir
    if (imagenesValidas == 0) {
        cout << "\nNo se pudo cargar ninguna imagen. Programa terminado." << endl;
        return -1;
    }
    
    // Mostrar cada imagen en su propia ventana
    cout << "\n=== MOSTRANDO IMÁGENES ===" << endl;
    cout << "Presiona cualquier tecla en una ventana para pasar a la siguiente." << endl;
    cout << "Presiona ESC para salir en cualquier momento.\n" << endl;
    
    for (size_t i = 0; i < imagenes.size(); i++) {
        string nombreVentana = "Imagen " + to_string(i + 1) + "/" +  to_string(imagenesValidas) + ": " +  nombresArchivos[i];
        
        namedWindow(nombreVentana, WINDOW_AUTOSIZE);
        imshow(nombreVentana, imagenes[i]);
        
        cout << "Mostrando: " << nombresArchivos[i] << endl;
        cout << "  Dimensiones: " << imagenes[i].cols << " x " << imagenes[i].rows << endl;
        
        int tecla = waitKey(0); // Esperar hasta que se presione una tecla
        
        // Si se presiona ESC (código 27), salir
        if (tecla == 27) {
            cout << "\nSaliendo..." << endl;
            break;
        }
        
        destroyWindow(nombreVentana); // Cerrar la ventana actual
    }

    destroyAllWindows(); // Cerrar todas las ventanas restantes
    
    cout << "\nPrograma terminado." << endl;
    return 0;
}
