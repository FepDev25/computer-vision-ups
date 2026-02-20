# Proyecto 06: Procesamiento de Video en Tiempo Real y Detección de Movimiento

## 📋 Descripción General

Este proyecto introduce el procesamiento de video en tiempo real desde una cámara web, implementando conversión personalizada a escala de grises y detección de movimiento mediante diferencia de frames.

## 🎯 Objetivos de Aprendizaje

- Capturar y procesar video en tiempo real desde una cámara web
- Implementar conversiones de espacio de color personalizadas
- Entender la diferencia entre frames para detectar movimiento
- Trabajar con el bucle principal de captura de video
- Controlar el flujo de video con FPS

## 📚 Conceptos Teóricos

### 1. Captura de Video en Tiempo Real

#### Clase VideoCapture

```cpp
VideoCapture video(0);  // 0 = cámara predeterminada
```

**Parámetros de dispositivo**:

- `0`: Cámara web principal
- `1`, `2`, ...: Cámaras adicionales
- `"ruta/video.mp4"`: Archivo de video

#### Verificación de Apertura

```cpp
if (!video.isOpened()) {
    // La cámara no está disponible
}
```

**Causas comunes de fallo**:

- Cámara en uso por otra aplicación
- Permisos insuficientes
- Driver no instalado
- Hardware desconectado

### 2. Conversión BGR a Escala de Grises (Implementación Manual)

#### Método Implementado en el Código

```cpp
Mat convertirGris(Mat img) {
    Mat gris = Mat::zeros(img.size(), CV_8UC1);
    Vec3b pixel;
    int pixelg;

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            pixel = img.at<Vec3b>(i, j);
            pixelg = (pixel[0] + pixel[1] + pixel[2]) / 3; // Promedio simple
            gris.at<uchar>(i, j) = pixelg;
        }
    }
    return gris;
}
```

**Análisis**:

- **Método usado**: Promedio aritmético simple
- **Fórmula**: $Gray = \frac{B + G + R}{3}$

#### Comparación de Métodos

| Método | Fórmula | Ventajas | Desventajas |
|--------|---------|----------|-------------|
| **Promedio Simple** | $(B + G + R) / 3$ | Simple, rápido | No considera percepción humana |
| **Promedio Ponderado (ITU-R BT.601)** | $0.299R + 0.587G + 0.114B$ | Perceptualmente correcto | Ligeramente más lento |
| **Desaturación** | $(max(R,G,B) + min(R,G,B)) / 2$ | Preserva luminosidad | Puede perder contraste |
| **Luminosidad** | $max(R, G, B)$ | Muy rápido | Pierde mucha información |

**¿Por qué pesos diferentes?**
El ojo humano tiene diferente sensibilidad a cada color:

- **Verde**: 58.7% (máxima sensibilidad)
- **Rojo**: 29.9%
- **Azul**: 11.4% (mínima sensibilidad)

### 3. Detección de Movimiento por Diferencia de Frames

#### Concepto Fundamental

La detección de movimiento se basa en comparar el frame actual con el anterior. Los píxeles que cambian indican movimiento.

#### Implementación en el Código

```cpp
Mat anterior;
Mat resta;

// En el bucle:
if (anterior.empty()) {
    anterior = gris.clone();
}

absdiff(gris, anterior, resta);  // Diferencia absoluta
anterior = gris.clone();         // Actualizar frame anterior
```

#### Función `absdiff()`

Calcula la diferencia absoluta píxel por píxel:

$$
resta(x,y) = |gris(x,y) - anterior(x,y)|
$$

**Resultado**:

- **Píxeles blancos** (valor alto): Cambio significativo → Movimiento
- **Píxeles negros** (valor bajo): Sin cambio → Fondo estático

#### Ventajas de este Método

✅ Simple y eficiente computacionalmente
✅ Funciona bien para movimientos obvios
✅ No requiere calibración compleja

#### Limitaciones

❌ Sensible a cambios de iluminación
❌ Ruido de la cámara puede generar falsos positivos
❌ No distingue entre tipos de movimiento

### 4. Procesamiento de Video - Bucle Principal

```cpp
while (true) {
    video >> frame;                    // Capturar frame
    flip(frame, frame, 1);            // Espejo horizontal
    
    gris = convertirGris(frame);      // Procesar
    
    imshow("Cámara Web", frame);      // Mostrar
    
    if (waitKey(23) == 27) {          // Control
        break;
    }
}
```

#### Componentes del Bucle

1. **Captura**: `video >> frame`
   - Operador `>>` sobrecargado para captura
   - Equivalente a `video.read(frame)`

2. **Volteo Horizontal**: `flip(frame, frame, 1)`
   - Parámetro `1`: Volteo horizontal (efecto espejo)
   - Parámetro `0`: Volteo vertical
   - Parámetro `-1`: Ambos ejes

3. **Procesamiento**: Conversiones y análisis

4. **Visualización**: `imshow()`

5. **Control de FPS**: `waitKey(23)`
   - 23 ms ≈ 43 FPS
   - Fórmula: $FPS = \frac{1000}{delay\_ms}$

### 5. Control de Tiempo y FPS

#### ¿Por qué waitKey(23)?

```bash
FPS = 1000 / 23 ≈ 43.5 frames por segundo
```

**Valores comunes**:

- `waitKey(1)` → ~1000 FPS (limitado por hardware)
- `waitKey(16)` → ~62.5 FPS (suave para pantalla 60Hz)
- `waitKey(33)` → ~30 FPS (video estándar)
- `waitKey(40)` → 25 FPS (PAL video)

#### Detección de Teclas

```cpp
if (waitKey(23) == 27) {  // 27 = código ASCII de ESC
    break;
}
```

### 6. Conversión de Espacios de Color

#### De Gris a BGR (para visualización)

```cpp
cvtColor(gris, grisO, COLOR_GRAY2BGR);
```

**¿Por qué?** Para mostrar imágenes en escala de grises junto con imágenes en color en ventanas similares.

Resultado

- Entrada: 1 canal (gris)
- Salida: 3 canales (B=G=R) → gris visual pero con estructura BGR

## 💻 Análisis Detallado del Código

### Estructura del Programa

```bash
Inicio
  ↓
Abrir Cámara
  ↓
Crear Ventanas
  ↓
┌─────────────────┐
│  Bucle Infinito │
│                 │
│  1. Capturar    │
│  2. Voltear     │
│  3. Convertir   │
│  4. Detectar    │
│  5. Mostrar     │
│  6. Esperar     │
└─────────────────┘
  ↓
Liberar Recursos
  ↓
Fin
```

### Ventanas Creadas

1. **Cámara Web**: Frame original en color
2. **Cámara Web - Gris**: Conversión personalizada (1 canal)
3. **Cámara Web - Gris (Color)**: Gris visualizado como BGR
4. **Cámara Web - Diferencia**: Detección de movimiento

### Optimizaciones Posibles

#### 1. Evitar Copia Innecesaria

```cpp
// Original:
grisO = gris convertido a BGR

// Optimizado:
// Solo crear si realmente se necesita mostrar
```

#### 2. Procesamiento Paralelo

```cpp
// Usar hilos para procesamiento independiente
// thread t1([&]{ procesar_conversión(); });
// thread t2([&]{ procesar_diferencia(); });
```

#### 3. ROI para Procesamiento Selectivo

```cpp
// Solo procesar región central
Rect roi(frame.cols/4, frame.rows/4, frame.cols/2, frame.rows/2);
Mat frame_roi = frame(roi);
```

## 💻 Compilación y Ejecución

```bash
make
./main
```

**Controles**:

- **ESC**: Salir del programa
- Las ventanas se actualizan automáticamente en tiempo real

## 📊 Análisis de Rendimiento

### Consumo de Recursos

Para resolución VGA (640×480):

```bash
Frame original (BGR):     640 × 480 × 3 = 921,600 bytes ≈ 900 KB
Frame gris (1 canal):     640 × 480 × 1 = 307,200 bytes ≈ 300 KB
Frame diferencia:         640 × 480 × 1 = 307,200 bytes ≈ 300 KB

Total por iteración:      ~1.5 MB
A 30 FPS:                 ~45 MB/s
```

## 🎓 Ejercicios Propuestos

### Nivel Básico

1. Modifica el delay para lograr exactamente 30 FPS
2. Agrega un contador de FPS real usando `getTickCount()`
3. Cambia la conversión a gris para usar la fórmula ponderada correcta

### Nivel Intermedio

1. Implementa umbralización en la imagen de diferencia para resaltar solo movimientos significativos
1. Calcula el porcentaje de área en movimiento
1. Graba el video procesado a un archivo usando `VideoWriter`

### Nivel Avanzado

1. Implementa un sistema de alarma que detecte movimiento y guarde automáticamente frames
1. Crea un "efecto fantasma" mostrando el promedio de los últimos N frames
1. Implementa detección de movimiento mejorada con sustracción de fondo (Background Subtraction)

## 🔍 Aplicaciones Prácticas

### 1. Vigilancia y Seguridad

- Sistemas de alarma por movimiento
- Monitoreo de áreas restringidas
- Detección de intrusos

### 2. Análisis de Tráfico

- Conteo de vehículos
- Detección de infracciones
- Análisis de flujo vehicular

### 3. Deportes y Biomecánica

- Análisis de movimiento de atletas
- Captura de movimiento simple
- Estudios de postura

### 4. Interacción Humano-Computadora

- Gestos básicos para control
- Detección de presencia
- Activación por movimiento

## 🐛 Problemas Comunes

### Error: "No se pudo abrir la cámara"

**Soluciones**:

```bash
# Linux: Verificar permisos
ls -l /dev/video0
sudo usermod -a -G video $USER

# Verificar dispositivos disponibles
v4l2-ctl --list-devices
```

### Imagen con Ruido en la Diferencia

**Solución**: Aplicar filtro de mediana antes de calcular diferencia:

```cpp
medianBlur(gris, gris, 5);
```

### FPS Bajo

**Soluciones**:

- Reducir resolución de captura
- Procesar solo cada N frames
- Optimizar algoritmo de conversión

## 📖 Referencias

- [OpenCV - VideoCapture Class](https://docs.opencv.org/4.x/d8/dfe/classcv_1_1VideoCapture.html)
- [OpenCV - Motion Analysis](https://docs.opencv.org/4.x/d7/df3/group__imgproc__motion.html)
- [Background Subtraction Tutorial](https://docs.opencv.org/4.x/d1/dc5/tutorial_background_subtraction.html)

## 🚀 Siguiente Paso

Continúa con **p07_opencv** para aprender sobre espacios de color avanzados (HSV, LAB), operaciones bitwise, trackbars interactivos y segmentación por color.
