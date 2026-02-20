# GUÍA TÉCNICA: Invarianza de Descriptores de Fourier (FFT)

**Asignatura:** Visión por Computador  
**Tema:** Extracción de Características y Shape Signature  
**Recurso:** Guía de Estudio

---

## 1. El Problema de la Invarianza

Para un computador, una imagen no es una forma abstracta, es una matriz de números. Si tomamos una figura geométrica y la rotamos o la hacemos más grande, los números en la matriz cambian por completo, aunque el objeto sea el mismo.

A continuación, visualizamos este problema utilizando dos imágenes (matrices) de $699 \times 491$ que contienen exactamente el mismo **polígono irregular**, pero bajo distintas transformaciones.

### MATRIZ A: Figura Original

**Características:** Pequeña, Orientación Original (0°).
Esta imagen (matriz) representa un polígono irregular definido por (área) de color azul. Observa que es compacto y ocupa una región pequeña de la cuadrícula.

![Polígono original](./Polígono-Original.png)

### MATRIZ B: Figura Transformada

**Transformaciones aplicadas:**

1. **Escalado (44%):** La figura es el menos de la mitad de grande (observa que ahora tiene menos píxeles de ancho y alto).

2. **Rotación (210°):** La "punta" del inferior del polígono ahora apunta hacia la parte superior derecha.

3. **Traslación:** Se ha movido de posición en la cuadrícula.

![Polígono Rotado, Escalado y Trasladado](./Polígono-RET.png)

**Conclusión Visual:** Aunque $\mathbf{M}_A$ y $\mathbf{M}_B$ no comparten píxeles en las mismas coordenadas, **representan el mismo objeto**. Nuestro algoritmo debe producir la misma "firma" para ambos.

---

## 2. Pipeline Matemático (Paso a Paso)

Para lograr la invarianza, transformamos los datos espaciales a frecuencia siguiendo estos pasos:

### Paso 1: Re-muestreo (Resampling)

* **Problema:** La figura en $\mathbf{M}_A$ tiene un perímetro de aprox. 12 píxeles. La figura en $\mathbf{M}_B$, al ser doble de grande, tiene un perímetro de aprox. 24 píxeles. La FFT necesita vectores de entrada del mismo tamaño.
* **Solución:** Interpolamos ambos contornos para obtener exactamente $N=64$ puntos.

### Paso 2: Invarianza a Traslación (Centrado)

Calculamos el centroide $(\bar{x}, \bar{y})$ de la nube de puntos y restamos este valor a cada coordenada.
$$P_{nuevo} = P_{original} - Centroide$$
Esto mueve ambas figuras al origen $(0,0)$, eliminando el efecto de la traslación.

### Paso 3: Shape Signature (Señal 1D)

Convertimos el contorno 2D en una señal 1D calculando la distancia de cada punto al centroide:
$$r(n) = \sqrt{x(n)^2 + y(n)^2}$$

### Paso 4: Invarianza a Escala y Rotación (FFT)

Aplicamos la Transformada Discreta de Fourier y normalizamos:

* **Rotación:** Usamos la Magnitud $|F(k)|$. (Elimina la fase/giro).
* **Escala:** Dividimos por el componente DC ($F(0)$).
    $$Descriptor(k) = \frac{|F(k)|}{|F(0)|}$$

---

## 3. Implementación en OpenCV C++

El siguiente código implementa el pipeline completo y compara las dos matrices teóricas anteriores.

```cpp
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace cv;
using namespace std;

// Función crítica: Normaliza el número de vértices a N
vector<Point2f> resampleContour(const vector<Point>& contour, int N) {
    vector<Point2f> resampled;
    double perimeter = arcLength(contour, true);
    double step = perimeter / N;
    
    double currentDist = 0.0;
    int idx = 0;
    // Interpolación lineal a lo largo del perímetro
    for (int i = 0; i < N; i++) {
        double target = i * step;
        double segLen = norm(contour[(idx+1)%contour.size()] - contour[idx]);
        while (currentDist + segLen < target) {
            currentDist += segLen;
            idx = (idx + 1) % contour.size();
            segLen = norm(contour[(idx+1)%contour.size()] - contour[idx]);
        }
        Point2f A = contour[idx];
        Point2f B = contour[(idx+1)%contour.size()];
        double alpha = (target - currentDist) / segLen;
        resampled.push_back(A + alpha * (B - A));
    }
    return resampled;
}

vector<float> computeSignature(const vector<Point>& rawContour) {
    int N = 64; 
    vector<Point2f> contour = resampleContour(rawContour, N);
    
    // Calcular Centroide y Distancias
    Moments mu = moments(contour);
    Point2f centroid(mu.m10 / mu.m00, mu.m01 / mu.m00);
    vector<float> signal(N);
    for(int i=0; i<N; i++) signal[i] = norm(contour[i] - centroid);
    
    // DFT
    Mat dftOut;
    dft(signal, dftOut, DFT_COMPLEX_OUTPUT);
    
    // Magnitud y Normalización
    vector<float> mags;
    vector<Mat> planes;
    split(dftOut, planes);
    magnitude(planes[0], planes[1], mags);
    
    vector<float> output;
    float dc = mags[0];
    for(int i=1; i<=10; i++) output.push_back(mags[i] / dc);
    
    return output;
} 
```
