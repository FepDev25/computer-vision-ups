#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    // Verificar que se proporcionó una ruta de imagen
    if (argc < 2) {
        cout << "Uso: " << argv[0] << " <ruta_imagen>" << endl;
        return -1;
    }

    // 1. CARGAR IMAGEN
    Mat imagen_original = imread(argv[1]);
    if (imagen_original.empty()) {
        cout << "Error: No se pudo cargar la imagen" << endl;
        return -1;
    }
    cout << "Imagen cargada: " << imagen_original.size() << endl;

    // 2. CONVERSIÓN A ESCALA DE GRISES
    Mat imagen_gris;
    cvtColor(imagen_original, imagen_gris, COLOR_BGR2GRAY);

    // 3. APLICAR DESENFOQUE GAUSSIANO (reduce ruido)
    Mat imagen_desenfocada;
    GaussianBlur(imagen_gris, imagen_desenfocada, Size(5, 5), 0);

    // 4. DETECCIÓN DE BORDES (Canny)
    Mat bordes;
    Canny(imagen_desenfocada, bordes, 50, 150);

    // 5. UMBRALIZACIÓN (Binarización)
    Mat imagen_binaria;
    threshold(imagen_gris, imagen_binaria, 127, 255, THRESH_BINARY);

    // 6. DETECCIÓN DE CONTORNOS
    vector<vector<Point>> contornos;
    findContours(bordes.clone(), contornos, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    // Dibujar contornos sobre la imagen original
    Mat imagen_contornos = imagen_original.clone();
    drawContours(imagen_contornos, contornos, -1, Scalar(0, 255, 0), 2);
    cout << "Contornos detectados: " << contornos.size() << endl;

    // 7. HISTOGRAMA
    Mat histograma;
    int histSize = 256;
    float range[] = {0, 256};
    const float* histRange = {range};
    calcHist(&imagen_gris, 1, 0, Mat(), histograma, 1, &histSize, &histRange);

    // Visualizar histograma
    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);
    Mat imagenHistograma(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));
    normalize(histograma, histograma, 0, imagenHistograma.rows, NORM_MINMAX);
    
    for(int i = 1; i < histSize; i++) {
        line(imagenHistograma, 
             Point(bin_w*(i-1), hist_h - cvRound(histograma.at<float>(i-1))),
             Point(bin_w*i, hist_h - cvRound(histograma.at<float>(i))),
             Scalar(255, 255, 255), 2);
    }

    // 8. REDIMENSIONAR IMAGEN
    Mat imagen_redimensionada;
    resize(imagen_original, imagen_redimensionada, Size(800, 600));

    // MOSTRAR TODAS LAS IMÁGENES PROCESADAS
    imshow("1. Original", imagen_original);
    imshow("2. Escala de Grises", imagen_gris);
    imshow("3. Desenfoque Gaussiano", imagen_desenfocada);
    imshow("4. Detección de Bordes (Canny)", bordes);
    imshow("5. Binarización", imagen_binaria);
    imshow("6. Contornos Detectados", imagen_contornos);
    imshow("7. Histograma", imagenHistograma);
    imshow("8. Redimensionada", imagen_redimensionada);

    cout << "\nPresiona cualquier tecla para cerrar las ventanas..." << endl;
    waitKey(0);
    destroyAllWindows();

    // GUARDAR RESULTADOS
    imwrite("resultado_gris.jpg", imagen_gris);
    imwrite("desenfoque_gaussiano.jpg", imagen_desenfocada);
    imwrite("resultado_bordes.jpg", bordes);
    imwrite("resultado_binaria.jpg", imagen_binaria);
    imwrite("resultado_contornos.jpg", imagen_contornos);
    imwrite("resultado_histograma.jpg", imagenHistograma);
    imwrite("resultado_redimensionada.jpg", imagen_redimensionada);


    cout << "\nImágenes guardadas exitosamente!" << endl;

    return 0;
}