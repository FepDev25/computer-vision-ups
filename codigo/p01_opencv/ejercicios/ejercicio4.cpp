
//  EJERCICIO 4: Información completa de la imagen
 
//  Objetivo: Mostrar información adicional como el número de canales, tipo de datos, y otras propiedades de la imagen
 
//  Compilación: g++ ejercicio4.cpp -o ejercicio4 `pkg-config --cflags --libs opencv4`
//  Ejecución: ./ejercicio4 <ruta_imagen>

#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip>

using namespace std;
using namespace cv;

// Función auxiliar para obtener el nombre del tipo de dato
string obtenerNombreTipo(int tipo) {
    string tipoStr;
    
    int profundidad = tipo & CV_MAT_DEPTH_MASK;
    int canales = 1 + (tipo >> CV_CN_SHIFT);
    
    switch(profundidad) {
        case CV_8U:  tipoStr = "8U"; break;   // unsigned char
        case CV_8S:  tipoStr = "8S"; break;   // signed char
        case CV_16U: tipoStr = "16U"; break;  // unsigned short
        case CV_16S: tipoStr = "16S"; break;  // signed short
        case CV_32S: tipoStr = "32S"; break;  // int
        case CV_32F: tipoStr = "32F"; break;  // float
        case CV_64F: tipoStr = "64F"; break;  // double
        default:     tipoStr = "Desconocido"; break;
    }
    
    return "CV_" + tipoStr + "C" + to_string(canales);
}

// Función auxiliar para obtener descripción del tipo de dato
string obtenerDescripcionTipo(int tipo) {
    int profundidad = tipo & CV_MAT_DEPTH_MASK;
    
    switch(profundidad) {
        case CV_8U:  return "8 bits sin signo (0-255)";
        case CV_8S:  return "8 bits con signo (-128 a 127)";
        case CV_16U: return "16 bits sin signo (0-65535)";
        case CV_16S: return "16 bits con signo (-32768 a 32767)";
        case CV_32S: return "32 bits entero con signo";
        case CV_32F: return "32 bits punto flotante";
        case CV_64F: return "64 bits punto flotante (doble precisión)";
        default:     return "Tipo desconocido";
    }
}

int main(int argc, char** argv) {
    if (argc != 2){
        cout << "Uso: " << argv[0] << " <ruta_imagen>" << endl;
        cout << "Se debe proporcionar la ruta de una imagen." << endl;
        return -1;
    }

    Mat imagen = imread(argv[1]);

    if (imagen.empty()) {
        cout << "Error: No se pudo cargar la imagen." << endl;
        cout << "Verifica que la ruta sea correcta: " << argv[1] << endl;
        return -1;
    }

    // ========== NUEVO: Mostrar información completa de la imagen ==========
    cout << "INFORMACIÓN COMPLETA DE LA IMAGEN \n" << endl;
    
    // 1. Información del archivo
    cout << "* ARCHIVO:" << endl;
    cout << "   Ruta: " << argv[1] << endl;
    cout << endl;
    
    // 2. Dimensiones
    cout << "* DIMENSIONES:" << endl;
    cout << "   Ancho: " << imagen.cols << " píxeles" << endl;
    cout << "   Alto: " << imagen.rows << " píxeles" << endl;
    cout << "   Total de píxeles: " << (imagen.cols * imagen.rows) << endl;
    cout << "   Resolución: " << imagen.cols << " x " << imagen.rows << endl;
    
    // Calcular aspecto
    double aspectRatio = (double)imagen.cols / imagen.rows;
    cout << "   Relación de aspecto: " << fixed << setprecision(2) << aspectRatio << ":1" << endl;
    cout << endl;
    
    // 3. Información de canales
    cout << "* CANALES:" << endl;
    int numCanales = imagen.channels();
    cout << "   Número de canales: " << numCanales << endl;
    
    string tipoImagen;
    if (numCanales == 1) {
        tipoImagen = "Escala de grises";
    } else if (numCanales == 3) {
        tipoImagen = "Color BGR (Blue, Green, Red)";
    } else if (numCanales == 4) {
        tipoImagen = "Color BGRA (con canal Alpha/transparencia)";
    } else {
        tipoImagen = "Multicanal (" + to_string(numCanales) + " canales)";
    }
    cout << "   Tipo de imagen: " << tipoImagen << endl;
    cout << endl;
    
    // 4. Tipo de datos
    cout << "* TIPO DE DATOS:" << endl;
    int tipo = imagen.type();
    cout << "   Tipo OpenCV: " << obtenerNombreTipo(tipo) << endl;
    cout << "   Descripción: " << obtenerDescripcionTipo(tipo) << endl;
    
    int profundidad = tipo & CV_MAT_DEPTH_MASK;
    int bytesporElemento;
    switch(profundidad) {
        case CV_8U:
        case CV_8S:  bytesporElemento = 1; break;
        case CV_16U:
        case CV_16S: bytesporElemento = 2; break;
        case CV_32S:
        case CV_32F: bytesporElemento = 4; break;
        case CV_64F: bytesporElemento = 8; break;
        default:     bytesporElemento = 0; break;
    }
    cout << "   Bytes por elemento: " << bytesporElemento << endl;
    cout << endl;
    
    // 5. Memoria
    cout << "* MEMORIA:" << endl;
    size_t totalBytes = imagen.total() * imagen.elemSize();
    double totalKB = totalBytes / 1024.0;
    double totalMB = totalKB / 1024.0;
    
    cout << "   Tamaño en memoria: ";
    if (totalMB >= 1.0) {
        cout << fixed << setprecision(2) << totalMB << " MB" << endl;
    } else {
        cout << fixed << setprecision(2) << totalKB << " KB" << endl;
    }
    cout << "   (" << totalBytes << " bytes)" << endl;
    cout << "   Bytes por píxel: " << imagen.elemSize() << endl;
    cout << "   Continuidad en memoria: " << (imagen.isContinuous() ? "Sí" : "No") << endl;
    cout << endl;
    
    // 6. Propiedades adicionales
    cout << "*  PROPIEDADES ADICIONALES:" << endl;
    cout << "   Es matriz vacía: " << (imagen.empty() ? "Sí" : "No") << endl;
    cout << "   Número de dimensiones: " << imagen.dims << endl;
    cout << "   Step (bytes por fila): " << imagen.step << endl;
    cout << endl;
    
    // 7. Rango de valores (para imágenes pequeñas)
    if (imagen.total() < 10000) { // Solo si la imagen es pequeña
        double minVal, maxVal;
        minMaxLoc(imagen.reshape(1), &minVal, &maxVal); // reshape(1) convierte a 1 canal
        cout << "* ESTADÍSTICAS DE PÍXELES:" << endl;
        cout << "   Valor mínimo: " << minVal << endl;
        cout << "   Valor máximo: " << maxVal << endl;
        
        Scalar meanVal = mean(imagen);
        cout << "   Valor promedio por canal: ";
        for (int i = 0; i < numCanales; i++) {
            cout << fixed << setprecision(1) << meanVal[i];
            if (i < numCanales - 1) cout << ", ";
        }
        cout << endl << endl;
    }
    
    // Mostrar la imagen en una ventana
    namedWindow("Imagen Cargada", WINDOW_AUTOSIZE);
    imshow("Imagen Cargada", imagen);
    
    cout << "\nPresiona cualquier tecla para cerrar la ventana..." << endl;
    waitKey(0); // Esperar hasta que se presione una tecla

    destroyAllWindows(); // Cerrar todas las ventanas abiertas
    
    return 0;
}
