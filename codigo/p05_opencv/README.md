# Proyecto 05: Manipulación de Píxeles y Fundamentos de Imágenes Digitales

## 📋 Descripción General

Este proyecto explora los fundamentos de la manipulación de píxeles a nivel de bits, la creación de imágenes desde cero, y conceptos teóricos fundamentales sobre resolución, profundidad de color y conectividad de píxeles.

## 🎯 Objetivos de Aprendizaje

- Crear imágenes desde cero manipulando píxeles individualmente
- Comprender los diferentes tipos de imágenes (binarias, escala de grises, color)
- Dominar el acceso y modificación de píxeles usando `at<>()`
- Calcular tamaños de archivos de video sin compresión
- Entender conceptos de resolución y profundidad de color
- Trabajar con regiones de interés (ROI)

## 📚 Conceptos Teóricos Fundamentales

### 1. Tipos de Datos en OpenCV

#### CV_8UC1 - Imagen en Blanco y Negro / Escala de Grises

```cpp
Mat blancoNegro = Mat::zeros(Size(800, 600), CV_8UC1);
```

**Desglose del tipo**:

- `CV_`: Prefijo de OpenCV
- `8U`: 8 bits sin signo (unsigned) → valores de 0 a 255
- `C1`: 1 canal (monocromático)

**Valores**:

- `0` = Negro (ausencia total de luz)
- `255` = Blanco (intensidad máxima de luz)
- `1-254` = Tonos de gris

#### CV_8UC3 - Imagen a Color (BGR)

```cpp
Mat color = Mat(Size(800, 600), CV_8UC3, Scalar(255, 255, 255));
```

**Características**:

- `8U`: 8 bits por canal
- `C3`: 3 canales (Blue, Green, Red)
- Cada píxel = 3 bytes (24 bits totales)
- $2^{24}$ = 16,777,216 colores posibles

### 2. Acceso a Píxeles

#### Para Imágenes en Escala de Grises (1 canal)

```cpp
blancoNegro.at<uchar>(i, j) = 255;
```

- `uchar` = unsigned char (0-255)
- `(i, j)` = (fila, columna)

#### Para Imágenes a Color (3 canales)

```cpp
color.at<Vec3b>(i, j) = Vec3b(B, G, R);
```

- `Vec3b` = Vector de 3 bytes
- Orden BGR: `Vec3b(Blue, Green, Red)`

### 3. Creación de Imágenes

#### Inicialización con Ceros (Negro)

```cpp
Mat::zeros(Size(ancho, alto), tipo);
```

#### Inicialización con Valor Específico

```cpp
Mat gris = Mat(Size(800, 600), CV_8UC1, Scalar(255)); // Blanco
Mat color = Mat(Size(800, 600), CV_8UC3, Scalar(B, G, R));
```

### 4. Resolución y Profundidad de Color

#### Resolución

**Definición**: Cantidad de píxeles en una imagen

Ejemplo del código:

- Ancho: 800 píxeles
- Alto: 600 píxeles
- Resolución total: 800 × 600 = 480,000 píxeles

#### Profundidad de Color

**Definición**: Cantidad de bits usados para representar el color de cada píxel

Tipos comunes:

- **1 bit**: Binario (blanco/negro) → 2 colores
- **8 bits**: Escala de grises → 256 tonos
- **24 bits** (8+8+8): Color RGB → 16.7 millones de colores
- **32 bits** (8+8+8+8): RGBA con transparencia

### 5. Cálculo de Tamaño de Video (Ejercicio del Código)

**Problema planteado**:
Video de seguridad de 2 minutos:

- Resolución: 2048 × 2048
- 3 canales (color)
- Profundidad: 2 bits por canal
- FPS: 30 fotogramas por segundo
- Sin compresión

**Solución paso a paso**:

```bash
1. Tamaño por píxel:
   3 canales × 2 bits = 6 bits por píxel
   
2. Tamaño por imagen:
   2048 × 2048 píxeles × 6 bits = 25,165,824 bits
   = 3,145,728 bytes
   = ~3.15 MB por frame

3. Total de frames:
   2 minutos = 120 segundos
   120 segundos × 30 fps = 3,600 frames

4. Tamaño total:
   3,145,728 bytes/frame × 3,600 frames
   = 11,324,620,800 bytes
   = ~11.32 GB
```

⚠️ **Nota en el código**: Hay un error en el cálculo. Usa 2 bits cuando probablemente quiso decir 2 bytes (16 bits) por canal.

### 6. Conectividad de Píxeles

#### Vecindad 4-conectada (Cruz)

```bash
      [·]
   [·][P][·]
      [·]
```

Un píxel tiene 4 vecinos ortogonales (arriba, abajo, izquierda, derecha)

#### Vecindad 8-conectada (Completa)

```bash
   [·][·][·]
   [·][P][·]
   [·][·][·]
```

Un píxel tiene 8 vecinos (incluyendo diagonales)

**Aplicaciones**:

- **4-conectada**: Detección de componentes conectados estricta
- **8-conectada**: Etiquetado de regiones, detección de bordes

### 7. Regiones de Interés (ROI)

El código extrae el cuadrante superior izquierdo:

```cpp
int cx = imagen.cols / 2;  // Mitad del ancho
int cy = imagen.rows / 2;  // Mitad del alto

// Extraer píxeles manualmente
for (int i = 0; i < cy; i++) {
    for (int j = 0; j < cx; j++) {
        cuadrante.at<Vec3b>(i, j) = imagen.at<Vec3b>(i, j);
    }
}
```

**Forma más eficiente usando ROI**:

```cpp
Mat cuadrante = imagen(Rect(0, 0, cx, cy)).clone();
```

## 💻 Análisis del Código

### Ejercicio 1: Diagonal Blanca sobre Fondo Negro

```cpp
Mat blancoNegro = Mat::zeros(Size(800, 600), CV_8UC1);
for (int i = 0; i < blancoNegro.rows; i++) {
    for (int j = 0; j < blancoNegro.cols; j++) {
        if (i == j) {
            blancoNegro.at<uchar>(i, j) = 255;
        }
    }
}
```

**Resultado**: Diagonal blanca desde (0,0) hasta (599,599)

**Concepto**: Solo se dibuja cuando fila = columna

### Ejercicio 2: Diagonal con Degradado de Grises

```cpp
Mat gris = Mat(Size(800, 600), CV_8UC1, Scalar(255));
for (int i = 0, cont = 0; i < gris.rows; i++) {
    for (int j = 0; j < gris.cols; j++) {
        if (i == j) {
            gris.at<uchar>(i, j) = cont;
            cont++;
            if (cont > 255) cont = 0;
        }
    }
}
```

**Resultado**: Diagonal que va de negro (0) a blanco (255) gradualmente

**Concepto**: Incremento secuencial de intensidad

### Ejercicio 3: Diagonal Amarilla (Color)

```cpp
Mat color = Mat(Size(800, 600), CV_8UC3, Scalar(255, 255, 255));
for (int i = 0, cont = 0; i < color.rows; i++) {
    for (int j = 0; j < color.cols; j++) {
        if (i == j) {
            color.at<Vec3b>(i, j) = Vec3b(0, 233, 233); // Amarillo
        }
    }
}
```

**Color BGR(0, 233, 233)**:

- Blue: 0 (sin azul)
- Green: 233 (verde casi máximo)
- Red: 233 (rojo casi máximo)
- Resultado: Amarillo brillante

### Ejercicio 4: Extracción de Cuadrante

Divide la imagen en 4 partes y extrae el superior izquierdo.

```bash
┌─────┬─────┐
│ ✓   │     │  ← Cuadrante extraído
├─────┼─────┤
│     │     │
└─────┴─────┘
```

## 🔍 Conceptos Avanzados

### Interacción Luz-Objetos

**3 Tipos de objetos según comportamiento con la luz**:

1. **Reflectivos**: Reflejan la luz (metales, espejos)
2. **Absorbentes**: Absorben la luz (objetos negros, mate)
3. **Translúcidos**: Dejan pasar parte de la luz (vidrio esmerilado, papel)

### Sensores de Imagen

1. **Sensor Simple**: Un solo píxel
2. **Sensor Lineal**: Línea de píxeles (escáneres)
3. **Sensor CCD/CMOS**: Matriz 2D de píxeles (cámaras)

### Cuantización

Proceso de convertir valores continuos de luz en valores discretos digitales

Ejemplo:

- Luz continua: 0.0 → 1.0
- Cuantizada (8 bits): 0 → 255
- Pérdida de información en el proceso

## 💻 Compilación y Ejecución

```bash
make
./main ruta/a/imagen.jpg
```

## 📊 Salida del Programa

El programa muestra 4 ventanas:

1. **Diagonal Blanco/Negro**: Línea blanca sobre fondo negro
2. **Diagonal Escala Grises**: Degradado de negro a blanco
3. **Diagonal Amarilla**: Línea amarilla sobre fondo blanco
4. **Cuadrante Superior Izquierdo**: ROI extraída de la imagen

Archivo guardado:

- `cuadrante_superior_izquierdo.jpg`

## 🎓 Ejercicios Propuestos

### Nivel Básico

1. Dibuja una diagonal secundaria (de esquina superior derecha a inferior izquierda)
2. Crea una imagen con un patrón de tablero de ajedrez
3. Genera una imagen con círculos concéntricos

### Nivel Intermedio

1. Extrae los 4 cuadrantes y combínalos en orden diferente
1. Crea un degradado horizontal de color (de rojo a azul)
1. Implementa un filtro que invierta los colores de una imagen (negativo)

### Nivel Avanzado

1. Calcula el tamaño de un video Full HD (1920x1080) de 1 hora a 60 fps sin compresión
1. Implementa una función que rote una imagen 90 grados usando solo manipulación de píxeles
1. Crea un efecto de mosaico dividiendo la imagen en bloques

## 🧮 Cálculos Útiles

### Tamaño de Imagen en Memoria

```bash
Tamaño (bytes) = ancho × alto × canales × bytes_por_canal
```

Ejemplos:

- **HD (1280×720, RGB, 8-bit)**: 1280 × 720 × 3 × 1 = 2.76 MB
- **4K (3840×2160, RGB, 8-bit)**: 3840 × 2160 × 3 × 1 = 24.88 MB

### Tasa de Datos de Video

```bash
Tasa (MB/s) = (ancho × alto × canales × bytes_por_canal × fps) / 1,048,576
```

Ejemplo Full HD @ 30fps:

```bash
(1920 × 1080 × 3 × 1 × 30) / 1,048,576 = 177.98 MB/s
```

## 📖 Referencias

- [OpenCV - Mat Class](https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html)
- [Digital Image Processing - Gonzalez & Woods]
- [OpenCV - Basic Operations on Images](https://docs.opencv.org/4.x/d3/df2/tutorial_py_basic_ops.html)

## 🚀 Siguiente Paso

Continúa con **p06_opencv** para aprender sobre procesamiento de video en tiempo real, conversión de espacios de color y detección de movimiento.
