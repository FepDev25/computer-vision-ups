#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main() {
    cout << "=== DETECCIÓN DE ROSTROS Y OJOS EN TIEMPO REAL ===" << endl;
    cout << "Presiona 'ESC' para salir" << endl;
    cout << "Presiona 'c' para capturar una foto" << endl;
    cout << "Presiona 'g' para activar/desactivar escala de grises" << endl << endl;

    // Cargar clasificadores Haar Cascade para detección
    // Estos archivos vienen incluidos con OpenCV
    CascadeClassifier detector_rostros, detector_ojos;
    
    String ruta_opencv = "/home/felipep/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/share/opencv4/haarcascades/";
    
    if (!detector_rostros.load(ruta_opencv + "haarcascade_frontalface_default.xml")) {
        cout << "Error: No se pudo cargar el clasificador de rostros" << endl;
        cout << "Verifica la ruta: " << ruta_opencv << endl;
        return -1;
    }
    
    if (!detector_ojos.load(ruta_opencv + "haarcascade_eye.xml")) {
        cout << "Error: No se pudo cargar el clasificador de ojos" << endl;
        return -1;
    }
    
    cout << "✓ Clasificadores cargados exitosamente" << endl;

    // Abrir la cámara web (0 = cámara predeterminada)
    VideoCapture camara(0);
    if (!camara.isOpened()) {
        cout << "Error: No se pudo abrir la cámara" << endl;
        return -1;
    }
    
    cout << "✓ Cámara iniciada correctamente" << endl << endl;

    // Variables de configuración
    bool modo_gris = false;
    int foto_contador = 1;
    
    Mat frame, frame_gris;
    vector<Rect> rostros, ojos;

    // Bucle principal de captura de video
    while (true) {
        // Capturar frame de la cámara
        camara >> frame;
        if (frame.empty()) {
            cout << "Error: Frame vacío" << endl;
            break;
        }

        // Convertir a escala de grises para mejor detección
        cvtColor(frame, frame_gris, COLOR_BGR2GRAY);
        
        // Ecualizar histograma para mejorar el contraste
        equalizeHist(frame_gris, frame_gris);

        // DETECTAR ROSTROS
        detector_rostros.detectMultiScale(
            frame_gris,
            rostros,
            1.1,     // Factor de escala
            3,       // Mínimo de vecinos
            0,       // Flags
            Size(30, 30) // Tamaño mínimo del rostro
        );

        // Procesar cada rostro detectado
        for (size_t i = 0; i < rostros.size(); i++) {
            // Dibujar rectángulo alrededor del rostro
            rectangle(frame, rostros[i], Scalar(255, 0, 0), 3);
            
            // Agregar texto con el número de rostro
            putText(frame, 
                    "Rostro #" + to_string(i + 1), 
                    Point(rostros[i].x, rostros[i].y - 10),
                    FONT_HERSHEY_SIMPLEX, 
                    0.9, 
                    Scalar(255, 0, 0), 
                    2);

            // Región de interés (ROI) para buscar ojos solo dentro del rostro
            Mat roi_gris = frame_gris(rostros[i]);
            Mat roi_color = frame(rostros[i]);

            // DETECTAR OJOS dentro del rostro
            detector_ojos.detectMultiScale(
                roi_gris,
                ojos,
                1.1,
                5,
                0,
                Size(20, 20)
            );

            // Dibujar círculos en los ojos detectados
            for (size_t j = 0; j < ojos.size(); j++) {
                Point centro(
                    rostros[i].x + ojos[j].x + ojos[j].width / 2,
                    rostros[i].y + ojos[j].y + ojos[j].height / 2
                );
                int radio = cvRound((ojos[j].width + ojos[j].height) * 0.25);
                circle(frame, centro, radio, Scalar(0, 255, 0), 3);
                
                // Punto en el centro del ojo
                circle(frame, centro, 3, Scalar(0, 0, 255), -1);
            }
        }

        // Información en pantalla
        String info = "Rostros: " + to_string(rostros.size());
        putText(frame, info, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 2);
        
        // Mostrar FPS aproximado
        putText(frame, "ESC=Salir | C=Captura | G=Gris", 
                Point(10, frame.rows - 10), 
                FONT_HERSHEY_SIMPLEX, 
                0.6, 
                Scalar(255, 255, 255), 
                2);

        // Mostrar el frame (en color o gris según el modo)
        if (modo_gris) {
            imshow("Detección de Rostros y Ojos", frame_gris);
        } else {
            imshow("Detección de Rostros y Ojos", frame);
        }

        // Manejar entrada del teclado
        char tecla = (char)waitKey(10);
        
        if (tecla == 27) { // ESC
            cout << "Saliendo..." << endl;
            break;
        }
        else if (tecla == 'c' || tecla == 'C') { // Capturar foto
            String nombre_archivo = "captura_" + to_string(foto_contador++) + ".jpg";
            imwrite(nombre_archivo, frame);
            cout << "✓ Foto guardada: " << nombre_archivo << endl;
        }
        else if (tecla == 'g' || tecla == 'G') { // Toggle gris
            modo_gris = !modo_gris;
            cout << "Modo escala de grises: " << (modo_gris ? "ON" : "OFF") << endl;
        }
    }

    // Liberar recursos
    camara.release();
    destroyAllWindows();
    
    cout << "\n¡Programa finalizado!" << endl;
    return 0;
}
