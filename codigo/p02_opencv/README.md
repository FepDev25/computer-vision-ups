# Proyecto 02: Procesamiento Fundamental de Imágenes

## Descripción General

Este proyecto es una introducción completa a las técnicas fundamentales de procesamiento de imágenes digitales. Implementa 8 operaciones esenciales que todo desarrollador de visión por computador debe dominar.

## Objetivos de Aprendizaje

- Comprender las transformaciones de espacio de color
- Aplicar filtros de suavizado para reducción de ruido
- Implementar detección de bordes
- Realizar umbralización (thresholding)
- Detectar y analizar contornos
- Calcular y visualizar histogramas
- Redimensionar imágenes manteniendo proporciones

## Conceptos Teóricos Detallados

### 1. Conversión a Escala de Grises

#### ¿Qué es?

La escala de grises representa imágenes usando solo intensidades de luz (de negro a blanco), eliminando la información de color.

#### ¿Por qué convertir a gris?

- **Reducción de dimensionalidad**: De 3 canales (BGR) a 1 canal
- **Menor costo computacional**: Procesamiento más rápido
- **Simplificación de algoritmos**: Muchos algoritmos funcionan mejor en gris
- **Enfoque en estructura**: Resalta formas y texturas sobre el color

#### Fórmula de conversión

OpenCV usa la fórmula estándar ITU-R BT.601:

```bash
Gray = 0.299*R + 0.587*G + 0.114*B
```

**¿Por qué estos pesos?** El ojo humano es más sensible al verde, seguido del rojo y luego el azul.

### 2. Desenfoque Gaussiano

#### Fundamento Matemático

El desenfoque Gaussiano aplica una **convolución** con un kernel basado en la distribución gaussiana:

$$
G(x,y) = \frac{1}{2\pi\sigma^2} e^{-\frac{x^2+y^2}{2\sigma^2}}
$$

Donde:

- $\sigma$ es la desviación estándar (controla el grado de desenfoque)
- $(x,y)$ son las coordenadas relativas al centro del kernel

#### ¿Para qué sirve?

- **Reducción de ruido**: Elimina ruido de alta frecuencia
- **Pre-procesamiento**: Mejora la detección de bordes
- **Suavizado**: Crea transiciones más suaves entre píxeles

#### Parámetros en el código

```cpp
GaussianBlur(imagen_gris, imagen_desenfocada, Size(5, 5), 0);
```

- `Size(5, 5)`: Tamaño del kernel (debe ser impar)
- `0`: Sigma calculado automáticamente desde el tamaño del kernel

### 3. Detección de Bordes con Canny

#### ¿Qué es un borde?

Un borde es un cambio brusco en la intensidad de píxeles, indicando límites de objetos.

#### Algoritmo de Canny (5 pasos)

1. **Suavizado Gaussiano**: Reduce ruido
2. **Cálculo de gradientes**: Usa operador Sobel para encontrar dirección e intensidad
3. **Supresión no-máxima**: Adelgaza bordes a 1 píxel de grosor
4. **Umbralización con histéresis**: Usa dos umbrales (bajo y alto)
5. **Seguimiento de bordes**: Conecta bordes fuertes con débiles

#### Parámetros

```cpp
Canny(imagen_desenfocada, bordes, 50, 150);
```

- `50`: Umbral inferior (bordes débiles)
- `150`: Umbral superior (bordes fuertes)
- **Relación recomendada**: 1:2 o 1:3

#### ¿Por qué es el mejor detector de bordes?

- **Detección óptima**: Minimiza falsos positivos/negativos
- **Buena localización**: Bordes cercanos a la posición real
- **Respuesta única**: Un solo punto por borde

### 4. Umbralización (Thresholding)

#### Concepto

Convierte imágenes en escala de grises a imágenes binarias (blanco/negro).

#### Fórmula

$$
dst(x,y) = \begin{cases}
maxVal & \text{si } src(x,y) > threshold \\
0 & \text{en otro caso}
\end{cases}
$$

#### Tipos de Umbralización

- `THRESH_BINARY`: Binaria estándar
- `THRESH_BINARY_INV`: Binaria invertida
- `THRESH_TRUNC`: Truncamiento
- `THRESH_TOZERO`: A cero
- `THRESH_OTSU`: Automática (método de Otsu)

#### Aplicaciones

- Segmentación de objetos
- OCR (Reconocimiento de texto)
- Separación de primer plano/fondo

### 5. Detección de Contornos

#### ¿Qué es un contorno?

Una curva que conecta todos los puntos continuos de un borde que tienen el mismo color o intensidad.

#### Algoritmo

```cpp
findContours(bordes.clone(), contornos, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
```

**Modos de recuperación** (`RETR_*`):

- `RETR_EXTERNAL`: Solo contornos externos
- `RETR_LIST`: Todos los contornos, sin jerarquía
- `RETR_TREE`: Jerarquía completa
- `RETR_CCOMP`: Dos niveles de jerarquía

**Métodos de aproximación** (`CHAIN_APPROX_*`):

- `CHAIN_APPROX_NONE`: Todos los puntos del contorno
- `CHAIN_APPROX_SIMPLE`: Solo puntos de esquina (reduce memoria)

#### Dteccion de contornos: Aplicaciones

- Reconocimiento de formas
- Detección de objetos
- Análisis morfológico
- Conteo de objetos

### 6. Histogramas

#### Definición

Un histograma es una representación gráfica de la distribución de intensidades de píxeles en una imagen.

#### ¿Qué nos dice?

- **Brillo general**: Distribución desplazada a la derecha (brillante) o izquierda (oscura)
- **Contraste**: Rango de valores (estrecho = bajo contraste, amplio = alto contraste)
- **Exposición**: Información sobre sub/sobre-exposición

#### Cálculo

```cpp
calcHist(&imagen_gris, 1, 0, Mat(), histograma, 1, &histSize, &histRange);
```

Parámetros:

- `&imagen_gris`: Array de imágenes fuente
- `1`: Número de imágenes
- `0`: Canal a procesar
- `Mat()`: Máscara (vacía = toda la imagen)
- `histograma`: Salida
- `1`: Número de dimensiones del histograma
- `&histSize`: Número de bins (256 para 8-bit)
- `&histRange`: Rango de valores [0, 256)

#### Normalización

Ajusta los valores del histograma a un rango específico para visualización:

```cpp
normalize(histograma, histograma, 0, imagenHistograma.rows, NORM_MINMAX);
```

### 7. Redimensionamiento de Imágenes

#### Métodos de Interpolación

Cuando redimensionamos, necesitamos calcular valores de píxeles intermedios:

1. **INTER_NEAREST**: Vecino más cercano (rápido, menor calidad)
2. **INTER_LINEAR**: Bilineal (equilibrado) - **predeterminado**
3. **INTER_CUBIC**: Bicúbica (más lenta, mejor calidad)
4. **INTER_AREA**: Área (mejor para reducción de tamaño)
5. **INTER_LANCZOS4**: Lanczos (máxima calidad, más lenta)

#### Uso

```cpp
resize(imagen_original, imagen_redimensionada, Size(800, 600));
```

## 💻 Compilación y Ejecución

### Compilar

```bash
make
```

### Ejecutar

```bash
./main ruta/a/imagen.jpg
```

### Salida

El programa muestra 8 ventanas simultáneamente y guarda 7 imágenes procesadas:

- `resultado_gris.jpg`
- `desenfoque_gaussiano.jpg`
- `resultado_bordes.jpg`
- `resultado_binaria.jpg`
- `resultado_contornos.jpg`
- `resultado_histograma.jpg`
- `resultado_redimensionada.jpg`

## 🔍 Análisis del Flujo de Procesamiento

```bash
Imagen Original (BGR)
        ↓
Escala de Grises (1 canal)
        ↓
Desenfoque Gaussiano (reducción de ruido)
        ↓
Detección de Bordes (Canny)
        ↓
Detección de Contornos
```

## 📊 Casos de Uso Prácticos

### 1. Control de Calidad Industrial

- Detección de defectos en productos
- Medición de dimensiones
- Verificación de componentes

### 2. Procesamiento de Documentos

- Binarización para OCR
- Detección de bordes de páginas
- Corrección de perspectiva

### 3. Análisis Médico

- Segmentación de tejidos
- Detección de anomalías
- Análisis de rayos X

### 4. Visión Robótica

- Detección de objetos para manipulación
- Navegación basada en contornos
- Reconocimiento de formas

## 🎓 Ejercicios Propuestos

### Nivel Básico

1. Experimenta con diferentes tamaños de kernel en el desenfoque Gaussiano
2. Prueba diferentes umbrales en Canny (50-150 vs 100-200)
3. Compara THRESH_BINARY con THRESH_BINARY_INV

### Nivel Intermedio

1. Implementa la detección automática de umbrales usando el método de Otsu
2. Filtra contornos pequeños (ruido) usando `cv::contourArea()`
3. Calcula el área y perímetro de cada contorno detectado

### Nivel Avanzado

1. Implementa ecualización de histograma adaptativa (CLAHE)
2. Combina múltiples técnicas para segmentar un objeto específico
3. Crea un pipeline personalizado para mejorar imágenes de baja calidad

## 🐛 Problemas Comunes y Soluciones

### Muchos contornos detectados (ruido)

**Solución**: Aumenta el desenfoque Gaussiano o filtra por área mínima

### Bordes discontinuos

**Solución**: Reduce el umbral inferior en Canny o mejora el pre-procesamiento

### Histograma no visible

**Solución**: Verifica la normalización y el rango de visualización

## 📖 Referencias Técnicas

- [OpenCV - Image Filtering](https://docs.opencv.org/4.x/d4/d86/group__imgproc__filter.html)
- [OpenCV - Canny Edge Detection](https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html)
- [OpenCV - Contours](https://docs.opencv.org/4.x/d3/dc0/group__imgproc__shape.html)
- [Paper original Canny (1986)](https://ieeexplore.ieee.org/document/4767851)

## 🚀 Siguiente Paso

Continúa con **p03_opencv** para aplicar estos conceptos en detección de rostros y ojos en tiempo real usando clasificadores Haar Cascade.
