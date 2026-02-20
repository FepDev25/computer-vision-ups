# Proyecto 01: Carga y Visualización de Imágenes con OpenCV

## Descripción General

Este proyecto introduce los conceptos fundamentales de OpenCV en C++, enfocándose en la carga, validación y visualización de imágenes. Es el primer paso para entender cómo trabajar con datos visuales en visión por computador.

## Objetivos de Aprendizaje

- Comprender la estructura básica de un programa OpenCV
- Aprender a cargar imágenes desde archivos
- Manejar errores en la carga de imágenes
- Visualizar imágenes en ventanas
- Entender la clase `Mat` de OpenCV

## Conceptos Teóricos

### 1. La Clase Mat

`Mat` (Matrix) es la estructura de datos fundamental en OpenCV para representar imágenes:

- **Almacenamiento**: Guarda píxeles en memoria de forma continua
- **Multidimensional**: Puede representar imágenes 2D, 3D o más dimensiones
- **Tipado**: Soporta diferentes tipos de datos (8-bit, 16-bit, float, etc.)
- **Canales**: Puede tener 1 canal (gris) o múltiples canales (BGR, RGBA)

### 2. Espacios de Color

Por defecto, OpenCV carga imágenes en formato **BGR** (Blue, Green, Red):

- **B (Blue)**: Canal azul [0-255]
- **G (Green)**: Canal verde [0-255]
- **R (Red)**: Canal rojo [0-255]

**Nota importante**: A diferencia de otros frameworks que usan RGB, OpenCV usa BGR por razones históricas.

### 3. Ventanas en OpenCV

OpenCV proporciona funciones para crear y gestionar ventanas GUI:

- `namedWindow()`: Crea una ventana con nombre específico
- `imshow()`: Muestra una imagen en la ventana
- `waitKey()`: Espera una pulsación de tecla (0 = espera indefinida)
- `destroyAllWindows()`: Cierra todas las ventanas abiertas

## Análisis del Código

### Paso 1: Validación de Argumentos

```cpp
if (argc != 2){
    cout << "Se debe proporcionar la ruta de una imagen." << endl;
    return -1;
}
```

**¿Por qué?** Verifica que el usuario proporcione exactamente un argumento (la ruta de la imagen). `argc` cuenta el número de argumentos de línea de comandos.

### Paso 2: Carga de la Imagen

```cpp
Mat imagen = imread(argv[1]);
```

**Función `imread()`**:

- Lee la imagen desde el archivo especificado
- Devuelve un objeto `Mat`
- Si falla, devuelve una matriz vacía

### Paso 3: Verificación de Carga

```cpp
if (imagen.empty()) {
    cout << "No se pudo cargar la imagen." << endl;
    return -1;
}
```

**¿Por qué es importante?** Siempre debemos verificar si la carga fue exitosa antes de procesar la imagen. Posibles causas de fallo:

- Ruta incorrecta
- Formato no soportado
- Archivo corrupto
- Permisos insuficientes

### Paso 4: Visualización

```cpp
namedWindow("Imagen Cargada", WINDOW_AUTOSIZE);
imshow("Imagen Cargada", imagen);
waitKey(0);
```

**Proceso**:

1. `namedWindow()`: Crea una ventana con el nombre "Imagen Cargada"
   - `WINDOW_AUTOSIZE`: La ventana se ajusta al tamaño de la imagen
2. `imshow()`: Muestra la imagen en la ventana
3. `waitKey(0)`: Espera indefinidamente hasta que se presione una tecla

### Paso 5: Limpieza

```cpp
destroyAllWindows();
```

**Buena práctica**: Libera los recursos de las ventanas antes de terminar el programa.

## Compilación y Ejecución

### Opción 1: Usando Makefile

```bash
make
./mostrar_imagen ruta/a/tu/imagen.jpg
```

### Opción 2: Compilación Manual

```bash
g++ main.cpp -o mostrar_imagen `pkg-config --cflags --libs opencv4`
./mostrar_imagen ruta/a/tu/imagen.jpg
```

## Formatos de Imagen Soportados

OpenCV soporta múltiples formatos:

- **JPEG/JPG** (.jpg, .jpeg) - Comprimido con pérdida
- **PNG** (.png) - Comprimido sin pérdida, soporta transparencia
- **BMP** (.bmp) - Sin compresión
- **TIFF** (.tiff, .tif) - Alta calidad
- **WebP** (.webp) - Formato moderno de Google

## Casos de Uso

1. **Visualizador de imágenes simple**
2. **Validación de datos de entrada en pipelines de procesamiento**
3. **Debugging de algoritmos de visión por computador**
4. **Inspección manual de datasets de imágenes**

## Ejercicios Propuestos

1. Modifica el programa para mostrar las dimensiones de la imagen (ancho x alto)
2. Agrega la capacidad de procesar múltiples imágenes desde la línea de comandos
3. Implementa un temporizador que cierre automáticamente la ventana después de 5 segundos
4. Muestra información adicional como el número de canales y tipo de datos

## 🐛 Problemas Comunes

### Error: "No se pudo cargar la imagen"

**Soluciones**:

- Verifica que la ruta sea correcta (puede ser relativa o absoluta)
- Confirma que el formato de imagen esté soportado
- Revisa los permisos del archivo

### Error de Compilación con pkg-config

**Solución**:

```bash
# Verifica la instalación de OpenCV
pkg-config --modversion opencv4

# Si no está instalado, instala OpenCV:
sudo apt-get install libopencv-dev
```

## 📖 Referencias

- [Documentación oficial OpenCV - imread()](https://docs.opencv.org/4.x/d4/da8/group__imgcodecs.html#ga288b8b3da0892bd651fce07b3bbd3a56)
- [Documentación oficial OpenCV - Mat](https://docs.opencv.org/4.x/d3/d63/classcv_1_1Mat.html)
- [Tutorial OpenCV - Reading Images](https://docs.opencv.org/4.x/db/deb/tutorial_display_image.html)

## 🚀 Siguiente Paso

Continúa con **p02_opencv** donde aprenderás técnicas de procesamiento de imágenes como conversión a escala de grises, detección de bordes, y más operaciones avanzadas.
