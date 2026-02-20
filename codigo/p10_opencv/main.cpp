
#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>

using namespace std;
using namespace cv;

// Variables globales para trackbars
int kSize = 1;          // Tamaño del kernel
int morph_elem = 0;     // 0: Cruz, 1: Rectángulo, 2: Elipse
int iterations = 1;     // Número de iteraciones

void eventoTrack(int, void*) {
    // Esta función se llama cuando cambia algún trackbar
}

Mat crearCanvas(vector<Mat>& imagenes, vector<string>& titulos, int filas, int cols) {
    // Asegurar que todas las imágenes tengan 3 canales
    vector<Mat> imagenesColor;
    for (auto& img : imagenes) {
        Mat temp;
        if (img.channels() == 1) {
            cvtColor(img, temp, COLOR_GRAY2BGR);
        } else {
            temp = img.clone();
        }
        imagenesColor.push_back(temp);
    }
    
    int anchoImg = imagenesColor[0].cols;
    int altoImg = imagenesColor[0].rows;
    int margen = 30;
    int espacioTitulo = 25;
    
    // Crear canvas
    int anchoCanvas = cols * anchoImg + (cols + 1) * margen;
    int altoCanvas = filas * (altoImg + espacioTitulo) + (filas + 1) * margen;
    Mat canvas(altoCanvas, anchoCanvas, CV_8UC3, Scalar(40, 40, 40));
    
    // Colocar imágenes en el canvas
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            if (idx >= imagenesColor.size()) break;
            
            int x = margen + j * (anchoImg + margen);
            int y = margen + espacioTitulo + i * (altoImg + espacioTitulo + margen);
            
            // Copiar imagen
            imagenesColor[idx].copyTo(canvas(Rect(x, y, anchoImg, altoImg)));
            
            // Agregar título
            putText(canvas, titulos[idx], Point(x, y - 5),
                    FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 255, 255), 1);
        }
    }
    
    return canvas;
}

int main(int argc, char *argv[]){
    (void)argc;
    (void)argv;

    VideoCapture video(0);

    if (!video.isOpened()) {
        cerr << "Error: No se pudo abrir la cámara" << endl;
        return -1;
    }

    // Crear ventana principal y trackbars
    namedWindow("Operaciones Morfologicas", WINDOW_NORMAL);
    createTrackbar("Kernel Size", "Operaciones Morfologicas", &kSize, 10, eventoTrack);
    createTrackbar("Elem: 0=Cruz 1=Rect 2=Elipse", "Operaciones Morfologicas", &morph_elem, 2, eventoTrack);
    createTrackbar("Iteraciones", "Operaciones Morfologicas", &iterations, 5, eventoTrack);

    Mat frame, gris;
    
    cout << "\n╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║        Operaciones Morfológicas - OpenCV                 ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";
    cout << "\n Controles:\n";
    cout << "   - Kernel Size: Tamaño del elemento estructurante\n";
    cout << "   - Elem: 0=Cruz, 1=Rectángulo, 2=Elipse\n";
    cout << "   - Iteraciones: Número de veces que se aplica la operación\n";
    cout << "   - ESC: Salir\n\n";

    while(true){
        video >> frame;
        
        if (frame.empty()) {
            cerr << "Error: Frame vacío" << endl;
            break;
        }
        
        // Redimensionar para mejor visualización
        resize(frame, frame, Size(320, 240));
        
        // Convertir a escala de grises
        cvtColor(frame, gris, COLOR_BGR2GRAY);
        
        // Calcular tamaño de kernel (debe ser impar)
        int kernelSize = kSize * 2 + 1;
        if (kernelSize < 1) kernelSize = 1;
        
        // Seleccionar tipo de elemento estructurante
        int morphType;
        if (morph_elem == 0) morphType = MORPH_CROSS;
        else if (morph_elem == 1) morphType = MORPH_RECT;
        else morphType = MORPH_ELLIPSE;
        
        Mat elemento = getStructuringElement(morphType, 
                                            Size(kernelSize, kernelSize), 
                                            Point(-1, -1));
        
        int iters = max(1, iterations);
        
        // ============================================
        // APLICAR TODAS LAS OPERACIONES MORFOLÓGICAS
        // ============================================
        
        Mat erosion, dilatacion, opening, closing;
        Mat gradiente, tophat, blackhat;
        Mat contraste;
        
        // 1. Erosión - Elimina bordes
        erode(gris, erosion, elemento, Point(-1, -1), iters);
        
        // 2. Dilatación - Engrosa áreas
        dilate(gris, dilatacion, elemento, Point(-1, -1), iters);
        
        // 3. Opening - Erosión + Dilatación (elimina ruido pequeño)
        morphologyEx(gris, opening, MORPH_OPEN, elemento, Point(-1, -1), iters);
        
        // 4. Closing - Dilatación + Erosión (cierra huecos)
        morphologyEx(gris, closing, MORPH_CLOSE, elemento, Point(-1, -1), iters);
        
        // 5. Gradiente - Dilatación - Erosión (resalta bordes)
        morphologyEx(gris, gradiente, MORPH_GRADIENT, elemento, Point(-1, -1), iters);
        
        // 6. Top Hat - Imagen original - Opening (resalta objetos claros)
        morphologyEx(gris, tophat, MORPH_TOPHAT, elemento, Point(-1, -1), iters);
        
        // 7. Black Hat - Closing - Imagen original (resalta objetos oscuros)
        morphologyEx(gris, blackhat, MORPH_BLACKHAT, elemento, Point(-1, -1), iters);
        
        // 8. Mejora de contraste: Original + (Top Hat - Black Hat)
        Mat diferencia;
        subtract(tophat, blackhat, diferencia);
        add(gris, diferencia, contraste);
        
        // ============================================
        // CREAR CANVAS CON TODAS LAS OPERACIONES
        // ============================================
        
        vector<Mat> imagenes = {
            gris, erosion, dilatacion, opening,
            closing, gradiente, tophat, blackhat,
            diferencia, contraste
        };
        
        vector<string> titulos = {
            "1. Original (Gris)",
            "2. Erosion",
            "3. Dilatacion", 
            "4. Opening",
            "5. Closing",
            "6. Gradiente",
            "7. Top Hat",
            "8. Black Hat",
            "9. TopHat - BlackHat",
            "10. Contraste Mejorado"
        };
        
        // Crear canvas 4x3 (10 imágenes + 2 espacios vacíos)
        Mat canvas = crearCanvas(imagenes, titulos, 3, 4);
        
        // Agregar información de parámetros
        string kernelInfo = "Kernel: " + to_string(kernelSize) + "x" + to_string(kernelSize);
        string elemInfo = "Elem: " + string(morph_elem == 0 ? "Cruz" : 
                                           morph_elem == 1 ? "Rectangulo" : "Elipse");
        string iterInfo = "Iteraciones: " + to_string(iters);
        
        putText(canvas, kernelInfo, Point(20, canvas.rows - 60),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 255), 2);
        putText(canvas, elemInfo, Point(20, canvas.rows - 35),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 255), 2);
        putText(canvas, iterInfo, Point(20, canvas.rows - 10),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 255), 2);
        
        // Mostrar canvas
        imshow("Operaciones Morfologicas", canvas);

        // Salir con ESC
        if(waitKey(30) == 27)
            break;
    }
    
    video.release();
    destroyAllWindows();
    
    cout << "\nPrograma finalizado\n\n";

    return 0;
}