# Proyecto 07: Espacios de Color, Segmentación y Operaciones Bitwise

## 📋 Descripción General

Este proyecto explora espacios de color avanzados (HSV, LAB), segmentación por color usando trackbars interactivos, y operaciones bitwise para manipulación de imágenes con máscaras.

## 🎯 Objetivos de Aprendizaje

- Comprender espacios de color HSV y LAB
- Implementar segmentación de objetos por color
- Usar trackbars para ajuste interactivo de parámetros
- Dominar operaciones bitwise (AND, OR, NOT)
- Trabajar con máscaras binarias
- Crear composiciones de imágenes (lienzos)

## 📚 Conceptos Teóricos Fundamentales

### 1. Espacios de Color

#### BGR (Blue-Green-Red)

- **Predeterminado en OpenCV**
- Basado en componentes de color primarios
- Difícil para segmentación por color
- Sensible a iluminación

#### HSV (Hue-Saturation-Value)

```cpp
cvtColor(frame, hsv, COLOR_BGR2HSV);
```

**Componentes**:

| Canal | Nombre | Rango OpenCV | Significado |
|-------|--------|--------------|-------------|
| **H** | Hue (Matiz) | 0-180 | Color puro (rojo, verde, azul, etc.) |
| **S** | Saturation (Saturación) | 0-255 | Pureza del color (0=gris, 255=puro) |
| **V** | Value (Valor/Brillo) | 0-255 | Intensidad de luz (0=negro, 255=brillante) |

**¿Por qué H va de 0-180?**

- En teoría, Hue va de 0° a 360°
- OpenCV lo comprime a 0-180 para usar 8 bits
- $H_{OpenCV} = H_{grados} / 2$

**Ventajas de HSV**:
✅ Separa color de intensidad (iluminación)
✅ Excelente para segmentación por color
✅ Robusto a cambios de iluminación
✅ Intuitivo para humanos

**Mapa de Colores en HSV**:

```bash
H = 0-10, 170-180:  Rojo
H = 10-25:          Naranja
H = 25-35:          Amarillo
H = 35-85:          Verde
H = 85-125:         Azul
H = 125-155:        Violeta
H = 155-170:        Magenta
```

#### LAB (Lightness-A-B)

```cpp
cvtColor(frame, lab, COLOR_BGR2Lab);
```

**Componentes**:

| Canal | Nombre | Rango OpenCV | Significado |
|-------|--------|--------------|-------------|
| **L** | Lightness (Luminosidad) | 0-255 | Brillo (0=negro, 255=blanco) |
| **A** | Canal A | 0-255 | Verde (-) ↔ Magenta (+) |
| **B** | Canal B | 0-255 | Azul (-) ↔ Amarillo (+) |

**Ventajas de LAB**:
✅ Percepción uniforme del color
✅ Separación completa luz-color
✅ Usado en industria gráfica
✅ Mejor para corrección de color

**Uso en el Código**:

```cpp
inRange(lab, Scalar(lmin, aMin, bMin), Scalar(lmax, aMax, bMax), binaria);
```

### 2. Segmentación por Color - inRange()

#### Función inRange()

Crea una máscara binaria donde los píxeles dentro del rango son blancos (255) y fuera son negros (0).

```cpp
inRange(imagen_fuente, limite_inferior, limite_superior, imagen_destino);
```

**Matemáticamente**:

$$
dst(x,y) = \begin{cases}
255 & \text{si } lower \leq src(x,y) \leq upper \\
0 & \text{en otro caso}
\end{cases}
$$

**Ejemplo del código**:

```cpp
inRange(lab, Scalar(lmin, aMin, bMin), Scalar(lmax, aMax, bMax), binaria);
```

Para cada píxel, verifica:

- `L` está entre `lmin` y `lmax`?
- `A` está entre `aMin` y `aMax`?
- `B` está entre `bMin` y `bMax`?

Si TODAS las condiciones son verdaderas → Píxel BLANCO
Si alguna es falsa → Píxel NEGRO

### 3. Trackbars Interactivos

#### Creación de Trackbars

```cpp
createTrackbar("L min", "Video", &lmin, 255, eventoTrackbar, NULL);
```

**Parámetros**:

- `"L min"`: Nombre del trackbar (se muestra en la UI)
- `"Video"`: Ventana donde se coloca
- `&lmin`: Puntero a la variable que se actualiza
- `255`: Valor máximo
- `eventoTrackbar`: Función callback (puede ser NULL)
- `NULL`: Datos de usuario opcionales

#### Función Callback

```cpp
void eventoTrackbar(int v, void* /*data*/) {
    cout << "Valor trackbar: " << v << " L min: " << lmin << endl;
}
```

**Flujo**:

1. Usuario mueve el trackbar
2. Variable asociada se actualiza automáticamente
3. Se llama la función callback (opcional)
4. El bucle principal usa el nuevo valor

### 4. Operaciones Bitwise

#### AND - Intersección

```cpp
bitwise_and(frame, frame, imagenAnd, mascaraM);
```

**Tabla de verdad**:

```bash
Píxel Frame  Píxel Máscara  Resultado
    255          255           255
    255           0             0
     0           255            0
     0            0             0
```

**Uso**: Extraer solo la región de interés (ROI) definida por la máscara.

#### OR - Unión

```cpp
bitwise_or(frame, frame, imagenOr, mascaraM);
```

**Tabla de verdad**:

```bash
Píxel Frame  Píxel Máscara  Resultado
    255          255           255
    255           0            255
     0           255           255
     0            0             0
```

**Uso**: Combinar múltiples máscaras o regiones.

#### NOT - Inversión

```cpp
bitwise_not(frame, negativo);
```

**Efecto**: Invierte todos los bits

- Negro (0) → Blanco (255)
- Blanco (255) → Negro (0)
- Colores se invierten (complementario)

**Resultado visual**: Efecto "negativo fotográfico"

### 5. Máscaras en OpenCV

Una máscara es una imagen binaria (blanco/negro) que define qué píxeles procesar.

#### Creación de Máscara Circular (en el código)

```cpp
Mat mascaraM = Mat::zeros(Size(frameWidth, frameHeight), CV_8UC1);
circle(mascaraM, Point(frameWidth/2, frameHeight/2), 73, Scalar(255), -1);
```

**Parámetros de circle()**:

- `mascaraM`: Imagen destino
- `Point(x, y)`: Centro del círculo
- `73`: Radio en píxeles
- `Scalar(255)`: Color blanco
- `-1`: Relleno completo (>0 = solo borde)

**Uso de la máscara**:

```cpp
bitwise_and(frame, frame, imagenAnd, mascaraM);
```

Solo los píxeles dentro del círculo blanco se mantienen, el resto se vuelve negro.

### 6. Composición de Imágenes - Lienzos

#### Creación de Lienzo

```cpp
Mat lienzo = Mat::zeros(Size(frameWidth * 2, frameHeight), CV_8UC3);
```

**Dimensiones**: Doble ancho para poner dos imágenes lado a lado

#### Copia de Región (ROI)

```cpp
tmp.copyTo(lienzo(Rect(frameWidth, 0, frameWidth, frameHeight)));
```

**Rect(x, y, ancho, alto)**:

- `x`: Posición X de inicio
- `y`: Posición Y de inicio
- `frameWidth`: Ancho de la región
- `frameHeight`: Alto de la región

**Resultado**: Imagen copiada en la mitad derecha del lienzo

## 💻 Análisis Detallado del Código

### Estructura del Programa

```bash
1. Abrir cámara
2. Crear ventanas
3. Crear 6 trackbars (L, A, B min/max)
4. Obtener dimensiones del video
5. Crear máscara circular
6. Bucle principal:
   ├─ Capturar frame
   ├─ Convertir a HSV
   ├─ Convertir a LAB
   ├─ Segmentar por color (inRange)
   ├─ Aplicar operaciones bitwise
   ├─ Crear lienzo comparativo
   └─ Mostrar 9 ventanas diferentes
```

### Ventanas Mostradas

1. **Video**: Frame original con trackbars
2. **HSV**: Imagen en espacio HSV
3. **LAB**: Imagen en espacio LAB
4. **BINARIA**: Resultado de segmentación
5. **MASCARA**: Máscara circular
6. **AND**: Operación AND con máscara
7. **OR**: Operación OR con máscara
8. **NEGATIVO**: Inversión de colores
9. **Lienzo**: Comparación binaria/segmentada

### Flujo de Segmentación

```bash
Frame BGR
    ↓
Convertir a LAB
    ↓
inRange(L∈[lmin,lmax], A∈[aMin,aMax], B∈[bMin,bMax])
    ↓
Imagen Binaria (Máscara)
    ↓
bitwise_and con frame original
    ↓
Solo objetos del color deseado
```

## 🎨 Casos de Uso Prácticos

### 1. Detección de Objetos por Color

Ejemplo: Detectar objetos rojos

```cpp
// En HSV (rojo está en dos rangos)
inRange(hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), mask1);
inRange(hsv, Scalar(170, 100, 100), Scalar(180, 255, 255), mask2);
bitwise_or(mask1, mask2, final_mask);
```

### 2. Chroma Key (Pantalla Verde)

```cpp
// Detectar verde
inRange(hsv, Scalar(35, 50, 50), Scalar(85, 255, 255), green_mask);
bitwise_not(green_mask, inverse_mask);
bitwise_and(foreground, foreground, result, inverse_mask);
// Agregar background donde estaba el verde
```

### 3. Seguimiento de Objetos

```cpp
inRange(hsv, lower_color, upper_color, mask);
findContours(mask, contours, ...);
// Encontrar el contorno más grande → objeto a seguir
```

## 💻 Compilación y Ejecución

```bash
make
./main
```

**Controles**:

- **Trackbars**: Ajusta los rangos de L, A, B
- **ESC**: Salir

## 🎓 Ejercicios Propuestos

### Nivel Básico

1. Encuentra los valores LAB para detectar solo objetos azules
2. Crea una máscara rectangular en lugar de circular
3. Invierte la máscara (blanco fuera, negro dentro del círculo)

### Nivel Intermedio

1. Implementa detección de múltiples colores simultáneamente
1. Guarda los valores de trackbar óptimos en un archivo JSON
1. Calcula el porcentaje de pantalla ocupado por el color segmentado

### Nivel Avanzado

1. Implementa un sistema de seguimiento de objetos por color con rectángulo delimitador
1. Crea un "pincel virtual" que dibuje en el aire detectando un objeto de color específico
1. Implementa un contador de objetos de diferentes colores en tiempo real

## 🔧 Optimización y Mejoras

### Reducir Ruido en Segmentación

```cpp
// Después de inRange()
Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
morphologyEx(binaria, binaria, MORPH_OPEN, kernel);   // Elimina ruido
morphologyEx(binaria, binaria, MORPH_CLOSE, kernel);  // Cierra huecos
```

### Aplicar Blur Antes de Segmentación

```cpp
GaussianBlur(lab, lab, Size(5, 5), 0);
inRange(lab, lower, upper, binaria);
```

## 🐛 Problemas Comunes

### Segmentación No Funciona

**Soluciones**:

- Verifica el espacio de color (HSV vs LAB)
- Ajusta los rangos con trackbars en tiempo real
- Considera la iluminación del ambiente
- Prueba con diferentes espacios de color

### Demasiadas Ventanas (consume mucha memoria)

**Solución**: Comenta las ventanas que no necesitas:

```cpp
// imshow("HSV", hsv);  // Desactivar
```

### Trackbars No Actualizan

**Solución**: Asegúrate de que las variables estén declaradas globalmente:

```cpp
int lmin = 0;  // Global, no dentro de main()
```

## 📖 Referencias Técnicas

- [OpenCV - Color Conversions](https://docs.opencv.org/4.x/de/d25/imgproc_color_conversions.html)
- [OpenCV - inRange](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#ga48af0ab51e36436c5d04340e036ce981)
- [OpenCV - Bitwise Operations](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#ga60b4d04b251ba5eb1392c34425497e14)
- [Color Spaces - Interactive Visualizer](https://colorizer.org/)

## 🚀 Siguiente Paso

Continúa con **p08_opencv** para aprender sobre histogramas avanzados, ecualización de histograma, CLAHE (Contrast Limited Adaptive Histogram Equalization) y procesamiento de canales de color.
