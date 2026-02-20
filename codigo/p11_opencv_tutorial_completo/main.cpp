/*
 * ═══════════════════════════════════════════════════════════════════════════
 * TUTORIAL COMPLETO: PROCESAMIENTO DIGITAL DE IMÁGENES CON OPENCV
 * ═══════════════════════════════════════════════════════════════════════════
 * 
 * Este programa es un tutorial exhaustivo que cubre todas las técnicas
 * fundamentales de Procesamiento Digital de Imágenes (PDI) con OpenCV.
 * 
 * CONTENIDO:
 * 1. Carga y exploración de imágenes
 * 2. Información y propiedades de imágenes
 * 3. Conversiones de espacios de color (RGB, HSV, LAB, Grises)
 * 4. Operaciones básicas (brillo, contraste, rotación, escalado)
 * 5. Filtrado y suavizado (Gaussiano, Mediana, Bilateral)
 * 6. Detección de bordes (Sobel, Laplaciano, Canny)
 * 7. Transformaciones morfológicas (Erosión, Dilatación, Apertura, Cierre)
 * 8. Umbralización y binarización (Simple, Adaptativa, Otsu)
 * 9. Histogramas y ecualización (Histograma, Equalización, CLAHE)
 * 10. Detección de contornos y formas
 * 11. Operaciones lógicas entre imágenes (AND, OR, XOR, NOT)
 * 12. Segmentación por color
 * 
 * Compilación:
 *   g++ main.cpp -o tutorial `pkg-config --cflags --libs opencv4`
 * 
 * O usando el Makefile:
 *   make
 * 
 * Uso:
 *   ./tutorial <ruta_imagen.jpg>
 * 
 * ═══════════════════════════════════════════════════════════════════════════
 */

#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip>

using namespace std;
using namespace cv;

// ═══════════════════════════════════════════════════════════════════════════
// FUNCIONES AUXILIARES PARA MOSTRAR INFORMACIÓN
// ═══════════════════════════════════════════════════════════════════════════

void imprimirSeparador(const string& titulo = "") {
    cout << "\n" << string(80, '═') << endl;
    if (!titulo.empty()) {
        int padding = (80 - titulo.length() - 2) / 2;
        cout << string(padding, ' ') << titulo << endl;
        cout << string(80, '═') << endl;
    }
}

void imprimirSeccion(const string& numero, const string& titulo) {
    cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║ " << numero << ". " << left << setw(70) << titulo << " ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝" << endl;
}

// Convierte el tipo de dato de OpenCV a string legible
string obtenerTipoDato(int tipo) {
    string r;
    uchar depth = tipo & CV_MAT_DEPTH_MASK;
    uchar chans = 1 + (tipo >> CV_CN_SHIFT);
    
    switch (depth) {
        case CV_8U:  r = "8U"; break;
        case CV_8S:  r = "8S"; break;
        case CV_16U: r = "16U"; break;
        case CV_16S: r = "16S"; break;
        case CV_32S: r = "32S"; break;
        case CV_32F: r = "32F"; break;
        case CV_64F: r = "64F"; break;
        default:     r = "User"; break;
    }
    
    r += "C";
    r += (chans + '0');
    return r;
}

// Muestra una imagen en una ventana con redimensionamiento automático
void mostrarImagen(const string& ventana, const Mat& imagen, bool esperar = false) {
    namedWindow(ventana, WINDOW_NORMAL);
    resizeWindow(ventana, 600, 450);
    imshow(ventana, imagen);
    if (esperar) {
        cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
        waitKey(0);
        destroyWindow(ventana);
    }
}   

// ═══════════════════════════════════════════════════════════════════════════
// FUNCIÓN PRINCIPAL
// ═══════════════════════════════════════════════════════════════════════════

int main(int argc, char** argv) {
    
    imprimirSeparador("TUTORIAL COMPLETO DE PROCESAMIENTO DIGITAL DE IMÁGENES");
    cout << "Este programa demostrará paso a paso las técnicas de PDI con OpenCV" << endl;
    imprimirSeparador();
    
    // Verificar argumentos
    if (argc < 2) {
        cerr << "Error: Debes proporcionar una imagen como argumento." << endl;
        cerr << "Uso: " << argv[0] << " <ruta_imagen.jpg>" << endl;
        return -1;
    }
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 1. CARGA DE IMAGEN
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("1", "CARGA Y VERIFICACIÓN DE IMAGEN");
    
    cout << "\n📁 Cargando imagen desde: " << argv[1] << endl;
    
    // imread() lee la imagen desde el disco
    // Por defecto, carga en formato BGR (Blue, Green, Red)
    Mat imagen_original = imread(argv[1], IMREAD_COLOR);
    
    // Verificar si la carga fue exitosa
    if (imagen_original.empty()) {
        cerr << "\n✗ Error: No se pudo cargar la imagen." << endl;
        cerr << "  Verifica que la ruta sea correcta y el formato soportado." << endl;
        cerr << "  Formatos soportados: JPG, PNG, BMP, TIFF, etc." << endl;
        return -1;
    }
    
    cout << "✓ Imagen cargada exitosamente" << endl;
    mostrarImagen("1. Imagen Original", imagen_original, true);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 2. INFORMACIÓN DE LA IMAGEN
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("2", "INFORMACIÓN Y PROPIEDADES DE LA IMAGEN");
    
    cout << "\n📊 DIMENSIONES:" << endl;
    cout << "   • Ancho (columnas): " << imagen_original.cols << " píxeles" << endl;
    cout << "   • Alto (filas):     " << imagen_original.rows << " píxeles" << endl;
    cout << "   • Tamaño total:     " << imagen_original.total() << " píxeles" << endl;
    cout << "   • Relación aspecto: " << fixed << setprecision(2) 
         << (float)imagen_original.cols / imagen_original.rows << ":1" << endl;
    
    cout << "\n🎨 CANALES Y TIPO:" << endl;
    cout << "   • Canales:          " << imagen_original.channels() << " (BGR)" << endl;
    cout << "   • Tipo de dato:     " << obtenerTipoDato(imagen_original.type()) << endl;
    cout << "   • Profundidad:      " << imagen_original.depth() << " bits por canal" << endl;
    cout << "   • Bytes por píxel:  " << imagen_original.elemSize() << " bytes" << endl;
    
    cout << "\n💾 ALMACENAMIENTO:" << endl;
    size_t bytes_totales = imagen_original.total() * imagen_original.elemSize();
    cout << "   • Tamaño en memoria: " << bytes_totales / 1024.0 / 1024.0 << " MB" << endl;
    cout << "   • Continua:          " << (imagen_original.isContinuous() ? "Sí" : "No") << endl;
    
    // Calcular estadísticas por canal
    vector<Mat> canales_bgr;
    split(imagen_original, canales_bgr);
    
    cout << "\n📈 ESTADÍSTICAS POR CANAL:" << endl;
    string nombres[] = {"Azul (B)", "Verde (G)", "Rojo (R)"};
    for (int i = 0; i < 3; i++) {
        Scalar media = mean(canales_bgr[i]);
        double minVal, maxVal;
        minMaxLoc(canales_bgr[i], &minVal, &maxVal);
        cout << "   • " << nombres[i] << ":" << endl;
        cout << "     - Mínimo:  " << (int)minVal << endl;
        cout << "     - Máximo:  " << (int)maxVal << endl;
        cout << "     - Promedio: " << fixed << setprecision(2) << media[0] << endl;
    }
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 3. CONVERSIONES DE ESPACIOS DE COLOR
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("3", "CONVERSIONES DE ESPACIOS DE COLOR");
    
    cout << "\n🎨 Los espacios de color representan colores de diferentes formas:" << endl;
    cout << "   • BGR: Blue-Green-Red (estándar en OpenCV)" << endl;
    cout << "   • RGB: Red-Green-Blue (estándar en displays)" << endl;
    cout << "   • HSV: Hue-Saturation-Value (tono-saturación-valor)" << endl;
    cout << "   • LAB: Lightness-A-B (luminosidad-verde/rojo-azul/amarillo)" << endl;
    cout << "   • GRAY: Escala de grises (un solo canal)" << endl;
    
    // Convertir a escala de grises
    // cvtColor() convierte entre espacios de color
    Mat imagen_gris;
    cvtColor(imagen_original, imagen_gris, COLOR_BGR2GRAY);
    cout << "\n✓ Conversión a escala de grises (GRAY):" << endl;
    cout << "  Fórmula: Gray = 0.299*R + 0.587*G + 0.114*B" << endl;
    cout << "  Resultado: 1 canal, valores de 0 (negro) a 255 (blanco)" << endl;
    mostrarImagen("3a. Escala de Grises", imagen_gris, false);
    
    // Convertir a HSV
    Mat imagen_hsv;
    cvtColor(imagen_original, imagen_hsv, COLOR_BGR2HSV);
    cout << "\n✓ Conversión a HSV (Hue-Saturation-Value):" << endl;
    cout << "  • H (Matiz):      0-180° (en OpenCV, 0-180)" << endl;
    cout << "  • S (Saturación): 0-255 (intensidad del color)" << endl;
    cout << "  • V (Valor):      0-255 (brillo)" << endl;
    cout << "  Útil para: Segmentación por color, detección de objetos" << endl;
    mostrarImagen("3b. Espacio HSV", imagen_hsv, false);
    
    // Convertir a LAB
    Mat imagen_lab;
    cvtColor(imagen_original, imagen_lab, COLOR_BGR2Lab);
    cout << "\n✓ Conversión a LAB (CIE L*a*b*):" << endl;
    cout << "  • L (Luminosidad): 0-255 (oscuro a claro)" << endl;
    cout << "  • a: 0-255 (verde a rojo)" << endl;
    cout << "  • b: 0-255 (azul a amarillo)" << endl;
    cout << "  Útil para: Corrección de color, comparación perceptual" << endl;
    mostrarImagen("3c. Espacio LAB", imagen_lab, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 4. OPERACIONES BÁSICAS
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("4", "OPERACIONES BÁSICAS: BRILLO, CONTRASTE, TRANSFORMACIONES");
    
    cout << "\n💡 AJUSTE DE BRILLO Y CONTRASTE:" << endl;
    cout << "   Fórmula: nueva_imagen = alfa * imagen + beta" << endl;
    cout << "   • alfa (contraste): multiplica los valores (1.0 = sin cambio)" << endl;
    cout << "   • beta (brillo): suma a los valores (0 = sin cambio)" << endl;
    
    Mat imagen_brillante;
    double alfa = 1.0;  // Contraste (1.0 sin cambio, >1 más contraste, <1 menos)
    int beta = 50;      // Brillo (positivo más claro, negativo más oscuro)
    imagen_original.convertTo(imagen_brillante, -1, alfa, beta);
    
    cout << "\n✓ Imagen con brillo aumentado (beta = +50):" << endl;
    mostrarImagen("4a. Brillo Aumentado", imagen_brillante, false);
    
    Mat imagen_contraste;
    alfa = 1.5;
    beta = 0;
    imagen_original.convertTo(imagen_contraste, -1, alfa, beta);
    
    cout << "✓ Imagen con contraste aumentado (alfa = 1.5):" << endl;
    mostrarImagen("4b. Contraste Aumentado", imagen_contraste, false);
    
    // Rotación
    cout << "\n🔄 ROTACIÓN:" << endl;
    cout << "   Usar getRotationMatrix2D() para crear matriz de rotación" << endl;
    cout << "   Luego warpAffine() para aplicar la transformación" << endl;
    
    Point2f centro(imagen_original.cols / 2.0, imagen_original.rows / 2.0);
    double angulo = 45.0;  // grados
    double escala = 1.0;
    Mat matriz_rotacion = getRotationMatrix2D(centro, angulo, escala);
    Mat imagen_rotada;
    warpAffine(imagen_original, imagen_rotada, matriz_rotacion, imagen_original.size());
    
    cout << "✓ Imagen rotada 45° alrededor del centro:" << endl;
    mostrarImagen("4c. Rotación 45°", imagen_rotada, false);
    
    // Escalado
    cout << "\n📏 ESCALADO/REDIMENSIONAMIENTO:" << endl;
    cout << "   resize() cambia el tamaño de la imagen" << endl;
    cout << "   Métodos de interpolación: INTER_LINEAR, INTER_CUBIC, INTER_NEAREST" << endl;
    
    Mat imagen_pequena;
    resize(imagen_original, imagen_pequena, Size(), 0.5, 0.5, INTER_LINEAR);
    
    cout << "✓ Imagen reducida al 50% (interpolación lineal):" << endl;
    cout << "   Tamaño original: " << imagen_original.size() << endl;
    cout << "   Tamaño nuevo:    " << imagen_pequena.size() << endl;
    mostrarImagen("4d. Escalado 50%", imagen_pequena, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 5. FILTRADO Y SUAVIZADO
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("5", "FILTRADO Y SUAVIZADO (REDUCCIÓN DE RUIDO)");
    
    cout << "\n🔧 Los filtros suavizan la imagen y reducen el ruido:" << endl;
    
    // Filtro Gaussiano
    cout << "\n• FILTRO GAUSSIANO:" << endl;
    cout << "  Usa una función gaussiana para promediar píxeles vecinos" << endl;
    cout << "  Preserva mejor los bordes que el promedio simple" << endl;
    cout << "  Parámetros: kernel size (debe ser impar) y sigma (desviación)" << endl;
    
    Mat imagen_gaussiano;
    GaussianBlur(imagen_original, imagen_gaussiano, Size(15, 15), 0);
    cout << "  ✓ Aplicado con kernel 15x15" << endl;
    mostrarImagen("5a. Filtro Gaussiano", imagen_gaussiano, false);
    
    // Filtro de Mediana
    cout << "\n• FILTRO DE MEDIANA:" << endl;
    cout << "  Reemplaza cada píxel por la mediana de sus vecinos" << endl;
    cout << "  Excelente para eliminar ruido 'salt and pepper'" << endl;
    cout << "  Preserva muy bien los bordes" << endl;
    
    Mat imagen_mediana;
    medianBlur(imagen_original, imagen_mediana, 9);
    cout << "  ✓ Aplicado con kernel 9x9" << endl;
    mostrarImagen("5b. Filtro de Mediana", imagen_mediana, false);
    
    // Filtro Bilateral
    cout << "\n• FILTRO BILATERAL:" << endl;
    cout << "  Suaviza pero preserva bordes" << endl;
    cout << "  Considera tanto la distancia espacial como la diferencia de intensidad" << endl;
    cout << "  Ideal para reducir ruido manteniendo detalles" << endl;
    
    Mat imagen_bilateral;
    bilateralFilter(imagen_original, imagen_bilateral, 9, 75, 75);
    cout << "  ✓ Aplicado con d=9, sigmaColor=75, sigmaSpace=75" << endl;
    mostrarImagen("5c. Filtro Bilateral", imagen_bilateral, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 6. DETECCIÓN DE BORDES
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("6", "DETECCIÓN DE BORDES");
    
    cout << "\n🔍 Los detectores de bordes identifican cambios bruscos de intensidad:" << endl;
    
    // Aplicar Gaussian blur primero (recomendado antes de detectar bordes)
    Mat imagen_suavizada;
    GaussianBlur(imagen_gris, imagen_suavizada, Size(5, 5), 0);
    
    // Sobel
    cout << "\n• OPERADOR SOBEL:" << endl;
    cout << "  Calcula gradientes en direcciones X e Y" << endl;
    cout << "  Magnitud del gradiente indica la fuerza del borde" << endl;
    
    Mat sobel_x, sobel_y, sobel_magnitude;
    Sobel(imagen_suavizada, sobel_x, CV_16S, 1, 0, 3);  // Derivada en X
    Sobel(imagen_suavizada, sobel_y, CV_16S, 0, 1, 3);  // Derivada en Y
    
    // Convertir a valores absolutos y combinar
    Mat abs_sobel_x, abs_sobel_y;
    convertScaleAbs(sobel_x, abs_sobel_x);
    convertScaleAbs(sobel_y, abs_sobel_y);
    addWeighted(abs_sobel_x, 0.5, abs_sobel_y, 0.5, 0, sobel_magnitude);
    
    cout << "  ✓ Sobel X + Sobel Y = Magnitud del gradiente" << endl;
    mostrarImagen("6a. Detección Sobel", sobel_magnitude, false);
    
    // Laplaciano
    cout << "\n• OPERADOR LAPLACIANO:" << endl;
    cout << "  Segunda derivada, detecta regiones de cambio rápido" << endl;
    cout << "  Sensible al ruido, por eso se suaviza primero" << endl;
    
    Mat laplaciano;
    Laplacian(imagen_suavizada, laplaciano, CV_16S, 3);
    convertScaleAbs(laplaciano, laplaciano);
    
    cout << "  ✓ Aplicado con kernel 3x3" << endl;
    mostrarImagen("6b. Detección Laplaciano", laplaciano, false);
    
    // Canny
    cout << "\n• DETECTOR CANNY:" << endl;
    cout << "  Algoritmo multi-etapa: suavizado → gradiente → supresión → histéresis" << endl;
    cout << "  Produce bordes delgados y bien definidos" << endl;
    cout << "  Parámetros: umbral bajo y alto (ratio típico 1:2 o 1:3)" << endl;
    
    Mat bordes_canny;
    Canny(imagen_suavizada, bordes_canny, 50, 150);
    
    cout << "  ✓ Aplicado con umbral bajo=50, alto=150" << endl;
    cout << "  → Canny es el detector más usado por su calidad" << endl;
    mostrarImagen("6c. Detección Canny", bordes_canny, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 7. TRANSFORMACIONES MORFOLÓGICAS
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("7", "TRANSFORMACIONES MORFOLÓGICAS");
    
    cout << "\n🔨 Operaciones morfológicas procesan formas en imágenes binarias:" << endl;
    cout << "   Trabajan con un 'elemento estructurante' (kernel)" << endl;
    
    // Primero necesitamos una imagen binaria
    Mat imagen_binaria;
    threshold(imagen_gris, imagen_binaria, 127, 255, THRESH_BINARY);
    mostrarImagen("7. Imagen Binaria Base", imagen_binaria, false);
    
    // Crear elemento estructurante
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
    
    // Erosión
    cout << "\n• EROSIÓN:" << endl;
    cout << "  Reduce el tamaño de objetos blancos" << endl;
    cout << "  Elimina píxeles en los bordes de los objetos" << endl;
    cout << "  Útil para: eliminar ruido pequeño, separar objetos conectados" << endl;
    
    Mat imagen_erosion;
    erode(imagen_binaria, imagen_erosion, kernel);
    cout << "  ✓ Aplicada con kernel rectangular 5x5" << endl;
    mostrarImagen("7a. Erosión", imagen_erosion, false);
    
    // Dilatación
    cout << "\n• DILATACIÓN:" << endl;
    cout << "  Aumenta el tamaño de objetos blancos" << endl;
    cout << "  Añade píxeles en los bordes de los objetos" << endl;
    cout << "  Útil para: rellenar huecos, unir objetos cercanos" << endl;
    
    Mat imagen_dilatacion;
    dilate(imagen_binaria, imagen_dilatacion, kernel);
    cout << "  ✓ Aplicada con kernel rectangular 5x5" << endl;
    mostrarImagen("7b. Dilatación", imagen_dilatacion, false);
    
    // Apertura (Opening)
    cout << "\n• APERTURA (Opening = Erosión + Dilatación):" << endl;
    cout << "  Elimina ruido pequeño manteniendo objetos grandes" << endl;
    cout << "  Suaviza contornos externos" << endl;
    
    Mat imagen_apertura;
    morphologyEx(imagen_binaria, imagen_apertura, MORPH_OPEN, kernel);
    cout << "  ✓ Aplicada (erosión seguida de dilatación)" << endl;
    mostrarImagen("7c. Apertura", imagen_apertura, false);
    
    // Cierre (Closing)
    cout << "\n• CIERRE (Closing = Dilatación + Erosión):" << endl;
    cout << "  Rellena pequeños huecos en objetos" << endl;
    cout << "  Une objetos cercanos" << endl;
    
    Mat imagen_cierre;
    morphologyEx(imagen_binaria, imagen_cierre, MORPH_CLOSE, kernel);
    cout << "  ✓ Aplicada (dilatación seguida de erosión)" << endl;
    mostrarImagen("7d. Cierre", imagen_cierre, false);
    
    // Gradiente morfológico
    cout << "\n• GRADIENTE MORFOLÓGICO (Dilatación - Erosión):" << endl;
    cout << "  Resalta los contornos de los objetos" << endl;
    
    Mat imagen_gradiente;
    morphologyEx(imagen_binaria, imagen_gradiente, MORPH_GRADIENT, kernel);
    cout << "  ✓ Aplicado (diferencia entre dilatación y erosión)" << endl;
    mostrarImagen("7e. Gradiente Morfológico", imagen_gradiente, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 8. UMBRALIZACIÓN Y BINARIZACIÓN
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("8", "UMBRALIZACIÓN Y BINARIZACIÓN");
    
    cout << "\n📊 La umbralización convierte imágenes en escala de grises a binarias:" << endl;
    cout << "   Píxeles > umbral → blanco (255)" << endl;
    cout << "   Píxeles ≤ umbral → negro (0)" << endl;
    
    // Umbralización simple
    cout << "\n• UMBRALIZACIÓN SIMPLE:" << endl;
    cout << "  Usa un valor de umbral fijo para toda la imagen" << endl;
    
    Mat umbral_simple;
    threshold(imagen_gris, umbral_simple, 127, 255, THRESH_BINARY);
    cout << "  ✓ Umbral fijo = 127" << endl;
    mostrarImagen("8a. Umbralización Simple", umbral_simple, false);
    
    // Umbralización inversa
    Mat umbral_inverso;
    threshold(imagen_gris, umbral_inverso, 127, 255, THRESH_BINARY_INV);
    cout << "\n• UMBRALIZACIÓN INVERSA:" << endl;
    cout << "  Invierte el resultado (fondo blanco, objetos negros)" << endl;
    mostrarImagen("8b. Umbralización Inversa", umbral_inverso, false);
    
    // Otsu
    cout << "\n• MÉTODO DE OTSU:" << endl;
    cout << "  Calcula automáticamente el umbral óptimo" << endl;
    cout << "  Maximiza la varianza entre clases" << endl;
    cout << "  Ideal cuando no conoces el mejor umbral" << endl;
    
    Mat umbral_otsu;
    double umbral_calculado = threshold(imagen_gris, umbral_otsu, 0, 255, THRESH_BINARY + THRESH_OTSU);
    cout << "  ✓ Umbral calculado automáticamente: " << umbral_calculado << endl;
    mostrarImagen("8c. Método de Otsu", umbral_otsu, false);
    
    // Umbralización adaptativa
    cout << "\n• UMBRALIZACIÓN ADAPTATIVA:" << endl;
    cout << "  Calcula un umbral diferente para cada región de la imagen" << endl;
    cout << "  Útil con iluminación no uniforme" << endl;
    cout << "  Métodos: ADAPTIVE_THRESH_MEAN_C o ADAPTIVE_THRESH_GAUSSIAN_C" << endl;
    
    Mat umbral_adaptativo;
    adaptiveThreshold(imagen_gris, umbral_adaptativo, 255, 
                     ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);
    cout << "  ✓ Método gaussiano, tamaño de bloque=11, constante=2" << endl;
    mostrarImagen("8d. Umbralización Adaptativa", umbral_adaptativo, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 9. HISTOGRAMAS Y ECUALIZACIÓN
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("9", "HISTOGRAMAS Y ECUALIZACIÓN");
    
    cout << "\n📈 El histograma muestra la distribución de intensidades de píxeles:" << endl;
    cout << "   Eje X: Intensidad (0-255)" << endl;
    cout << "   Eje Y: Número de píxeles con esa intensidad" << endl;
    
    // Calcular histograma
    cout << "\n• CÁLCULO DEL HISTOGRAMA:" << endl;
    Mat histograma;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    calcHist(&imagen_gris, 1, 0, Mat(), histograma, 1, &histSize, &histRange);
    
    // Dibujar histograma
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    
    // Normalizar para que quepa en la imagen
    normalize(histograma, histograma, 0, histImage.rows, NORM_MINMAX);
    
    // Dibujar líneas del histograma
    for (int i = 1; i < histSize; i++) {
        line(histImage, 
             Point(bin_w * (i - 1), hist_h - cvRound(histograma.at<float>(i - 1))),
             Point(bin_w * i, hist_h - cvRound(histograma.at<float>(i))),
             Scalar(255, 255, 255), 2);
    }
    
    cout << "  ✓ Histograma calculado y visualizado" << endl;
    cout << "  Interpretación:" << endl;
    cout << "    - Picos a la izquierda: imagen oscura" << endl;
    cout << "    - Picos a la derecha: imagen clara" << endl;
    cout << "    - Distribución estrecha: poco contraste" << endl;
    cout << "    - Distribución amplia: mucho contraste" << endl;
    mostrarImagen("9a. Histograma Original", histImage, false);
    mostrarImagen("9a-img. Imagen Original (gris)", imagen_gris, false);
    
    // Ecualización de histograma
    cout << "\n• ECUALIZACIÓN DE HISTOGRAMA:" << endl;
    cout << "  Redistribuye las intensidades para mejorar el contraste" << endl;
    cout << "  Estira el histograma para usar todo el rango 0-255" << endl;
    
    Mat imagen_ecualizada;
    equalizeHist(imagen_gris, imagen_ecualizada);
    
    // Calcular histograma de la imagen ecualizada
    Mat histograma_eq;
    calcHist(&imagen_ecualizada, 1, 0, Mat(), histograma_eq, 1, &histSize, &histRange);
    Mat histImage_eq(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(histograma_eq, histograma_eq, 0, histImage_eq.rows, NORM_MINMAX);
    
    for (int i = 1; i < histSize; i++) {
        line(histImage_eq,
             Point(bin_w * (i - 1), hist_h - cvRound(histograma_eq.at<float>(i - 1))),
             Point(bin_w * i, hist_h - cvRound(histograma_eq.at<float>(i))),
             Scalar(255, 255, 255), 2);
    }
    
    cout << "  ✓ Ecualización aplicada" << endl;
    cout << "  → El histograma ahora está más distribuido" << endl;
    mostrarImagen("9b. Histograma Ecualizado", histImage_eq, false);
    mostrarImagen("9b-img. Imagen Ecualizada", imagen_ecualizada, false);
    
    // CLAHE
    cout << "\n• CLAHE (Contrast Limited Adaptive Histogram Equalization):" << endl;
    cout << "  Ecualización adaptativa que divide la imagen en regiones" << endl;
    cout << "  Limita la amplificación del contraste para evitar sobre-realce" << endl;
    cout << "  Mejor que ecualización global para imágenes con iluminación variable" << endl;
    
    Ptr<CLAHE> clahe = createCLAHE();
    clahe->setClipLimit(2.0);
    clahe->setTilesGridSize(Size(8, 8));
    
    Mat imagen_clahe;
    clahe->apply(imagen_gris, imagen_clahe);
    
    cout << "  ✓ CLAHE aplicado con clipLimit=2.0, tiles=8x8" << endl;
    cout << "  → Mejor para imágenes médicas, visión nocturna, etc." << endl;
    mostrarImagen("9c. CLAHE", imagen_clahe, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 10. DETECCIÓN DE CONTORNOS
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("10", "DETECCIÓN DE CONTORNOS Y FORMAS");
    
    cout << "\n🔍 Los contornos son curvas que unen puntos en el borde de objetos:" << endl;
    
    // Detectar bordes con Canny
    Mat bordes_para_contornos;
    Canny(imagen_gris, bordes_para_contornos, 50, 150);
    
    // Encontrar contornos
    cout << "\n• DETECCIÓN DE CONTORNOS:" << endl;
    cout << "  findContours() busca curvas continuas en imagen binaria" << endl;
    cout << "  Modos de recuperación:" << endl;
    cout << "    - RETR_EXTERNAL: solo contornos externos" << endl;
    cout << "    - RETR_LIST: todos los contornos sin jerarquía" << endl;
    cout << "    - RETR_TREE: jerarquía completa (contornos dentro de contornos)" << endl;
    cout << "  Métodos de aproximación:" << endl;
    cout << "    - CHAIN_APPROX_NONE: guarda todos los puntos" << endl;
    cout << "    - CHAIN_APPROX_SIMPLE: comprime segmentos (ahorra memoria)" << endl;
    
    vector<vector<Point>> contornos;
    vector<Vec4i> jerarquia;
    findContours(bordes_para_contornos, contornos, jerarquia, RETR_TREE, CHAIN_APPROX_SIMPLE);
    
    cout << "\n  ✓ Contornos detectados: " << contornos.size() << endl;
    
    // Dibujar contornos
    Mat imagen_contornos = imagen_original.clone();
    drawContours(imagen_contornos, contornos, -1, Scalar(0, 255, 0), 2);
    
    cout << "  ✓ Contornos dibujados en verde" << endl;
    mostrarImagen("10a. Contornos Detectados", imagen_contornos, false);
    
    // Análisis de contornos
    cout << "\n• ANÁLISIS DE CONTORNOS:" << endl;
    cout << "  Podemos calcular propiedades geométricas:" << endl;
    
    Mat imagen_analisis = imagen_original.clone();
    
    // Analizar los 5 contornos más grandes
    vector<pair<double, int>> areas_indices;
    for (size_t i = 0; i < contornos.size(); i++) {
        double area = contourArea(contornos[i]);
        if (area > 100) {  // Filtrar contornos muy pequeños
            areas_indices.push_back(make_pair(area, i));
        }
    }
    
    // Ordenar por área (mayor a menor)
    sort(areas_indices.begin(), areas_indices.end(), greater<pair<double, int>>());
    
    int num_analizar = min(5, (int)areas_indices.size());
    cout << "\n  Analizando los " << num_analizar << " contornos más grandes:" << endl;
    
    for (int i = 0; i < num_analizar; i++) {
        int idx = areas_indices[i].second;
        
        // Área
        double area = contourArea(contornos[idx]);
        
        // Perímetro
        double perimetro = arcLength(contornos[idx], true);
        
        // Rectángulo delimitador
        Rect rect = boundingRect(contornos[idx]);
        
        // Círculo delimitador
        Point2f centro;
        float radio;
        minEnclosingCircle(contornos[idx], centro, radio);
        
        // Aproximación poligonal
        vector<Point> poligono;
        approxPolyDP(contornos[idx], poligono, 0.02 * perimetro, true);
        
        cout << "\n  Contorno " << (i + 1) << ":" << endl;
        cout << "    • Área: " << (int)area << " píxeles²" << endl;
        cout << "    • Perímetro: " << (int)perimetro << " píxeles" << endl;
        cout << "    • Vértices (aproximado): " << poligono.size() << endl;
        cout << "    • Forma detectada: ";
        
        // Clasificación simple por número de vértices
        if (poligono.size() == 3) cout << "Triángulo";
        else if (poligono.size() == 4) {
            double aspecto = (double)rect.width / rect.height;
            if (aspecto >= 0.95 && aspecto <= 1.05) cout << "Cuadrado";
            else cout << "Rectángulo";
        }
        else if (poligono.size() > 6) cout << "Círculo/Elipse";
        else cout << "Polígono de " << poligono.size() << " lados";
        cout << endl;
        
        // Dibujar análisis
        drawContours(imagen_analisis, contornos, idx, Scalar(0, 255, 0), 2);
        rectangle(imagen_analisis, rect, Scalar(255, 0, 0), 2);
        circle(imagen_analisis, centro, (int)radio, Scalar(0, 0, 255), 2);
    }
    
    cout << "\n  Visualización:" << endl;
    cout << "    • Verde: contorno original" << endl;
    cout << "    • Azul: rectángulo delimitador" << endl;
    cout << "    • Rojo: círculo delimitador" << endl;
    
    mostrarImagen("10b. Análisis de Contornos", imagen_analisis, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 11. OPERACIONES LÓGICAS ENTRE IMÁGENES
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("11", "OPERACIONES LÓGICAS ENTRE IMÁGENES");
    
    cout << "\n🔣 Las operaciones lógicas combinan imágenes bit a bit:" << endl;
    
    // Crear dos máscaras de ejemplo
    Mat mascara1 = Mat::zeros(imagen_gris.size(), CV_8UC1);
    circle(mascara1, Point(imagen_gris.cols / 3, imagen_gris.rows / 2), 100, Scalar(255), -1);
    
    Mat mascara2 = Mat::zeros(imagen_gris.size(), CV_8UC1);
    circle(mascara2, Point(2 * imagen_gris.cols / 3, imagen_gris.rows / 2), 100, Scalar(255), -1);
    
    mostrarImagen("11. Máscara 1", mascara1, false);
    mostrarImagen("11. Máscara 2", mascara2, false);
    
    // AND
    cout << "\n• OPERACIÓN AND (bitwise_and):" << endl;
    cout << "  Resultado = 1 solo si AMBAS imágenes tienen 1" << endl;
    cout << "  Útil para: intersección, aplicar máscaras" << endl;
    
    Mat resultado_and;
    bitwise_and(mascara1, mascara2, resultado_and);
    cout << "  ✓ Muestra solo la región de intersección" << endl;
    mostrarImagen("11a. AND", resultado_and, false);
    
    // OR
    cout << "\n• OPERACIÓN OR (bitwise_or):" << endl;
    cout << "  Resultado = 1 si AL MENOS UNA imagen tiene 1" << endl;
    cout << "  Útil para: unión de regiones" << endl;
    
    Mat resultado_or;
    bitwise_or(mascara1, mascara2, resultado_or);
    cout << "  ✓ Muestra ambas regiones combinadas" << endl;
    mostrarImagen("11b. OR", resultado_or, false);
    
    // XOR
    cout << "\n• OPERACIÓN XOR (bitwise_xor):" << endl;
    cout << "  Resultado = 1 si las imágenes son DIFERENTES" << endl;
    cout << "  Útil para: detectar diferencias entre imágenes" << endl;
    
    Mat resultado_xor;
    bitwise_xor(mascara1, mascara2, resultado_xor);
    cout << "  ✓ Muestra solo las regiones no compartidas" << endl;
    mostrarImagen("11c. XOR", resultado_xor, false);
    
    // NOT
    cout << "\n• OPERACIÓN NOT (bitwise_not):" << endl;
    cout << "  Invierte todos los bits (negativo)" << endl;
    cout << "  0 → 255, 255 → 0" << endl;
    
    Mat resultado_not;
    bitwise_not(mascara1, resultado_not);
    cout << "  ✓ Invierte la máscara 1" << endl;
    mostrarImagen("11d. NOT", resultado_not, false);
    
    // Aplicación práctica: enmascarar imagen
    cout << "\n• APLICACIÓN: ENMASCARAR IMAGEN" << endl;
    cout << "  Usar AND para extraer solo una región de la imagen" << endl;
    
    Mat imagen_enmascarada;
    bitwise_and(imagen_gris, imagen_gris, imagen_enmascarada, mascara1);
    cout << "  ✓ Solo se muestra la región dentro del círculo" << endl;
    mostrarImagen("11e. Imagen Enmascarada", imagen_enmascarada, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 12. SEGMENTACIÓN POR COLOR
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("12", "SEGMENTACIÓN POR COLOR");
    
    cout << "\n🎨 La segmentación por color extrae objetos según su color:" << endl;
    cout << "   Es más efectiva en espacio HSV que BGR/RGB" << endl;
    
    cout << "\n• PROCESO DE SEGMENTACIÓN:" << endl;
    cout << "  1. Convertir imagen a HSV" << endl;
    cout << "  2. Definir rango de color deseado (min-max)" << endl;
    cout << "  3. Crear máscara con inRange()" << endl;
    cout << "  4. Aplicar máscara a imagen original" << endl;
    
    // Convertir a HSV
    Mat imagen_hsv_seg;
    cvtColor(imagen_original, imagen_hsv_seg, COLOR_BGR2HSV);
    
    // Definir rangos de color (ejemplo: detectar tonos rojos)
    // NOTA: En OpenCV, el rango H es 0-180 (no 0-360)
    cout << "\n  Ejemplo: Segmentar tonos ROJOS" << endl;
    cout << "  Rango H (Hue): 0-10 y 170-180 (rojo está en ambos extremos)" << endl;
    cout << "  Rango S (Saturation): 100-255 (saturación media-alta)" << endl;
    cout << "  Rango V (Value): 100-255 (brillo medio-alto)" << endl;
    
    // El rojo está en ambos extremos del espectro H
    Mat mascara_rojo1, mascara_rojo2, mascara_rojo;
    inRange(imagen_hsv_seg, Scalar(0, 100, 100), Scalar(10, 255, 255), mascara_rojo1);
    inRange(imagen_hsv_seg, Scalar(170, 100, 100), Scalar(180, 255, 255), mascara_rojo2);
    bitwise_or(mascara_rojo1, mascara_rojo2, mascara_rojo);
    
    cout << "\n  ✓ Máscara creada con inRange()" << endl;
    mostrarImagen("12a. Máscara Rojo", mascara_rojo, false);
    
    // Aplicar operaciones morfológicas para limpiar ruido
    cout << "\n  Limpiando máscara con morfología:" << endl;
    Mat kernel_limpieza = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    morphologyEx(mascara_rojo, mascara_rojo, MORPH_OPEN, kernel_limpieza);
    morphologyEx(mascara_rojo, mascara_rojo, MORPH_CLOSE, kernel_limpieza);
    cout << "  ✓ Apertura + Cierre aplicados" << endl;
    mostrarImagen("12b. Máscara Limpia", mascara_rojo, false);
    
    // Aplicar máscara a imagen original
    Mat resultado_segmentacion;
    bitwise_and(imagen_original, imagen_original, resultado_segmentacion, mascara_rojo);
    
    cout << "\n  ✓ Máscara aplicada a imagen original" << endl;
    cout << "  → Solo se muestran los objetos rojos detectados" << endl;
    mostrarImagen("12c. Segmentación por Color (Rojos)", resultado_segmentacion, false);
    
    // Ejemplo adicional: segmentar otro color (verde)
    cout << "\n  Ejemplo adicional: Segmentar tonos VERDES" << endl;
    cout << "  Rango H: 40-80 (verde)" << endl;
    
    Mat mascara_verde;
    inRange(imagen_hsv_seg, Scalar(40, 50, 50), Scalar(80, 255, 255), mascara_verde);
    morphologyEx(mascara_verde, mascara_verde, MORPH_OPEN, kernel_limpieza);
    
    Mat resultado_verde;
    bitwise_and(imagen_original, imagen_original, resultado_verde, mascara_verde);
    
    cout << "  ✓ Segmentación de verdes completada" << endl;
    mostrarImagen("12d. Segmentación por Color (Verdes)", resultado_verde, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // 13. MANIPULACIÓN DIRECTA DE PÍXELES
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeccion("13", "MANIPULACIÓN DIRECTA DE PÍXELES - NIVEL AVANZADO");
    
    cout << "\n🎯 En esta sección aprenderás a manipular píxeles directamente:" << endl;
    cout << "   • Acceder a píxeles individuales con .at<>()" << endl;
    cout << "   • Recorrer la imagen con bucles for" << endl;
    cout << "   • Aplicar transformaciones personalizadas" << endl;
    cout << "   • Combinar imágenes píxel por píxel" << endl;
    
    cout << "\n📝 CONCEPTOS BÁSICOS:" << endl;
    cout << "   • Mat.at<tipo>(fila, columna) - Acceso a píxel individual" << endl;
    cout << "   • Para escala de grises: at<uchar>(y, x)" << endl;
    cout << "   • Para color (BGR): at<Vec3b>(y, x)" << endl;
    cout << "   • Vec3b es un vector de 3 bytes: [B, G, R]" << endl;
    cout << "   • Coordenadas: (0,0) está en la esquina superior izquierda" << endl;
    
    // Crear una copia para manipulaciones
    Mat imagen_para_manipular = imagen_original.clone();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 1: MANIPULAR SOLO LA MITAD DE LA IMAGEN
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 1: MODIFICAR SOLO LA MITAD IZQUIERDA DE LA IMAGEN" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Convertir la mitad izquierda a escala de grises" << endl;
    cout << "   Técnica: Recorrer solo las columnas de 0 a cols/2" << endl;
    cout << "   Fórmula de gris: Gray = 0.299*R + 0.587*G + 0.114*B" << endl;
    
    Mat mitad_modificada = imagen_original.clone();
    int mitad_ancho = mitad_modificada.cols / 2;
    
    cout << "\n⚙️ Procesando..." << endl;
    for (int y = 0; y < mitad_modificada.rows; y++) {
        for (int x = 0; x < mitad_ancho; x++) {
            // Obtener el píxel BGR
            Vec3b pixel = mitad_modificada.at<Vec3b>(y, x);
            
            // Calcular valor en escala de grises
            uchar gris = static_cast<uchar>(
                0.299 * pixel[2] +  // R (Red)
                0.587 * pixel[1] +  // G (Green)
                0.114 * pixel[0]    // B (Blue)
            );
            
            // Asignar el mismo valor a los 3 canales
            mitad_modificada.at<Vec3b>(y, x) = Vec3b(gris, gris, gris);
        }
    }
    
    cout << "✓ Mitad izquierda convertida a escala de grises" << endl;
    cout << "  Píxeles procesados: " << mitad_modificada.rows * mitad_ancho << endl;
    mostrarImagen("13.1 Mitad Izquierda en Gris", mitad_modificada, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 2: MODIFICAR DIAGONAL PRINCIPAL
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 2: DIBUJAR LÍNEA ROJA EN LA DIAGONAL" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Dibujar una línea roja gruesa en la diagonal principal" << endl;
    cout << "   Técnica: Cuando fila == columna (ajustado por proporción)" << endl;
    cout << "   Color: Rojo puro en BGR = (0, 0, 255)" << endl;
    
    Mat diagonal_modificada = imagen_original.clone();
    int grosor = 5;  // Grosor de la línea
    
    cout << "\n⚙️ Dibujando diagonal..." << endl;
    for (int y = 0; y < diagonal_modificada.rows; y++) {
        // Calcular x proporcional (para imágenes no cuadradas)
        int x_diagonal = (y * diagonal_modificada.cols) / diagonal_modificada.rows;
        
        // Dibujar línea gruesa
        for (int offset = -grosor; offset <= grosor; offset++) {
            int x = x_diagonal + offset;
            if (x >= 0 && x < diagonal_modificada.cols) {
                // Rojo puro: B=0, G=0, R=255
                diagonal_modificada.at<Vec3b>(y, x) = Vec3b(0, 0, 255);
            }
        }
    }
    
    cout << "✓ Diagonal dibujada en rojo" << endl;
    cout << "  Grosor de línea: " << (grosor * 2 + 1) << " píxeles" << endl;
    mostrarImagen("13.2 Diagonal Roja", diagonal_modificada, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 3: UNIR DOS IMÁGENES COMPLEMENTARIAS
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 3: COMBINAR IMÁGENES COMPLEMENTARIAS (PATRÓN RAYAS)" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Crear dos imágenes con rayas complementarias y unirlas" << endl;
    cout << "   Imagen A: Rayas horizontales pares (0, 2, 4, ...) tienen contenido" << endl;
    cout << "   Imagen B: Rayas horizontales impares (1, 3, 5, ...) tienen contenido" << endl;
    cout << "   Resultado: Imagen completa sin rayas negras" << endl;
    
    // Crear imagen A (rayas pares)
    Mat imagen_a = Mat::zeros(imagen_original.size(), imagen_original.type());
    cout << "\n⚙️ Creando imagen A (rayas pares)..." << endl;
    for (int y = 0; y < imagen_original.rows; y++) {
        if (y % 2 == 0) {  // Filas pares
            for (int x = 0; x < imagen_original.cols; x++) {
                imagen_a.at<Vec3b>(y, x) = imagen_original.at<Vec3b>(y, x);
            }
        }
        // Filas impares quedan negras (inicializadas con zeros)
    }
    
    // Crear imagen B (rayas impares)
    Mat imagen_b = Mat::zeros(imagen_original.size(), imagen_original.type());
    cout << "⚙️ Creando imagen B (rayas impares)..." << endl;
    for (int y = 0; y < imagen_original.rows; y++) {
        if (y % 2 == 1) {  // Filas impares
            for (int x = 0; x < imagen_original.cols; x++) {
                imagen_b.at<Vec3b>(y, x) = imagen_original.at<Vec3b>(y, x);
            }
        }
        // Filas pares quedan negras
    }
    
    mostrarImagen("13.3a Imagen A (rayas pares)", imagen_a, false);
    mostrarImagen("13.3b Imagen B (rayas impares)", imagen_b, false);
    
    // Combinar las imágenes
    cout << "\n⚙️ Combinando imágenes A y B..." << endl;
    Mat imagen_combinada = Mat::zeros(imagen_original.size(), imagen_original.type());
    
    for (int y = 0; y < imagen_original.rows; y++) {
        for (int x = 0; x < imagen_original.cols; x++) {
            Vec3b pixel_a = imagen_a.at<Vec3b>(y, x);
            Vec3b pixel_b = imagen_b.at<Vec3b>(y, x);
            
            // Si el píxel de A no es negro, usar A, sino usar B
            if (pixel_a[0] != 0 || pixel_a[1] != 0 || pixel_a[2] != 0) {
                imagen_combinada.at<Vec3b>(y, x) = pixel_a;
            } else {
                imagen_combinada.at<Vec3b>(y, x) = pixel_b;
            }
        }
    }
    
    cout << "✓ Imágenes combinadas exitosamente" << endl;
    cout << "  Resultado: Imagen completa sin rayas negras" << endl;
    mostrarImagen("13.3c Imagen Combinada", imagen_combinada, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 4: PROMEDIO DE CANALES (PARTE SUPERIOR)
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 4: PROMEDIO DE CANALES (CONVERSIÓN A GRIS MANUAL)" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Convertir la mitad superior a gris usando promedio simple" << endl;
    cout << "   Técnica: Promedio = (B + G + R) / 3" << endl;
    cout << "   Área: Filas de 0 a rows/2" << endl;
    cout << "   Nota: Más simple que la fórmula ponderada estándar" << endl;
    
    Mat promedio_canales = imagen_original.clone();
    int mitad_alto = promedio_canales.rows / 2;
    
    cout << "\n⚙️ Aplicando promedio de canales en parte superior..." << endl;
    for (int y = 0; y < mitad_alto; y++) {
        for (int x = 0; x < promedio_canales.cols; x++) {
            Vec3b pixel = promedio_canales.at<Vec3b>(y, x);
            
            // Promedio simple de los tres canales
            uchar promedio = (pixel[0] + pixel[1] + pixel[2]) / 3;
            
            // Asignar el promedio a los tres canales
            promedio_canales.at<Vec3b>(y, x) = Vec3b(promedio, promedio, promedio);
        }
    }
    
    cout << "✓ Parte superior convertida a gris por promedio" << endl;
    cout << "  Píxeles procesados: " << mitad_alto * promedio_canales.cols << endl;
    mostrarImagen("13.4 Promedio de Canales", promedio_canales, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 5: MÁXIMO DE CANALES
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 5: MÁXIMO DE CANALES (EFECTO ARTÍSTICO)" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Tomar el valor máximo entre los 3 canales" << endl;
    cout << "   Técnica: Nuevo_valor = max(B, G, R)" << endl;
    cout << "   Efecto: Aumenta el brillo y resalta colores dominantes" << endl;
    cout << "   Aplicación: Toda la imagen" << endl;
    
    Mat maximo_canales = imagen_original.clone();
    
    cout << "\n⚙️ Aplicando máximo de canales..." << endl;
    for (int y = 0; y < maximo_canales.rows; y++) {
        for (int x = 0; x < maximo_canales.cols; x++) {
            Vec3b pixel = maximo_canales.at<Vec3b>(y, x);
            
            // Encontrar el valor máximo entre los tres canales
            uchar maximo = max({pixel[0], pixel[1], pixel[2]});
            
            // Asignar el máximo a los tres canales
            maximo_canales.at<Vec3b>(y, x) = Vec3b(maximo, maximo, maximo);
        }
    }
    
    cout << "✓ Efecto de máximo aplicado" << endl;
    cout << "  Resultado: Imagen con valores máximos de cada píxel" << endl;
    mostrarImagen("13.5 Máximo de Canales", maximo_canales, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 6: NEGACIÓN DE CANALES
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 6: NEGACIÓN/INVERSIÓN DE CANALES (NEGATIVO)" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Crear el negativo de la imagen" << endl;
    cout << "   Técnica: Nuevo_valor = 255 - Valor_original" << endl;
    cout << "   Efecto: Invierte todos los colores (como negativo fotográfico)" << endl;
    cout << "   Fórmula: Se aplica a cada canal independientemente" << endl;
    
    Mat negacion_canales = imagen_original.clone();
    
    cout << "\n⚙️ Aplicando negación a cada canal..." << endl;
    for (int y = 0; y < negacion_canales.rows; y++) {
        for (int x = 0; x < negacion_canales.cols; x++) {
            Vec3b pixel = negacion_canales.at<Vec3b>(y, x);
            
            // Invertir cada canal: 255 - valor
            negacion_canales.at<Vec3b>(y, x) = Vec3b(
                255 - pixel[0],  // B invertido
                255 - pixel[1],  // G invertido
                255 - pixel[2]   // R invertido
            );
        }
    }
    
    cout << "✓ Negativo aplicado" << endl;
    cout << "  Resultado: Colores invertidos (negativo fotográfico)" << endl;
    mostrarImagen("13.6 Negación de Canales", negacion_canales, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 7: INTERCAMBIAR CANALES
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 7: INTERCAMBIAR CANALES DE COLOR" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Intercambiar los canales BGR" << endl;
    cout << "   Original: BGR (Blue, Green, Red)" << endl;
    cout << "   Resultado: RGB (Red, Green, Blue)" << endl;
    cout << "   Efecto: Cambio drástico de colores" << endl;
    
    Mat canales_intercambiados = imagen_original.clone();
    
    cout << "\n⚙️ Intercambiando canales B ↔ R..." << endl;
    for (int y = 0; y < canales_intercambiados.rows; y++) {
        for (int x = 0; x < canales_intercambiados.cols; x++) {
            Vec3b pixel = canales_intercambiados.at<Vec3b>(y, x);
            
            // Intercambiar B y R (mantener G igual)
            canales_intercambiados.at<Vec3b>(y, x) = Vec3b(
                pixel[2],  // R → B
                pixel[1],  // G → G
                pixel[0]   // B → R
            );
        }
    }
    
    cout << "✓ Canales intercambiados" << endl;
    cout << "  Los tonos azules ahora se ven rojos y viceversa" << endl;
    mostrarImagen("13.7 Canales Intercambiados", canales_intercambiados, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 8: AISLAR UN SOLO CANAL
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 8: AISLAR CADA CANAL DE COLOR" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Mostrar cada canal por separado" << endl;
    cout << "   Técnica: Poner los otros canales en 0" << endl;
    cout << "   Resultado: 3 imágenes mostrando B, G, R individualmente" << endl;
    
    Mat solo_azul = imagen_original.clone();
    Mat solo_verde = imagen_original.clone();
    Mat solo_rojo = imagen_original.clone();
    
    cout << "\n⚙️ Aislando canales..." << endl;
    for (int y = 0; y < imagen_original.rows; y++) {
        for (int x = 0; x < imagen_original.cols; x++) {
            Vec3b pixel = imagen_original.at<Vec3b>(y, x);
            
            // Solo canal azul (B)
            solo_azul.at<Vec3b>(y, x) = Vec3b(pixel[0], 0, 0);
            
            // Solo canal verde (G)
            solo_verde.at<Vec3b>(y, x) = Vec3b(0, pixel[1], 0);
            
            // Solo canal rojo (R)
            solo_rojo.at<Vec3b>(y, x) = Vec3b(0, 0, pixel[2]);
        }
    }
    
    cout << "✓ Canales aislados" << endl;
    mostrarImagen("13.8a Solo Canal Azul", solo_azul, false);
    mostrarImagen("13.8b Solo Canal Verde", solo_verde, false);
    mostrarImagen("13.8c Solo Canal Rojo", solo_rojo, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 9: EFECTO SEPIA (TONO VINTAGE)
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 9: EFECTO SEPIA (FOTOGRAFÍA ANTIGUA)" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Aplicar efecto sepia (tono marrón vintage)" << endl;
    cout << "   Técnica: Usar transformación de matriz de color" << endl;
    cout << "   Fórmulas:" << endl;
    cout << "     nuevo_R = 0.393*R + 0.769*G + 0.189*B" << endl;
    cout << "     nuevo_G = 0.349*R + 0.686*G + 0.168*B" << endl;
    cout << "     nuevo_B = 0.272*R + 0.534*G + 0.131*B" << endl;
    
    Mat imagen_sepia = imagen_original.clone();
    
    cout << "\n⚙️ Aplicando transformación sepia..." << endl;
    for (int y = 0; y < imagen_sepia.rows; y++) {
        for (int x = 0; x < imagen_sepia.cols; x++) {
            Vec3b pixel = imagen_sepia.at<Vec3b>(y, x);
            
            // Obtener valores BGR originales
            double b = pixel[0];
            double g = pixel[1];
            double r = pixel[2];
            
            // Aplicar transformación sepia
            int nuevo_r = min(255, (int)(0.393 * r + 0.769 * g + 0.189 * b));
            int nuevo_g = min(255, (int)(0.349 * r + 0.686 * g + 0.168 * b));
            int nuevo_b = min(255, (int)(0.272 * r + 0.534 * g + 0.131 * b));
            
            imagen_sepia.at<Vec3b>(y, x) = Vec3b(nuevo_b, nuevo_g, nuevo_r);
        }
    }
    
    cout << "✓ Efecto sepia aplicado" << endl;
    cout << "  Resultado: Imagen con tono marrón vintage" << endl;
    mostrarImagen("13.9 Efecto Sepia", imagen_sepia, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 10: ESPEJO HORIZONTAL Y VERTICAL
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 10: EFECTO ESPEJO (FLIP MANUAL)" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Voltear la imagen horizontal y verticalmente" << endl;
    cout << "   Espejo horizontal: Intercambiar columnas (x ↔ cols-1-x)" << endl;
    cout << "   Espejo vertical: Intercambiar filas (y ↔ rows-1-y)" << endl;
    
    // Espejo horizontal
    Mat espejo_horizontal = imagen_original.clone();
    cout << "\n⚙️ Aplicando espejo horizontal..." << endl;
    for (int y = 0; y < espejo_horizontal.rows; y++) {
        for (int x = 0; x < espejo_horizontal.cols / 2; x++) {
            // Intercambiar píxeles simétricos
            Vec3b temp = espejo_horizontal.at<Vec3b>(y, x);
            espejo_horizontal.at<Vec3b>(y, x) = 
                espejo_horizontal.at<Vec3b>(y, espejo_horizontal.cols - 1 - x);
            espejo_horizontal.at<Vec3b>(y, espejo_horizontal.cols - 1 - x) = temp;
        }
    }
    cout << "✓ Espejo horizontal aplicado" << endl;
    mostrarImagen("13.10a Espejo Horizontal", espejo_horizontal, false);
    
    // Espejo vertical
    Mat espejo_vertical = imagen_original.clone();
    cout << "\n⚙️ Aplicando espejo vertical..." << endl;
    for (int y = 0; y < espejo_vertical.rows / 2; y++) {
        for (int x = 0; x < espejo_vertical.cols; x++) {
            // Intercambiar píxeles simétricos
            Vec3b temp = espejo_vertical.at<Vec3b>(y, x);
            espejo_vertical.at<Vec3b>(y, x) = 
                espejo_vertical.at<Vec3b>(espejo_vertical.rows - 1 - y, x);
            espejo_vertical.at<Vec3b>(espejo_vertical.rows - 1 - y, x) = temp;
        }
    }
    cout << "✓ Espejo vertical aplicado" << endl;
    mostrarImagen("13.10b Espejo Vertical", espejo_vertical, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 11: MOSAICO DE 4 CUADRANTES
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    cout << "EJERCICIO 11: MOSAICO DE 4 EFECTOS" << endl;
    cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
    
    cout << "\n📖 Objetivo: Dividir imagen en 4 cuadrantes con diferentes efectos" << endl;
    cout << "   Superior izquierdo: Original" << endl;
    cout << "   Superior derecho: Escala de grises" << endl;
    cout << "   Inferior izquierdo: Negativo" << endl;
    cout << "   Inferior derecho: Sepia" << endl;
    
    Mat mosaico = imagen_original.clone();
    int mitad_x = mosaico.cols / 2;
    int mitad_y = mosaico.rows / 2;
    
    cout << "\n⚙️ Creando mosaico de efectos..." << endl;
    
    for (int y = 0; y < mosaico.rows; y++) {
        for (int x = 0; x < mosaico.cols; x++) {
            Vec3b pixel = imagen_original.at<Vec3b>(y, x);
            
            // Cuadrante superior izquierdo: Original (no hacer nada)
            if (y < mitad_y && x < mitad_x) {
                // Mantener original
            }
            // Cuadrante superior derecho: Escala de grises
            else if (y < mitad_y && x >= mitad_x) {
                uchar gris = (pixel[0] + pixel[1] + pixel[2]) / 3;
                mosaico.at<Vec3b>(y, x) = Vec3b(gris, gris, gris);
            }
            // Cuadrante inferior izquierdo: Negativo
            else if (y >= mitad_y && x < mitad_x) {
                mosaico.at<Vec3b>(y, x) = Vec3b(
                    255 - pixel[0],
                    255 - pixel[1],
                    255 - pixel[2]
                );
            }
            // Cuadrante inferior derecho: Sepia
            else {
                double b = pixel[0], g = pixel[1], r = pixel[2];
                int nuevo_r = min(255, (int)(0.393 * r + 0.769 * g + 0.189 * b));
                int nuevo_g = min(255, (int)(0.349 * r + 0.686 * g + 0.168 * b));
                int nuevo_b = min(255, (int)(0.272 * r + 0.534 * g + 0.131 * b));
                mosaico.at<Vec3b>(y, x) = Vec3b(nuevo_b, nuevo_g, nuevo_r);
            }
        }
    }
    
    // Dibujar líneas divisorias
    line(mosaico, Point(mitad_x, 0), Point(mitad_x, mosaico.rows), 
         Scalar(255, 255, 255), 3);
    line(mosaico, Point(0, mitad_y), Point(mosaico.cols, mitad_y), 
         Scalar(255, 255, 255), 3);
    °
    cout << "✓ Mosaico creado con 4 efectos diferentes" << endl;
    mostrarImagen("13.11 Mosaico de Efectos", mosaico, false);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // EJERCICIO 12: PIXELACIÓN (EFECTO MOSAICO)
    // ═══════════════════════════════════════════════════════════════════════════
        cout << "\n━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        cout << "EJERCICIO 12: EFECTO PIXELACIÓN (BLOQUES)" << endl;
        cout << "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" << endl;
        
        cout << "\n📖 Objetivo: Reducir la resolución creando bloques de píxeles" << endl;
        cout << "   Técnica: Agrupar píxeles en bloques y usar el color promedio" << endl;
        cout << "   Tamaño de bloque: 10x10 píxeles" << endl;
    
    Mat imagen_pixelada = imagen_original.clone();
    int tamano_bloque = 10;
    
    cout << "\n⚙️ Aplicando efecto de pixelación..." << endl;
    
    for (int y = 0; y < imagen_pixelada.rows; y += tamano_bloque) {
        for (int x = 0; x < imagen_pixelada.cols; x += tamano_bloque) {
            // Calcular el promedio del bloque
            int suma_b = 0, suma_g = 0, suma_r = 0;
            int contador = 0;
            
            // Recorrer el bloque
            for (int by = y; by < min(y + tamano_bloque, imagen_pixelada.rows); by++) {
                for (int bx = x; bx < min(x + tamano_bloque, imagen_pixelada.cols); bx++) {
                    Vec3b pixel = imagen_original.at<Vec3b>(by, bx);
                    suma_b += pixel[0];
                    suma_g += pixel[1];
                    suma_r += pixel[2];
                    contador++;
                }
            }
            
            // Calcular promedio
            Vec3b color_promedio(
                suma_b / contador,
                suma_g / contador,
                suma_r / contador
            );
            
            // Aplicar el color promedio a todo el bloque
            for (int by = y; by < min(y + tamano_bloque, imagen_pixelada.rows); by++) {
                for (int bx = x; bx < min(x + tamano_bloque, imagen_pixelada.cols); bx++) {
                    imagen_pixelada.at<Vec3b>(by, bx) = color_promedio;
                }
            }
        }
    }
    
    cout << "✓ Efecto de pixelación aplicado" << endl;
    cout << "  Tamaño de bloque: " << tamano_bloque << "x" << tamano_bloque << " píxeles" << endl;
    mostrarImagen("13.12 Efecto Pixelación", imagen_pixelada, false);
    
    cout << "\n→ Presiona cualquier tecla para continuar..." << endl;
    waitKey(0);
    destroyAllWindows();
    
    // ═══════════════════════════════════════════════════════════════════════════
    // RESUMEN DE MANIPULACIÓN DE PÍXELES
    // ═══════════════════════════════════════════════════════════════════════════
    cout << "\n╔═══════════════════════════════════════════════════════════════════════════╗" << endl;
    cout << "║              RESUMEN: MANIPULACIÓN DIRECTA DE PÍXELES                    ║" << endl;
    cout << "╚═══════════════════════════════════════════════════════════════════════════╝" << endl;
    
    cout << "\n📚 TÉCNICAS APRENDIDAS:" << endl;
    cout << "\n1️⃣  ACCESO A PÍXELES:" << endl;
    cout << "   • at<Vec3b>(y, x) para imágenes BGR" << endl;
    cout << "   • at<uchar>(y, x) para escala de grises" << endl;
    cout << "   • Vec3b[0] = Blue, Vec3b[1] = Green, Vec3b[2] = Red" << endl;
    
    cout << "\n2️⃣  MODIFICACIÓN SELECTIVA:" << endl;
    cout << "   • Procesar solo una región (mitad, cuadrante, diagonal)" << endl;
    cout << "   • Usar condiciones if para aplicar selectivamente" << endl;
    
    cout << "\n3️⃣  OPERACIONES ENTRE CANALES:" << endl;
    cout << "   • Promedio: (B + G + R) / 3" << endl;
    cout << "   • Máximo: max(B, G, R)" << endl;
    cout << "   • Mínimo: min(B, G, R)" << endl;
    
    cout << "\n4️⃣  TRANSFORMACIONES DE COLOR:" << endl;
    cout << "   • Negación: 255 - valor" << endl;
    cout << "   • Intercambio de canales" << endl;
    cout << "   • Aislamiento de canales" << endl;
    cout << "   • Sepia (transformación matricial)" << endl;
    
    cout << "\n5️⃣  COMBINACIÓN DE IMÁGENES:" << endl;
    cout << "   • Unir imágenes complementarias" << endl;
    cout << "   • Crear mosaicos de efectos" << endl;
    cout << "   • Mezclar regiones de diferentes imágenes" << endl;
    
    cout << "\n6️⃣  EFECTOS GEOMÉTRICOS:" << endl;
    cout << "   • Espejo horizontal y vertical" << endl;
    cout << "   • Rotación píxel a píxel" << endl;
    cout << "   • Dibujo de formas básicas" << endl;
    
    cout << "\n7️⃣  EFECTOS ARTÍSTICOS:" << endl;
    cout << "   • Pixelación (reducción de resolución)" << endl;
    cout << "   • Efecto sepia (vintage)" << endl;
    cout << "   • Mosaicos de múltiples efectos" << endl;
    
    cout << "\n💡 MEJORES PRÁCTICAS:" << endl;
    cout << "   ✓ Siempre clonar la imagen antes de modificar: imagen.clone()" << endl;
    cout << "   ✓ Verificar límites: x < cols, y < rows" << endl;
    cout << "   ✓ Usar min(255, valor) para evitar desbordamiento" << endl;
    cout << "   ✓ Los bucles exteriores deben ser por filas (y), luego columnas (x)" << endl;
    cout << "   ✓ Considerar el rendimiento: acceso directo al puntero es más rápido" << endl;
    
    cout << "\n⚡ OPTIMIZACIÓN AVANZADA (para grandes imágenes):" << endl;
    cout << "   • Usar punteros directos en lugar de at<>()" << endl;
    cout << "   • Verificar isContinuous() para optimizar el acceso" << endl;
    cout << "   • Usar OpenMP para paralelizar bucles" << endl;
    cout << "   • Considerar operaciones vectorizadas (SIMD)" << endl;
    
    cout << "\n🎯 CASOS DE USO PRÁCTICOS:" << endl;
    cout << "   • Censura/pixelación de rostros" << endl;
    cout << "   • Marcas de agua personalizadas" << endl;
    cout << "   • Corrección de color selectiva" << endl;
    cout << "   • Efectos artísticos únicos" << endl;
    cout << "   • Procesamiento customizado no disponible en funciones estándar" << endl;
    
    cout << "\n→ Presiona cualquier tecla para continuar al resumen final..." << endl;
    waitKey(0);
    
    // ═══════════════════════════════════════════════════════════════════════════
    // RESUMEN FINAL
    // ═══════════════════════════════════════════════════════════════════════════
    imprimirSeparador("RESUMEN DEL TUTORIAL");
    
    cout << "\n🎓 Has completado el tutorial de Procesamiento Digital de Imágenes!" << endl;
    cout << "\n📚 Técnicas cubiertas:" << endl;
    cout << "   ✓ 1.  Carga y verificación de imágenes" << endl;
    cout << "   ✓ 2.  Extracción de información y propiedades" << endl;
    cout << "   ✓ 3.  Conversiones de espacios de color (BGR, HSV, LAB, GRAY)" << endl;
    cout << "   ✓ 4.  Operaciones básicas (brillo, contraste, rotación, escalado)" << endl;
    cout << "   ✓ 5.  Filtrado y suavizado (Gaussiano, Mediana, Bilateral)" << endl;
    cout << "   ✓ 6.  Detección de bordes (Sobel, Laplaciano, Canny)" << endl;
    cout << "   ✓ 7.  Morfología (Erosión, Dilatación, Apertura, Cierre)" << endl;
    cout << "   ✓ 8.  Umbralización (Simple, Otsu, Adaptativa)" << endl;
    cout << "   ✓ 9.  Histogramas y ecualización (Histograma, equalizeHist, CLAHE)" << endl;
    cout << "   ✓ 10. Detección y análisis de contornos" << endl;
    cout << "   ✓ 11. Operaciones lógicas (AND, OR, XOR, NOT)" << endl;
    cout << "   ✓ 12. Segmentación por color (inRange, máscaras)" << endl;
    cout << "   ✓ 13. Manipulación directa de píxeles (12 ejercicios prácticos)" << endl;
    
    cout << "\n💡 Consejos para tus proyectos:" << endl;
    cout << "   • Siempre verifica que la imagen se cargó correctamente" << endl;
    cout << "   • Suaviza antes de detectar bordes para reducir ruido" << endl;
    cout << "   • HSV es mejor que BGR para segmentación por color" << endl;
    cout << "   • Usa morfología para limpiar máscaras binarias" << endl;
    cout << "   • CLAHE > equalizeHist para iluminación no uniforme" << endl;
    cout << "   • Canny es el mejor detector de bordes general" << endl;
    cout << "   • Umbralización adaptativa > simple para iluminación variable" << endl;
    
    cout << "\n🔗 Flujos de trabajo comunes:" << endl;
    cout << "\n   DETECCIÓN DE OBJETOS:" << endl;
    cout << "   Imagen → Gris → Suavizado → Umbralización → Morfología → Contornos" << endl;
    
    cout << "\n   SEGMENTACIÓN POR COLOR:" << endl;
    cout << "   Imagen → HSV → inRange → Morfología → AND con original" << endl;
    
    cout << "\n   MEJORA DE CONTRASTE:" << endl;
    cout << "   Imagen → Gris → CLAHE → Procesamiento adicional" << endl;
    
    cout << "\n   DETECCIÓN DE BORDES:" << endl;
    cout << "   Imagen → Gris → GaussianBlur → Canny → Procesamiento" << endl;
    
    cout << "\n📖 Recursos adicionales:" << endl;
    cout << "   • Documentación OpenCV: https://docs.opencv.org/" << endl;
    cout << "   • Tutoriales: https://docs.opencv.org/master/d9/df8/tutorial_root.html" << endl;
    cout << "   • Ejemplos: https://github.com/opencv/opencv/tree/master/samples" << endl;
    
    imprimirSeparador();
    cout << "\n¡Gracias por usar este tutorial! Ahora estás listo para crear tus propios" << endl;
    cout << "proyectos de visión por computador. 🚀\n" << endl;
    
    return 0;
}
