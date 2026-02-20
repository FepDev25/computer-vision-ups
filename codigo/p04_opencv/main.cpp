#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

// Función para calcular la distancia entre dos puntos
double calcularDistancia(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Función para contar dedos usando defectos de convexidad
int contarDedos(vector<Vec4i>& defectos, vector<Point>& contorno, Point centro) {
    int dedos = 0;
    
    for (size_t i = 0; i < defectos.size(); i++) {
        int inicio_idx = defectos[i][0];
        int fin_idx = defectos[i][1];
        int punto_mas_lejano_idx = defectos[i][2];
        
        Point inicio = contorno[inicio_idx];
        Point fin = contorno[fin_idx];
        Point mas_lejano = contorno[punto_mas_lejano_idx];
        
        // Calcular ángulo entre los puntos
        double a = calcularDistancia(inicio, mas_lejano);
        double b = calcularDistancia(fin, mas_lejano);
        double c = calcularDistancia(inicio, fin);
        
        // Ley de cosenos
        double angulo = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b)) * 180 / CV_PI;
        
        // Filtrar por ángulo (dedos forman ángulos menores a 90 grados)
        if (angulo < 90 && calcularDistancia(mas_lejano, centro) > 30) {
            dedos++;
        }
    }
    
    return dedos + 1; // +1 porque los dedos son defectos + 1
}

int main() {
    cout << "=== DETECCIÓN DE MANOS Y CONTEO DE DEDOS EN TIEMPO REAL ===" << endl;
    cout << "Instrucciones:" << endl;
    cout << "1. Coloca tu mano frente a la cámara" << endl;
    cout << "2. Mueve tu mano para ver la detección en tiempo real" << endl;
    cout << "3. El programa contará tus dedos automáticamente" << endl << endl;
    cout << "Controles:" << endl;
    cout << "  ESC - Salir" << endl;
    cout << "  C - Capturar imagen" << endl;
    cout << "  B - Cambiar fondo (para calibración)" << endl;
    cout << "  H - Mostrar/Ocultar ayuda" << endl << endl;

    // Abrir cámara
    VideoCapture camara(0);
    if (!camara.isOpened()) {
        cout << "Error: No se pudo abrir la cámara" << endl;
        return -1;
    }
    
    cout << "✓ Cámara iniciada correctamente" << endl << endl;

    // Variables
    Mat frame, frame_hsv, mascara_piel, frame_procesado;
    bool mostrar_ayuda = true;
    int foto_contador = 1;
    
    // Rango HSV para detección de color piel (ajustable)
    Scalar lower_skin = Scalar(0, 20, 70);
    Scalar upper_skin = Scalar(20, 255, 255);

    while (true) {
        camara >> frame;
        if (frame.empty()) break;

        // Voltear horizontalmente para efecto espejo
        flip(frame, frame, 1);
        
        // Convertir a HSV para mejor detección de piel
        cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
        
        // Crear máscara para color de piel
        inRange(frame_hsv, lower_skin, upper_skin, mascara_piel);
        
        // Aplicar operaciones morfológicas para limpiar ruido
        Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(11, 11));
        morphologyEx(mascara_piel, mascara_piel, MORPH_CLOSE, kernel);
        morphologyEx(mascara_piel, mascara_piel, MORPH_OPEN, kernel);
        
        // Aplicar desenfoque gaussiano para suavizar
        GaussianBlur(mascara_piel, mascara_piel, Size(5, 5), 0);

        // Encontrar contornos
        vector<vector<Point>> contornos;
        findContours(mascara_piel.clone(), contornos, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        // Buscar el contorno más grande (probablemente la mano)
        double area_maxima = 0;
        int idx_max = -1;
        
        for (size_t i = 0; i < contornos.size(); i++) {
            double area = contourArea(contornos[i]);
            if (area > area_maxima && area > 5000) { // Filtrar por área mínima
                area_maxima = area;
                idx_max = i;
            }
        }

        // Procesar si se encontró una mano
        if (idx_max >= 0) {
            vector<Point> contorno_mano = contornos[idx_max];
            
            // Dibujar contorno de la mano
            drawContours(frame, contornos, idx_max, Scalar(0, 255, 0), 2);
            
            // Calcular envolvente convexa
            vector<Point> hull_points;
            vector<int> hull_indices;
            convexHull(contorno_mano, hull_points, false);
            convexHull(contorno_mano, hull_indices, false);
            
            // Dibujar envolvente convexa
            drawContours(frame, vector<vector<Point>>{hull_points}, 0, Scalar(255, 0, 0), 2);
            
            // Calcular defectos de convexidad
            vector<Vec4i> defectos;
            if (hull_indices.size() > 3) {
                convexityDefects(contorno_mano, hull_indices, defectos);
            }
            
            // Calcular centro de la palma usando momentos
            Moments m = moments(contorno_mano);
            Point centro(m.m10 / m.m00, m.m01 / m.m00);
            circle(frame, centro, 8, Scalar(0, 0, 255), -1);
            putText(frame, "Centro", Point(centro.x - 30, centro.y - 15), 
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 255), 2);
            
            // Dibujar defectos de convexidad (espacios entre dedos)
            int dedos_contados = 0;
            for (size_t i = 0; i < defectos.size(); i++) {
                int punto_mas_lejano_idx = defectos[i][2];
                Point mas_lejano = contorno_mano[punto_mas_lejano_idx];
                
                double distancia_centro = calcularDistancia(mas_lejano, centro);
                
                // Calcular ángulo
                int inicio_idx = defectos[i][0];
                int fin_idx = defectos[i][1];
                Point inicio = contorno_mano[inicio_idx];
                Point fin = contorno_mano[fin_idx];
                
                double a = calcularDistancia(inicio, mas_lejano);
                double b = calcularDistancia(fin, mas_lejano);
                double c = calcularDistancia(inicio, fin);
                double angulo = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b)) * 180 / CV_PI;
                
                // Si es un defecto válido (espacio entre dedos)
                if (angulo < 90 && distancia_centro > 30) {
                    circle(frame, mas_lejano, 5, Scalar(255, 255, 0), -1);
                    line(frame, inicio, mas_lejano, Scalar(255, 255, 0), 2);
                    line(frame, fin, mas_lejano, Scalar(255, 255, 0), 2);
                    dedos_contados++;
                }
            }
            
            // Ajustar conteo (dedos = defectos + 1)
            int dedos_finales = min(dedos_contados + 1, 5);
            
            // Calcular rectángulo delimitador
            Rect rect = boundingRect(contorno_mano);
            rectangle(frame, rect, Scalar(255, 0, 255), 2);
            
            // Mostrar información de la mano
            putText(frame, "MANO DETECTADA", Point(10, 30), 
                    FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0), 2);
            
            putText(frame, "Dedos: " + to_string(dedos_finales), 
                    Point(10, 70), FONT_HERSHEY_DUPLEX, 1.5, Scalar(0, 255, 255), 3);
            
            putText(frame, "Area: " + to_string((int)area_maxima), 
                    Point(10, 110), FONT_HERSHEY_SIMPLEX, 0.7, Scalar(255, 255, 255), 2);
            
            // Mostrar gesto según dedos
            string gesto = "";
            switch(dedos_finales) {
                case 0: gesto = "PUNO CERRADO"; break;
                case 1: gesto = "UNO / APUNTAR"; break;
                case 2: gesto = "DOS / PAZ / VICTORIA"; break;
                case 3: gesto = "TRES"; break;
                case 4: gesto = "CUATRO"; break;
                case 5: gesto = "CINCO / MANO ABIERTA"; break;
            }
            
            putText(frame, gesto, Point(10, 150), 
                    FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255, 100, 255), 2);
        } else {
            putText(frame, "Coloca tu mano frente a la camara", 
                    Point(50, frame.rows / 2), FONT_HERSHEY_SIMPLEX, 1, 
                    Scalar(0, 0, 255), 2);
        }

        // Mostrar ayuda en pantalla
        if (mostrar_ayuda) {
            putText(frame, "ESC=Salir | C=Captura | B=Fondo | H=Ayuda", 
                    Point(10, frame.rows - 10), FONT_HERSHEY_SIMPLEX, 
                    0.6, Scalar(255, 255, 255), 2);
        }

        // Mostrar frames
        imshow("Deteccion de Manos - Original", frame);
        imshow("Deteccion de Manos - Mascara", mascara_piel);

        // Manejar teclas
        char tecla = (char)waitKey(10);
        
        if (tecla == 27) { // ESC
            cout << "Saliendo..." << endl;
            break;
        }
        else if (tecla == 'c' || tecla == 'C') { // Capturar
            String nombre = "captura_mano_" + to_string(foto_contador++) + ".jpg";
            imwrite(nombre, frame);
            cout << "✓ Captura guardada: " << nombre << endl;
        }
        else if (tecla == 'b' || tecla == 'B') { // Calibrar fondo
            cout << "Ajustando detección de piel..." << endl;
            // Podrías agregar calibración interactiva aquí
        }
        else if (tecla == 'h' || tecla == 'H') { // Toggle ayuda
            mostrar_ayuda = !mostrar_ayuda;
        }
    }

    camara.release();
    destroyAllWindows();
    
    cout << "\n¡Programa finalizado!" << endl;
    cout << "Gracias por usar el detector de manos!" << endl;
    
    return 0;
}
