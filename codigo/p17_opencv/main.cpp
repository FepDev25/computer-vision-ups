#include <iostream>
#include <iomanip>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
using namespace dnn;

// Descriptores de Fourier
// Shape signature: extraer los bordes (ej canny), y a partir de ellos encontrar los contornos 
    // (puntos que conforman la forma de la figura)
// Para cada coordenada de contorno se busca las coordenadas x,y
// calcular el centroide de la figura
// calcular la distancia de cada punto del contorno al centroide
    // en un circulo, todas las distancias son iguales
        // la firma en este caso es un linea recta (x=numero del punto, y=distancia al centroide)
    // en un triangulo, las distancias crecen hasta las esquinas y decrecen en los lados
        // la firma en este caso es una señal periodica

// distancia euclidiana entre dos puntos
double distanciaEuclidiana(Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Función para normalizar el contorno a N puntos usando interpolación lineal
vector<Point2f> resampleContour(const vector<Point>& contour, int N) {
    vector<Point2f> resampled;
    double perimeter = arcLength(contour, true);
    double step = perimeter / N;
    
    double currentDist = 0.0;
    int idx = 0;
    
    // Interpolación lineal a lo largo del perímetro
    for (int i = 0; i < N; i++) {
        double target = i * step;
        double segLen = norm(contour[(idx+1) % contour.size()] - contour[idx]);
        
        while (currentDist + segLen < target && idx < (int)contour.size() - 1) {
            currentDist += segLen;
            idx = (idx + 1) % contour.size();
            segLen = norm(contour[(idx+1) % contour.size()] - contour[idx]);
        }
        
        Point2f A = contour[idx];
        Point2f B = contour[(idx+1) % contour.size()];
        double alpha = (segLen > 0) ? (target - currentDist) / segLen : 0;
        resampled.push_back(A + alpha * (B - A));
    }
    return resampled;
}

// Función para calcular la firma normalizada y obtener descriptores de Fourier
vector<float> computeFourierDescriptors(const vector<Point>& rawContour, int N = 64) {
    // Paso 1: Resampling - normalizar a N puntos
    vector<Point2f> contour = resampleContour(rawContour, N);
    
    // Paso 2: Centrado - calcular centroide
    Moments mu = moments(contour);
    Point2f centroid(mu.m10 / mu.m00, mu.m01 / mu.m00);
    
    // Paso 3: Shape Signature - calcular distancias al centroide
    vector<float> signal(N);
    for(int i = 0; i < N; i++) {
        signal[i] = norm(contour[i] - centroid);
    }
    
    // Paso 4: DFT
    Mat dftOut;
    dft(signal, dftOut, DFT_COMPLEX_OUTPUT);
    
    // Calcular magnitudes
    vector<Mat> planes;
    split(dftOut, planes);
    Mat magnitudes;
    magnitude(planes[0], planes[1], magnitudes);
    
    // Normalizar por el componente DC (F(0))
    vector<float> descriptors;
    float dc = (magnitudes.rows > magnitudes.cols) ? magnitudes.at<float>(0, 0) : magnitudes.at<float>(0);
    
    if (dc == 0) dc = 1.0; // evitar división por cero
    
    // Guardar los primeros 12 descriptores normalizados (empezando desde k=1)
    int totalElements = max(magnitudes.rows, magnitudes.cols);
    int numDescriptors = min(12, totalElements - 1);
    
    for(int i = 1; i <= numDescriptors; i++) {
        float mag = (magnitudes.rows > magnitudes.cols) ? magnitudes.at<float>(i, 0) : magnitudes.at<float>(i);
        descriptors.push_back(mag / dc);
    }
    
    return descriptors;
}

int main(int argc, char** argv) {
    
    Mat poligonoO = imread(argv[1]);
    Mat poligonoRET = imread(argv[2]);
    Mat grisO, grisRET;
    Mat bordesCannyO, bordesCannyRET;
    
    if (poligonoO.empty()) {
        cout << "Error: No se pudo leer la imagen original: " << argv[1] << endl;
        return -1;
    }
    if (poligonoRET.empty()) {
        cout << "Error: No se pudo leer la imagen RET: " << argv[2] << endl;
        return -1;
    }
    
    Mat contornosO = Mat(Size(poligonoO.cols, poligonoO.rows), CV_8UC3, Scalar(255,255,255));
    Mat contornosRET = Mat(Size(poligonoRET.cols, poligonoRET.rows), CV_8UC3, Scalar(255,255,255));

    // Procesamiento de polígono original
    cout << "\n POLÍGONO ORIGINAL " << endl;
    
    // Convertir a escala de grises
    cvtColor(poligonoO, grisO, COLOR_BGR2GRAY);

    // Aplicar un suavizado Gaussiano
    GaussianBlur(grisO, grisO, Size(3,3), 0.73);

    // Aplicar Canny para obtener los bordes
    Canny(grisO, bordesCannyO, 100, 200);

    // Calcular momentos para obtener el centroide
    Moments _moments = moments(bordesCannyO, true);
    double cx = _moments.m10 / _moments.m00;
    double cy = _moments.m01 / _moments.m00;
    cout << "Centroide: (" << cx << ", " << cy << ")" << endl;
    Point centroide(cx, cy);

    vector<vector<Point>> puntosO;
    vector<Vec4i> jerarquiaO;

    // Encontrar los contornos (findContours modifica la imagen, usar copia)
    Mat bordesCannyOCopy = bordesCannyO.clone();
    findContours(bordesCannyOCopy, puntosO, jerarquiaO, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    // imprimir cuantos puntos hay por cada contorno
    for (size_t i = 0; i < puntosO.size(); i++) {
        cout << "Contorno " << i << " tiene " << puntosO[i].size() << " puntos." << endl;
    }

    // Dibujar los contornos encontrados
    drawContours(contornosO, puntosO, 0, Scalar(3,3,233), 1);
    // Dibujar el centroide
    circle(contornosO, Point(cx, cy), 3, Scalar(3,3,233), -1);

    // Calcular las distancias de cada punto al centroide
    vector<double> distanciasO;
    double maxY = 0.0;
    double maxX = puntosO[0].size();

    for (size_t i = 0; i < puntosO[0].size(); i++) {
        distanciasO.push_back(distanciaEuclidiana(puntosO[0][i], centroide));

        if (distanciasO[distanciasO.size() - 1] > maxY) {
            maxY = distanciasO[distanciasO.size() - 1];
        }
    }
    cout << "Distancias al centroide del primer contorno: " << distanciasO.size() << endl;

    Mat firmaO = Mat::zeros(1024, 780, CV_8UC3);
    double escalaY = 780 / maxY;
    double escalaX = 1024 / maxX;
    for (size_t i = 0; i < distanciasO.size(); i++) {
        circle(firmaO, Point(i * escalaX, firmaO.rows - (escalaY * distanciasO[i])), 2, Scalar(3, 3, 233), -1);
    }

    //  PROCESAMIENTO POLÍGONO RET 
    cout << "\n POLÍGONO RET (Rotado, Escalado, Trasladado) " << endl;
    
    vector<vector<Point>> puntosRET;
    vector<Vec4i> jerarquiaRET;
    
    // Convertir a escala de grises
    cvtColor(poligonoRET, grisRET, COLOR_BGR2GRAY);

    // Aplicar un suavizado Gaussiano
    GaussianBlur(grisRET, grisRET, Size(5,5), 1.5);

    // Usar umbralización binaria inversa en lugar de Canny para mejor detección
    // Esto funciona mejor cuando el polígono es más claro que el fondo
    Mat binariaRET;
    threshold(grisRET, binariaRET, 200, 255, THRESH_BINARY);
    
    // Aplicar Canny a la imagen binarizada
    Canny(binariaRET, bordesCannyRET, 100, 200);

    // Encontrar los contornos (findContours modifica la imagen, usar copia)
    Mat bordesCannyRETCopy = bordesCannyRET.clone();
    findContours(bordesCannyRETCopy, puntosRET, jerarquiaRET, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    // imprimir cuantos puntos hay por cada contorno y su área
    for (size_t i = 0; i < puntosRET.size(); i++) {
        double area = contourArea(puntosRET[i]);
        cout << "Contorno " << i << " tiene " << puntosRET[i].size() 
             << " puntos, área: " << area << endl;
    }
    
    // Encontrar el contorno más grande
    int largestContourIdxRET = 0;
    size_t largestSizeRET = 0;
    for (size_t i = 0; i < puntosRET.size(); i++) {
        if (puntosRET[i].size() > largestSizeRET) {
            largestSizeRET = puntosRET[i].size();
            largestContourIdxRET = i;
        }
    }
    double largestAreaRET = contourArea(puntosRET[largestContourIdxRET]);
    
    // Calcular centroide del contorno más grande
    Moments _momentsRET = moments(puntosRET[largestContourIdxRET]);
    double cxRET = _momentsRET.m10 / _momentsRET.m00;
    double cyRET = _momentsRET.m01 / _momentsRET.m00;
    cout << "Centroide del contorno principal (contorno " << largestContourIdxRET 
         << ", " << largestSizeRET << " puntos): (" << cxRET << ", " << cyRET << ")" << endl;
    Point centroideRET(cxRET, cyRET);

    // Dibujar los contornos encontrados (el más grande)
    drawContours(contornosRET, puntosRET, largestContourIdxRET, Scalar(233,3,3), 1);
    // Dibujar el centroide
    circle(contornosRET, Point(cxRET, cyRET), 3, Scalar(233,3,3), -1);

    // Calcular las distancias de cada punto al centroide
    vector<double> distanciasRET;
    double maxYRET = 0.0;
    double maxXRET = puntosRET[largestContourIdxRET].size();

    for (size_t i = 0; i < puntosRET[largestContourIdxRET].size(); i++) {
        distanciasRET.push_back(distanciaEuclidiana(puntosRET[largestContourIdxRET][i], centroideRET));

        if (distanciasRET[distanciasRET.size() - 1] > maxYRET) {
            maxYRET = distanciasRET[distanciasRET.size() - 1];
        }
    }
    cout << "Distancias al centroide: " << distanciasRET.size() << endl;

    Mat firmaRET = Mat::zeros(1024, 780, CV_8UC3);
    double escalaYRET = 780 / maxYRET;
    double escalaXRET = 1024 / maxXRET;
    for (size_t i = 0; i < distanciasRET.size(); i++) {
        circle(firmaRET, Point(i * escalaXRET, firmaRET.rows - (escalaYRET * distanciasRET[i])), 2, Scalar(233, 3, 3), -1);
    }

    //  DESCRIPTORES DE FOURIER 
    cout << "\n DESCRIPTORES DE FOURIER " << endl;
    
    // Encontrar el contorno más grande del polígono original
    int largestContourIdxO = 0;
    double largestAreaO = 0;
    for (size_t i = 0; i < puntosO.size(); i++) {
        double area = contourArea(puntosO[i]);
        if (area > largestAreaO) {
            largestAreaO = area;
            largestContourIdxO = i;
        }
    }
    
    cout << "Usando contorno " << largestContourIdxO << " del Original (area: " << largestAreaO << ")" << endl;
    cout << "Usando contorno " << largestContourIdxRET << " del RET (area: " << largestAreaRET << ")" << endl;
    
    vector<float> descriptorsO = computeFourierDescriptors(puntosO[largestContourIdxO], 64);
    vector<float> descriptorsRET = computeFourierDescriptors(puntosRET[largestContourIdxRET], 64);
    
    cout << "\nDescriptores calculados: Original=" << descriptorsO.size() 
         << ", RET=" << descriptorsRET.size() << endl;
    
    if (descriptorsO.empty() || descriptorsRET.empty()) {
        cout << "Error: No se pudieron calcular los descriptores de Fourier" << endl;
        return -1;
    }
    
    int numToShow = min((int)descriptorsO.size(), (int)descriptorsRET.size());
    numToShow = min(numToShow, 12);
    
    cout << "\nPrimeros " << numToShow << " Descriptores de Fourier (Normalizados):\n" << endl;
    cout << "k\tOriginal\tRET\t\tDiferencia (%)" << endl;
    
    for (int i = 0; i < numToShow; i++) {
        float diff = abs(descriptorsO[i] - descriptorsRET[i]) / descriptorsO[i] * 100;
        cout << fixed << setprecision(4);
        cout << (i+1) << "\t" << descriptorsO[i] << "\t\t" << descriptorsRET[i] 
             << "\t\t" << diff << "%" << endl;
    }
    
    // Visualización gráfica de los descriptores
    Mat comparacion = Mat::zeros(400, 600, CV_8UC3);
    comparacion = Scalar(255, 255, 255); // fondo blanco
    
    // Encontrar el máximo para escalar
    float maxDesc = 0;
    for (int i = 0; i < numToShow; i++) {
        maxDesc = max(maxDesc, max(descriptorsO[i], descriptorsRET[i]));
    }
    
    // Dibujar barras
    int barWidth = 40;
    int spacing = 10;
    int offsetX = 30;
    int graphHeight = 350;
    
    for (int i = 0; i < numToShow; i++) {
        int x = offsetX + i * (barWidth + spacing);
        
        // Barra azul (Original)
        int heightO = (int)(descriptorsO[i] / maxDesc * graphHeight);
        rectangle(comparacion, 
                  Point(x, 380 - heightO), 
                  Point(x + barWidth/2 - 2, 380), 
                  Scalar(233, 3, 3), -1);
        
        // Barra roja (RET)
        int heightRET = (int)(descriptorsRET[i] / maxDesc * graphHeight);
        rectangle(comparacion, 
                  Point(x + barWidth/2 + 2, 380 - heightRET), 
                  Point(x + barWidth, 380), 
                  Scalar(3, 3, 233), -1);
        
        // Etiqueta del índice
        putText(comparacion, to_string(i+1), Point(x + 10, 395), 
                FONT_HERSHEY_SIMPLEX, 0.3, Scalar(0,0,0), 1);
    }
    
    // Leyenda
    rectangle(comparacion, Point(450, 20), Point(470, 40), Scalar(233, 3, 3), -1);
    putText(comparacion, "Original", Point(475, 35), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0,0,0), 1);
    
    rectangle(comparacion, Point(450, 50), Point(470, 70), Scalar(3, 3, 233), -1);
    putText(comparacion, "RET", Point(475, 65), FONT_HERSHEY_SIMPLEX, 0.4, Scalar(0,0,0), 1);

    //  GUARDAR IMÁGENES 
    // Crear carpeta de salida
    string outputDir = "resultados_fourier";
    system(("mkdir -p " + outputDir).c_str());
    
    cout << "\n GUARDANDO IMÁGENES " << endl;
    cout << "Carpeta de salida: " << outputDir << "/" << endl;
    
    // Guardar todas las imágenes generadas
    imwrite(outputDir + "/01_poligono_original.png", poligonoO);
    imwrite(outputDir + "/02_poligono_RET.png", poligonoRET);
    imwrite(outputDir + "/03_bordes_canny_original.png", bordesCannyO);
    imwrite(outputDir + "/04_bordes_canny_RET.png", bordesCannyRET);
    imwrite(outputDir + "/05_contornos_original.png", contornosO);
    imwrite(outputDir + "/06_contornos_RET.png", contornosRET);
    imwrite(outputDir + "/07_firma_original.png", firmaO);
    imwrite(outputDir + "/08_firma_RET.png", firmaRET);
    imwrite(outputDir + "/09_comparacion_descriptores.png", comparacion);
    
    cout << "01_poligono_original.png guardada" << endl;
    cout << "02_poligono_RET.png guardada" << endl;
    cout << "03_bordes_canny_original.png guardada" << endl;
    cout << "04_bordes_canny_RET.png guardada" << endl;
    cout << "05_contornos_original.png guardada" << endl;
    cout << "06_contornos_RET.png guardada" << endl;
    cout << "07_firma_original.png guardada" << endl;
    cout << "08_firma_RET.png guardada" << endl;
    cout << "09_comparacion_descriptores.png guardada" << endl;

    // == VISUALIZACIÓN ==
    namedWindow("Poligono Original", WINDOW_NORMAL);
    namedWindow("Poligono RET", WINDOW_NORMAL);
    namedWindow("Bordes Canny Original", WINDOW_NORMAL);
    namedWindow("Bordes Canny RET", WINDOW_NORMAL);
    namedWindow("Contornos Original", WINDOW_NORMAL);
    namedWindow("Contornos RET", WINDOW_NORMAL);
    namedWindow("Firma Original", WINDOW_NORMAL);
    namedWindow("Firma RET", WINDOW_NORMAL);
    namedWindow("Comparacion Descriptores de Fourier", WINDOW_NORMAL);

    imshow("Poligono Original", poligonoO);
    imshow("Poligono RET", poligonoRET);
    imshow("Bordes Canny Original", bordesCannyO);
    imshow("Bordes Canny RET", bordesCannyRET);
    imshow("Contornos Original", contornosO);
    imshow("Contornos RET", contornosRET);
    imshow("Firma Original", firmaO);
    imshow("Firma RET", firmaRET);
    imshow("Comparacion Descriptores de Fourier", comparacion);
    
    waitKey(0);

    return 0;
}

// Pipeline Matematico:

// 1. Resampling: 
    // cuando se cambia el tamaño de una imagen, se deben crear nuevos pixeles
// 2. Invarianza a traslacion (centrado)
// 3. Shape signature: representacion 1D de la forma
// 4. Invarianza a escala y rotacion

// tarea
// obtener los 10 o 12 primeros numeros de ambas figuras y deben ser similares para figuras similares
// 1. firmas y normalizar con interpolacion lineal
// 2. trasnformada de fourier, en lugar de una senal, darle una matriz, primer canal componente real y segundo canal componente imaginario