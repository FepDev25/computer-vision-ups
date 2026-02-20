# Proyecto 08: Histogramas, Ecualización y Mejora de Contraste

## 📋 Descripción General

Este proyecto profundiza en el análisis y manipulación de histogramas de imágenes, implementando técnicas de ecualización para mejorar el contraste y la calidad visual de imágenes.

## 🎯 Objetivos de Aprendizaje

- Calcular y visualizar histogramas manualmente
- Comprender la ecualización de histograma
- Aplicar CLAHE (Contrast Limited Adaptive Histogram Equalization)
- Trabajar con canales de color independientes
- Exportar datos a archivos CSV
- Procesar video en tiempo real con mejora de contraste

## 📚 Conceptos Teóricos Fundamentales

### 1. ¿Qué es un Histograma?

#### Definición

Un histograma es una representación gráfica de la distribución de intensidades de píxeles en una imagen.

**Para imagen en escala de grises**:

- Eje X: Valores de intensidad (0-255)
- Eje Y: Frecuencia (número de píxeles con ese valor)

#### Cálculo Manual (implementado en el código)

```cpp
int *histo = new int[256]();  // Array de 256 posiciones (0-255)

for (int i = 0; i < imagen.rows; i++) {
    for (int j = 0; j < imagen.cols; j++) {
        int valor = imagen.at<uchar>(i, j);  // Obtener intensidad del píxel
        histo[valor]++;  // Incrementar contador para ese valor
    }
}
```

**Proceso**:

1. Crear array de 256 posiciones (una por cada intensidad posible)
2. Recorrer cada píxel de la imagen
3. Leer su valor de intensidad
4. Incrementar el contador en esa posición

**Resultado**: `histo[127]` contiene cuántos píxeles tienen intensidad 127

### 2. Interpretación de Histogramas

#### Histograma de Imagen Oscura

```bash
Frecuencia
    |   ███
    |   ███
    |   ███
    |   ███
    +─────────────────>
    0              255
```

**Características**:

- Concentración a la izquierda
- Pocos píxeles brillantes
- Imagen sub-expuesta

#### Histograma de Imagen Clara

```bash
Frecuencia
    |           ███
    |           ███
    |           ███
    |           ███
    +─────────────────>
    0              255
```

**Características**:

- Concentración a la derecha
- Pocos píxeles oscuros
- Imagen sobre-expuesta

#### Histograma de Imagen Bien Expuesta

```bash
Frecuencia
    |   ███████████
    |   ███████████
    |   ███████████
    |   ███████████
    +─────────────────>
    0              255
```

**Características**:

- Distribución uniforme
- Buen uso del rango dinámico
- Contraste óptimo

### 3. Visualización del Histograma

#### Encontrar el Valor Máximo

```cpp
int maximo = 0;
for (int i = 0; i < 256; i++) {
    if (histo[i] > maximo) {
        maximo = histo[i];
    }
}
```

**¿Por qué?** Para escalar el histograma y que quepa en la ventana de visualización.

#### Escalado

```cpp
double escala = ((double)768 / (double)maximo);
```

**Propósito**: Normalizar la altura máxima del histograma a 768 píxeles.

#### Dibujo de Líneas

```cpp
for (int i = 0; i < 256; i++) {
    int altura = static_cast<int>(histo[i] * escala);
    line(lienzo, 
         Point(i * 4, 768),           // Punto inferior
         Point(i * 4, 768 - altura),  // Punto superior
         Scalar(255, 255, 255), 2);   // Color y grosor
}
```

**Detalles**:

- `i * 4`: Multiplica por 4 para ancho de 1024 píxeles (256 * 4)
- `768`: Altura del lienzo
- `768 - altura`: Resta para invertir (origen en esquina superior)

### 4. Ecualización de Histograma

#### Concepto

La ecualización de histograma redistribuye las intensidades para usar todo el rango disponible (0-255), mejorando el contraste.

#### Función de OpenCV

```cpp
equalizeHist(imagen2, ecualizada);
```

#### Algoritmo (paso a paso)

- Calcular histograma

```bash
histo[i] = número de píxeles con valor i
```

- Calcular histograma acumulado

```bash
histoAcum[i] = histoAcum[i-1] + histo[i]
```

- Normalizar

$$
nuevoValor(i) = \frac{histoAcum[i]}{totalPixeles} \times 255
$$

Ejemplo visual:

```bash
Antes:           Después:
0-50:  ████      0-100:  ██
50-100: █        100-150: ██
100-150:█        150-200: ██
150-200:         200-255: ██
200-255:
```

#### Ventajas

✅ Mejora contraste automáticamente
✅ Sin parámetros que ajustar
✅ Rápido y eficiente

#### Desventajas

❌ Puede amplificar ruido
❌ Puede sobre-realzar regiones
❌ No es adaptativo (global)

### 5. CLAHE (Contrast Limited Adaptive Histogram Equalization)

#### ¿Qué es?

Una versión mejorada de la ecualización que:

1. Divide la imagen en tiles (regiones pequeñas)
2. Ecualiza cada región independientemente
3. Limita el contraste para evitar amplificación de ruido

#### Implementación en el Código

```cpp
Ptr<CLAHE> clahe = createCLAHE();
clahe->apply(imagen2, imagenCLAHE);
```

#### Parámetros Configurables

```cpp
Ptr<CLAHE> clahe = createCLAHE(clipLimit, tileGridSize);
```

- **clipLimit** (default: 40.0)
  - Límite de amplificación de contraste
  - Mayor valor = más contraste (pero más ruido)
  - Menor valor = menos amplificación

- **tileGridSize** (default: Size(8, 8))
  - Tamaño de las regiones
  - Tiles más pequeños = más adaptativo
  - Tiles más grandes = más suave

#### Comparación Visual

| Método | Uso Ideal | Resultado |
|--------|-----------|-----------|
| **Original** | Referencia | Puede tener bajo contraste |
| **equalizeHist()** | Imágenes oscuras/claras uniformes | Contraste global mejorado |
| **CLAHE** | Imágenes médicas, bajo contraste local | Detalles locales mejorados |

### 6. Procesamiento por Canales de Color

El código procesa video ecualizando solo el canal L de LAB:

```cpp
cvtColor(frame, imagenLab, COLOR_BGR2Lab);
split(imagenLab, canales);          // Separar en L, A, B
equalizeHist(canales[0], canales[0]); // Ecualizar solo L
merge(canales, ecualizada);         // Recombinar
cvtColor(ecualizada, ecualizada, COLOR_Lab2BGR);
```

**¿Por qué LAB?**

- **L**: Luminosidad (brillo)
- **A, B**: Componentes de color

Al ecualizar solo L:
✅ Mejora el brillo/contraste
✅ Preserva los colores originales
✅ Resultado más natural

**Alternativa en RGB** (no recomendada):

```cpp
// Ecualizar cada canal RGB por separado
// Resultado: distorsión de colores
```

## 💻 Análisis Detallado del Código

### Parte 1: Análisis de Imagen Estática

```cpp
1. Cargar imagen en escala de grises
2. Calcular histograma manualmente
3. Exportar a CSV
4. Encontrar valor máximo
5. Calcular escala de visualización
6. Dibujar histograma en lienzo
7. Aplicar equalizeHist()
8. Aplicar CLAHE
9. Mostrar resultados
```

### Parte 2: Procesamiento de Video

```cpp
while (true) {
    video >> frame;
    cvtColor(frame, imagenLab, COLOR_BGR2GRAY);  // ⚠️ Error en código
    split(imagenLab, canales);
    merge(canales, ecualizada);
    cvtColor(ecualizada, ecualizada, COLOR_Lab2BGR);
    
    imshow("Ecualizada", ecualizada);
    imshow("Video", frame);
}
```

**⚠️ Nota**: Hay un error en el código:

```cpp
// Incorrecto:
cvtColor(frame, imagenLab, COLOR_BGR2GRAY);

// Debería ser:
cvtColor(frame, imagenLab, COLOR_BGR2Lab);
```

### Exportación a CSV

```cpp
ofstream file("histograma.csv");
for (int i = 0; i < 256; i++) {
    file << i << " " << histo[i] << endl;
}
file.close();
```

**Formato del archivo**:

```bash
0 120
1 145
2 180
...
255 95
```

**Uso**: Análisis en Excel, Python, MATLAB, etc.

## 💻 Compilación y Ejecución

```bash
make
./main
```

**Requisitos**:

- Archivo de imagen: `im2.jpg` en el mismo directorio
- Cámara web (para la segunda parte)

## 📊 Salida del Programa

### Archivos Generados

- `histograma.csv`: Datos del histograma

### Ventanas Mostradas (Parte 1)

1. **Imagen Original**: Imagen en escala de grises
2. **Histograma**: Visualización del histograma
3. **Imagen Ecualizada**: Resultado de equalizeHist()
4. **Imagen CLAHE**: Resultado de CLAHE

### Ventanas Mostradas (Parte 2)

1. **Video**: Stream original de la cámara
2. **Ecualizada**: Video con ecualización

## 🎓 Ejercicios Propuestos

### Nivel Básico

1. Corrige el error en el procesamiento de video (COLOR_BGR2GRAY → COLOR_BGR2Lab)
2. Calcula y muestra el histograma de una imagen a color (RGB)
3. Agrega título y ejes al histograma visualizado

### Nivel Intermedio

1. Implementa la ecualización manual (sin usar equalizeHist)
1. Compara histogramas antes y después de ecualizar en la misma ventana
1. Configura CLAHE con parámetros personalizados usando trackbars

### Nivel Avanzado

1. Implementa ecualización adaptativa manual por regiones
1. Calcula y visualiza el histograma acumulado
1. Crea una función que determine automáticamente si una imagen necesita ecualización

## 🔬 Aplicaciones Prácticas

### 1. Imágenes Médicas

```cpp
// Rayos X, TAC, Resonancias
Ptr<CLAHE> clahe = createCLAHE(2.0, Size(8, 8));
clahe->apply(rayosX, mejorado);
```

### 2. Fotografía

```cpp
// Mejorar fotos subexpuestas
equalizeHist(foto_oscura_gris, mejorada);
```

### 3. Visión Nocturna

```cpp
// Mejorar visibilidad en condiciones de baja luz
Ptr<CLAHE> clahe = createCLAHE(4.0, Size(4, 4));
clahe->apply(imagen_nocturna, mejorada);
```

### 4. Preprocesamiento para OCR

```cpp
// Mejorar texto antes de reconocimiento
equalizeHist(documento, documento_mejorado);
threshold(documento_mejorado, binario, 0, 255, THRESH_OTSU);
```

## 📈 Métricas de Calidad

### Entropía de la Imagen

Mide la cantidad de información:

```cpp
double calcularEntropia(Mat& img) {
    int histo[256] = {0};
    // Calcular histograma...
    
    double entropia = 0;
    int total = img.rows * img.cols;
    for (int i = 0; i < 256; i++) {
        if (histo[i] > 0) {
            double p = (double)histo[i] / total;
            entropia -= p * log2(p);
        }
    }
    return entropia;
}
```

**Interpretación**:

- Entropía baja: Poca variación (imagen uniforme)
- Entropía alta: Mucha variación (imagen con detalles)

## 🐛 Problemas Comunes

### Imagen Muy Ruidosa Después de Ecualizar

**Solución**: Usar CLAHE con clipLimit bajo:

```cpp
Ptr<CLAHE> clahe = createCLAHE(2.0, Size(8, 8));
```

### Colores Distorsionados

**Solución**: Ecualizar solo el canal de luminosidad (L en LAB o V en HSV)

### Histograma No Se Ve

**Solución**: Verifica el factor de escala y el tamaño del lienzo

## 📖 Referencias Técnicas

- [OpenCV - Histogram Equalization](https://docs.opencv.org/4.x/d5/daf/tutorial_py_histogram_equalization.html)
- [CLAHE Paper - Zuiderveld, 1994](https://dl.acm.org/doi/10.1016/B978-0-12-336156-1.50061-6)
- [OpenCV - calcHist](https://docs.opencv.org/4.x/d6/dc7/group__imgproc__hist.html)

## 🚀 Siguiente Paso

Continúa con **p09_opencv** para aprender sobre umbralización avanzada, implementación de funciones sigmoideas para binarización, y filtros de mediana para reducción de ruido.
