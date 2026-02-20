#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace cv;

// Variables Globales
Mat img_original, img_display;
int centro_x = -1, centro_y = -1;

// Valores iniciales aproximados
double param_a = 7.0;      
double param_fase = 0.0;    
double sentido = 1.0;      

void actualizarDibujo() {
    img_display = img_original.clone();
    
    // Dibujar cruz en el centro
    drawMarker(img_display, Point(centro_x, centro_y), Scalar(0, 255, 0), MARKER_CROSS, 20, 2);

    if (centro_x == -1) {
        putText(img_display, "HAZ CLICK EN EL CENTRO", Point(20, 50), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,0,255), 2);
        imshow("Calibrador Final", img_display);
        return;
    }

    string texto1 = "Centro X: " + to_string(centro_x) + " | Centro Y: " + to_string(centro_y);
    string texto2 = "Apertura (a): " + to_string(param_a).substr(0,4) + " | Fase: " + to_string(param_fase).substr(0,4);

    rectangle(img_display, Point(0,0), Point(650, 100), Scalar(0,0,0), -1);
    
    putText(img_display, texto1, Point(10, 30), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
    putText(img_display, texto2, Point(10, 60), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 255), 2);
    putText(img_display, "[Flechas] Mover Centro  [W/S] Apertura  [A/D] Fase", Point(10, 90), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(200, 200, 200), 1);

    // --- CÁLCULO ---
    double theta = 0.0;
    bool dentro = true;

    while (dentro) {
        double r = param_a * theta; 
        double angulo = (theta * sentido) + param_fase;
        int x = (int)(centro_x + r * cos(angulo));
        int y = (int)(centro_y + r * sin(angulo));

        if (r > max(img_original.cols, img_original.rows)) dentro = false;

        if (x >= 0 && x < img_original.cols && y >= 0 && y < img_original.rows) {
             circle(img_display, Point(x, y), 1, Scalar(0, 0, 0), -1);
        }
        
        double delta = (r < 1.0) ? 0.1 : (1.0 / r);
        theta += delta;
    }

    imshow("Calibrador Final", img_display);
}

void onMouse(int event, int x, int y, int, void*) {
    if (event == EVENT_LBUTTONDOWN) {
        centro_x = x;
        centro_y = y;
        actualizarDibujo();
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        cout << "Uso: ./programa <imagen>" << endl;
        return -1;
    }
    img_original = imread(argv[1]);
    if (img_original.empty()) {
        cout << "Error al cargar imagen" << endl;
        return -1;
    }

    centro_x = img_original.cols / 2;
    centro_y = img_original.rows / 2;

    namedWindow("Calibrador Final");
    setMouseCallback("Calibrador Final", onMouse);

    cout << "--- CONTROLES ---" << endl;
    cout << "CLICK / FLECHAS: Mover centro" << endl;
    cout << "W / S: Ajustar Apertura" << endl;
    cout << "A / D: Ajustar Rotacion (Fase)" << endl;
    cout << "ESPACIO: Imprimir parametros finales y guardar imagen" << endl;
    cout << "ESC: Salir" << endl;

    actualizarDibujo();

    while (true) {
        int k = waitKey(0); // Usamos int para capturar flechas correctamente
        if (k == 27) break; 
        
        // --- AJUSTE DE PARÁMETROS ---
        if (k == 'w' || k == 'W') param_a += 0.01;
        if (k == 's' || k == 'S') param_a -= 0.01;
        if (k == 'd' || k == 'D') param_fase += 0.01; 
        if (k == 'a' || k == 'A') param_fase -= 0.01; 
        
        // Arriba
        if (k == 82 || k == 'i' || k == 'I') centro_y -= 1;
        // Abajo
        if (k == 84 || k == 'k' || k == 'K') centro_y += 1;
        // Izquierda
        if (k == 81 || k == 'j' || k == 'J') centro_x -= 1;
        // Derecha
        if (k == 83 || k == 'l' || k == 'L') centro_x += 1;

        if (k == ' ') {
            // IMPRIMIR DATOS
            cout << "\n\n========================================" << endl;
            cout << "   ¡DATOS EXACTOS PARA TU CÓDIGO!   " << endl;
            cout << "========================================" << endl;
            cout << "int cx = " << centro_x << ";" << endl;
            cout << "int cy = " << centro_y << ";" << endl;
            cout << "double a = " << param_a << ";" << endl;
            cout << "double fase = " << param_fase << ";" << endl;
            cout << "========================================\n" << endl;
            
            // Generar imagen
            vector<Vec3b> extraidos;
            double t = 0;
            bool in = true;
            int px = -1, py = -1;
            while(in) {
                double r = param_a * t;
                if (r > max(img_original.cols, img_original.rows)) in = false;
                int x = centro_x + r * cos(t * sentido + param_fase);
                int y = centro_y + r * sin(t * sentido + param_fase);
                if (x>=0 && x<img_original.cols && y>=0 && y<img_original.rows) {
                    if (x!=px || y!=py) {
                        extraidos.push_back(img_original.at<Vec3b>(y, x));
                        px=x; py=y;
                    }
                }
                t += (r < 1.0 ? 0.1 : 1.0/r);
            }
            int lado = ceil(sqrt(extraidos.size()));
            Mat res = Mat::zeros(lado, lado, img_original.type());
            for(size_t i=0; i<extraidos.size(); i++) res.at<Vec3b>(i/lado, i%lado) = extraidos[i];
            
            imwrite("resultado_calibrado.jpg", res);
            imshow("Resultado Generado", res);
        }

        actualizarDibujo();
    }
    return 0;
}