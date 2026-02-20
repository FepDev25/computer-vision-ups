# Proyecto 09: Umbralización Avanzada y Filtros de Suavizado

## 📋 Descripción General

Este proyecto explora técnicas avanzadas de binarización implementando una función sigmoidal personalizada, comparándola con métodos estándar de umbralización, y aplicando filtros de mediana para reducción de ruido.

## 🎯 Objetivos de Aprendizaje

- Implementar umbralización con función sigmoidal
- Comprender la diferencia entre umbralización dura y suave
- Dominar el uso de trackbars para ajuste interactivo
- Aplicar filtro de mediana para reducción de ruido
- Comparar métodos de binarización en tiempo real

## 📚 Conceptos Teóricos Fundamentales

### 1. Umbralización (Thresholding)

#### Umbralización Estándar (Dura)

```cpp
threshold(frame, binariaT, mT, 255, THRESH_BINARY_INV);
```

**Función matemática**:
$$
dst(x,y) = \begin{cases}
255 & \text{si } src(x,y) \leq T \\
0 & \text{si } src(x,y) > T
\end{cases}
$$

**Características**:

- Transición **abrupta** en el umbral T
- Resultado: blanco o negro, sin grises
- Simple pero puede perder información

**Tipos en OpenCV**:

| Tipo | Fórmula | Resultado |
|------|---------|-----------|
| `THRESH_BINARY` | $dst = (src > T) ? 255 : 0$ | Normal |
| `THRESH_BINARY_INV` | $dst = (src > T) ? 0 : 255$ | Invertida |
| `THRESH_TRUNC` | $dst = (src > T) ? T : src$ | Truncamiento |
| `THRESH_TOZERO` | $dst = (src > T) ? src : 0$ | A cero |
| `THRESH_OTSU` | Calcula T automáticamente | Automática |

### 2. Función Sigmoidal (Umbralización Suave)

#### Implementación en el Código

```cpp
Mat sigmoideo(Mat imagen, double k = 1.3) {
    int pixel = 0;
    int opera = 0;
    Mat binaria = Mat::zeros(Size(imagen.cols, imagen.rows), CV_8UC1);

    for(int i = 0; i < imagen.rows; i++) {
        for(int j = 0; j < imagen.cols; j++) {
            pixel = imagen.at<uchar>(i,j);
            
            opera = 255.0 / (1.0 + exp(-k * (pixel - ((double)mT))));
            
            binaria.at<uchar>(i,j) = opera;
        }
    }
    return binaria;
}
```

#### Fórmula Matemática

$$
S(x) = \frac{255}{1 + e^{-k(x - m)}}
$$

Donde:

- **x**: Valor del píxel (0-255)
- **k**: Pendiente de la curva (qué tan abrupta es la transición)
- **m**: Punto medio (umbral, mT en el código)
- **S(x)**: Valor de salida (0-255)

#### Análisis de Parámetros

**Parámetro k (Pendiente)**:

- **k pequeño (k ≈ 0.1)**: Transición muy suave, muchos grises
- **k moderado (k ≈ 1.0)**: Transición equilibrada
- **k grande (k ≈ 10)**: Transición abrupta, similar a umbralización dura

**Parámetro m (Umbral)**:

- Define el punto central de la transición
- Valores por debajo de m → tienden a negro
- Valores por encima de m → tienden a blanco

#### Visualización de la Función

```bash
Salida (0-255)
    255 |          ╭────
        |        ╱
        |      ╱
    128 |    ╱ (punto m)
        |  ╱
        |╱
      0 +───────────────> Entrada (0-255)
        0      m      255
```

### 3. Comparación: Umbralización Dura vs Sigmoidal

#### Umbralización Dura (threshold)

```bash
Entrada:  0 --- 127 | 128 --- 255
                    ↑ (umbral)
Salida:   0 ----- 0 | 255 --- 255
```

**Ventajas**:
✅ Muy rápida
✅ Resultado claro (binario puro)
✅ Ideal para documentos

**Desventajas**:
❌ Pierde información de transición
❌ Sensible al ruido
❌ Puede crear bordes artificiales

#### Umbralización Sigmoidal (suave)

```bash
Entrada:  0 --- 100 --- 150 --- 255
                    ↑ (umbral m)
Salida:   0 --- 50 --- 200 --- 255
               (transición gradual)
```

**Ventajas**:
✅ Transición suave
✅ Preserva información de bordes
✅ Menos sensible a ruido
✅ Más natural visualmente

**Desventajas**:
❌ Más lenta (cálculo exponencial)
❌ No es binaria pura
❌ Requiere ajuste de parámetros

### 4. Filtro de Mediana

#### Concepto

Reemplaza cada píxel con la **mediana** de sus vecinos, eliminando ruido tipo "sal y pimienta".

#### Implementación en OpenCV

```cpp
medianBlur(frame, mediana, kSize);
```

**Parámetro kSize**:

- Tamaño del kernel (debe ser **impar**: 3, 5, 7, 9, ...)
- En el código: `kSize = getTrackbarPos("Kernel-Size", "Video") * 2 + 1`
  - Trackbar value 0 → kSize = 1
  - Trackbar value 1 → kSize = 3
  - Trackbar value 2 → kSize = 5

#### Algoritmo (ejemplo kernel 3×3)

Para píxel central:

```bash
Vecindad:     Ordenados:    Mediana:
[5, 1, 9]     [1, 3, 5]
[3, 8, 4]  →  [4, 5, 7]  →  5 (elemento central)
[7, 2, 6]     [8, 9, 6]
```

Resultado: El píxel toma el valor 5 (mediana de los 9 valores)

#### Ventajas del Filtro de Mediana

✅ **Excelente para ruido sal y pimienta** (píxeles blancos/negros aleatorios)
✅ **Preserva bordes** mejor que otros filtros de suavizado
✅ **No lineal** (a diferencia de Gaussiano)
✅ **Simple de implementar**

#### Comparación con Otros Filtros

| Filtro | Método | Mejor para | Preserva Bordes |
|--------|--------|------------|-----------------|
| **Mediana** | Valor central ordenado | Ruido sal y pimienta | ✅ Excelente |
| **Gaussiano** | Promedio ponderado | Ruido gaussiano | ⚠️ Regular |
| **Promedio** | Promedio simple | Ruido general | ❌ Pobre |
| **Bilateral** | Promedio ponderado por similitud | Suavizar preservando bordes | ✅ Excelente |

### 5. Trackbars Interactivos

#### Creación de Trackbars

```cpp
createTrackbar("T-M", "Video", nullptr, 255, eventoTrack);
createTrackbar("Kernel-Size", "Video", nullptr, 255, eventoTrack);
```

**Parámetros**:

- `"T-M"`: Nombre del trackbar (umbral medio)
- `"Video"`: Ventana donde se coloca
- `nullptr`: Puntero a variable (se obtiene con getTrackbarPos)
- `255`: Valor máximo
- `eventoTrack`: Función callback

#### Función Callback

```cpp
void eventoTrack(int v, void *data) {
    mT = getTrackbarPos("T-M", "Video");
    kSize = getTrackbarPos("Kernel-Size", "Video") * 2 + 1;
}
```

**Flujo**:

1. Usuario mueve trackbar
2. Se llama `eventoTrack()`
3. Variables globales se actualizan
4. El bucle principal usa los nuevos valores

## 💻 Análisis Detallado del Código

### Variables Globales

```cpp
int mT = 0;       // Umbral medio para función sigmoidal
int kSize = 1;    // Tamaño del kernel para filtro mediana
```

**¿Por qué globales?**

- Los trackbars necesitan acceso desde la función callback
- El bucle principal las lee en cada iteración

### Estructura del Bucle Principal

```cpp
while(true) {
    video >> frame;
    cvtColor(frame, frame, COLOR_BGR2GRAY);  // 1. Convertir a gris
    
    threshold(frame, binariaT, mT, 255, THRESH_BINARY_INV);  // 2. Umbral duro
    binariaCS = sigmoideo(frame);                             // 3. Umbral suave
    medianBlur(frame, mediana, kSize);                        // 4. Filtro mediana
    
    imshow("Video", frame);
    imshow("BinariaT", binariaT);
    imshow("BinariaCS", binariaCS);
    imshow("Mediana", mediana);
    
    if(waitKey(23) == 27) break;  // ESC para salir
}
```

### Orden de Procesamiento

```bash
Frame RGB
    ↓
Convertir a Escala de Grises
    ↓
    ├─→ threshold() → BinariaT (dura)
    ├─→ sigmoideo() → BinariaCS (suave)
    └─→ medianBlur() → Mediana (filtrada)
```

## 💻 Compilación y Ejecución

```bash
make
./main
```

**Controles**:

- **Trackbar T-M**: Ajusta el umbral (0-255)
- **Trackbar Kernel-Size**: Ajusta tamaño del filtro (1-511 píxeles)
- **ESC**: Salir

## 📊 Ventanas del Programa

1. **Video**: Frame original en escala de grises con trackbars
2. **BinariaT**: Resultado de umbralización dura (`threshold()`)
3. **BinariaCS**: Resultado de umbralización sigmoidal (suave)
4. **Mediana**: Resultado de filtro de mediana

## 🎓 Ejercicios Propuestos

### Nivel Básico

1. Agrega un trackbar para ajustar el parámetro `k` de la función sigmoidal
2. Implementa `THRESH_OTSU` para cálculo automático del umbral
3. Muestra los valores actuales de los trackbars en la ventana

### Nivel Intermedio

1. Compara 4 métodos de filtrado en tiempo real (mediana, gaussiano, bilateral, promedio)
1. Implementa detección de bordes después de la binarización
1. Calcula y muestra el porcentaje de píxeles blancos vs negros

### Nivel Avanzado

1. Implementa umbralización adaptativa (diferentes umbrales por región)
1. Crea una función sigmoidal inversa (S invertida)
1. Optimiza el filtro de mediana usando algoritmos más rápidos (histogram-based)

## 🔬 Aplicaciones Prácticas

### 1. Procesamiento de Documentos

```cpp
// Binarización suave para evitar pérdida de trazos finos
Mat doc_binario = sigmoideo(documento, k=0.5);
```

### 2. Segmentación de Objetos

```cpp
// Umbral adaptativo según iluminación local
threshold(region, binaria, 0, 255, THRESH_OTSU);
```

### 3. Reducción de Ruido en Cámaras

```cpp
// Aplicar mediana antes de procesamiento
medianBlur(frame_ruidoso, frame_limpio, 5);
```

### 4. Detección de Características

```cpp
// Realzar bordes con binarización
Mat bordes_binarios = sigmoideo(imagen_bordes, k=2.0);
```

## 🧮 Análisis Matemático de la Sigmoidal

### Derivada de la Sigmoidal

$$
\frac{dS}{dx} = \frac{255 \cdot k \cdot e^{-k(x-m)}}{(1 + e^{-k(x-m)})^2}
$$

**Interpretación**:

- Máxima pendiente en x = m
- Pendiente depende de k
- Mayor k → pendiente más pronunciada

### Límites

$$
\lim_{x \to -\infty} S(x) = 0
$$
$$
\lim_{x \to +\infty} S(x) = 255
$$

## 📈 Análisis de Rendimiento

### Complejidad Temporal

| Operación | Complejidad | Tiempo (640×480) |
|-----------|-------------|------------------|
| `threshold()` | O(n) | ~1 ms |
| `sigmoideo()` | O(n) | ~5 ms (por exp()) |
| `medianBlur(k=3)` | O(n) | ~3 ms |
| `medianBlur(k=9)` | O(n) | ~12 ms |

n = número de píxeles

### Optimización de Sigmoidal

**Versión optimizada con LUT (Look-Up Table)**:

```cpp
// Pre-calcular valores
uchar lut[256];
for (int i = 0; i < 256; i++) {
    lut[i] = 255.0 / (1.0 + exp(-k * (i - mT)));
}

// Aplicar rápidamente
for (int i = 0; i < img.rows; i++) {
    for (int j = 0; j < img.cols; j++) {
        binaria.at<uchar>(i,j) = lut[img.at<uchar>(i,j)];
    }
}
```

**Mejora**: ~10x más rápido

## 🐛 Problemas Comunes

### Kernel Size debe ser impar

**Error**: `OpenCV Error: Assertion failed (ksize % 2 == 1)`
**Solución**: Asegúrate de usar `kSize * 2 + 1`

### Función Sigmoidal Muy Lenta

**Solución**: Implementar LUT (tabla de búsqueda pre-calculada)

### Resultados Muy Oscuros/Claros

**Solución**: Ajusta el umbral mT con el trackbar

## 📖 Referencias Técnicas

- [OpenCV - Thresholding](https://docs.opencv.org/4.x/d7/d4d/tutorial_py_thresholding.html)
- [Sigmoid Function - Wikipedia](https://en.wikipedia.org/wiki/Sigmoid_function)
- [OpenCV - Median Filter](https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html#ga564869aa33e58769b4469101aac458f9)
- [Adaptive Thresholding](https://docs.opencv.org/4.x/d7/d1b/group__imgproc__misc.html#ga72b913f352e4a1b1b397736707afcde3)

## 🚀 Proyectos Relacionados

- **p02_opencv**: Procesamiento fundamental (detección de bordes, umbralización básica)
- **p07_opencv**: Segmentación por color
- **p08_opencv**: Ecualización de histogramas

## 💡 Conceptos Clave para Recordar

1. **Umbralización dura**: Transición abrupta, binaria pura
2. **Sigmoidal**: Transición suave, preserva información
3. **Filtro de mediana**: Excelente para ruido sal y pimienta
4. **Trackbars**: Permiten ajuste interactivo en tiempo real
5. **kSize impar**: Requisito para filtros espaciales
