# Proyecto 15 - Local Binary Patterns (LBP)

Este proyecto implementa y demuestra el uso de la librería LBP (Local Binary Patterns) para procesamiento de imágenes.

## Descripción

Los **Local Binary Patterns (LBP)** son un tipo de descriptor visual utilizado para clasificación en visión por computador. LBP fue introducido por Ojala et al. en 2002 y ha sido ampliamente utilizado en:

- Reconocimiento facial
- Detección de texturas
- Segmentación de objetos
- Sustracción de fondo
- Análisis de imágenes médicas

## Características del Proyecto

Este proyecto incluye:

1. **Procesamiento de imágenes estáticas**: Carga una imagen y aplica algoritmos LBP
2. **Procesamiento en tiempo real**: Usa la cámara web para aplicar LBP en vivo
3. **Comparación de algoritmos**: Compara visualmente diferentes variantes de LBP
4. **Visualización de histogramas**: Muestra el histograma del patrón LBP resultante

## Algoritmos LBP Implementados

La librería incluye 11 algoritmos diferentes:

1. **OLBP** (Original LBP) - Ojala et al. (2001)
2. **ELBP** (Extended LBP) - Mdakane and Bergh (2012)
3. **VARLBP** (Variance-based LBP) - Ojala et al. (2002)
4. **CSLBP** (Center-Symmetric LBP) - Heikkilä et al. (2006)
5. **CSLDP** (Center-Symmetric Local Derivative Pattern) - Xue et al. (2011)
6. **XCSLBP** (eXtended Center-Symmetric LBP) - Silva et al. (2015)
7. **SILTP** (Scale Invariant Local Ternary Pattern) - Liao et al. (2010)
8. **CSSILTP** (Center-Symmetric Scale Invariant LTP) - Wu et al. (2013)
9. **SCSLBP** (Spatial Center-Symmetric LBP) - Xue et al. (2010)
10. **BGLBP** (BackGround LBP) - Davarpanah et al. (2015)
11. **OCLBP** (Opponent Color LBP) - Maenpaa and Pietikainen (2004)

## Compilación

```bash
make
```

## Ejecución

```bash
make run
# o directamente
./main
```

## Uso

Al ejecutar el programa, verás un menú con las siguientes opciones:

### Opción 1: Procesar imagen desde archivo

1. Selecciona esta opción
2. Ingresa la ruta completa de una imagen (ej: `/ruta/a/tu/imagen.jpg`)
3. Selecciona el algoritmo LBP que deseas usar (1-4)
4. Observa los resultados:
   - Imagen original en color
   - Imagen en escala de grises
   - Resultado del algoritmo LBP
   - Histograma del patrón LBP

### Opción 2: Procesar desde cámara web

1. Selecciona esta opción
2. La cámara web se activará
3. Presiona las teclas numéricas para cambiar de algoritmo:
   - `1`: OLBP
   - `2`: ELBP
   - `3`: CSLBP
   - `4`: XCSLBP
4. Presiona `q` o `ESC` para salir

### Opción 3: Comparar múltiples algoritmos

1. Selecciona esta opción
2. Ingresa la ruta de una imagen
3. El programa mostrará 4 ventanas con diferentes algoritmos aplicados a la misma imagen
4. Compara visualmente las diferencias entre los métodos

## Dependencias

- **OpenCV 4.x** (o 2.4.x/3.4.x)
- **C++11** o superior
- **Librería LBP** (incluida en `segundo-interciclo/local_binnary_patterns/lbp-library`)

## Estructura del Proyecto

```
p15_lbp/
├── main.cpp          # Código principal
├── Makefile          # Archivo de compilación
└── README.md         # Esta documentación
```

## Teoría: ¿Qué es LBP?

**Local Binary Pattern** es un operador de textura simple pero efectivo que etiqueta los píxeles de una imagen comparando cada píxel con sus vecinos:

1. Para cada píxel, se compara su valor con los 8 píxeles vecinos
2. Si el vecino es mayor o igual, se marca como 1; si es menor, como 0
3. Esto genera un número binario de 8 bits (0-255)
4. El resultado es una imagen donde cada píxel representa el patrón binario local

### Ventajas de LBP

- **Invariante a cambios de iluminación monotónicos**: Solo compara valores relativos
- **Computacionalmente simple**: Operaciones rápidas de comparación
- **Robusto**: Funciona bien en diversas condiciones
- **Versátil**: Múltiples variantes para diferentes aplicaciones

## Ejemplos de Uso

### Análisis de Texturas
```bash
# Analiza imágenes con diferentes texturas
./main
# Selecciona opción 3 y carga una imagen con textura visible
```

### Procesamiento en Tiempo Real
```bash
# Usa tu cámara web
./main
# Selecciona opción 2 y experimenta con diferentes algoritmos
```

## Limpieza

```bash
make clean
```

## Referencias

- Ojala, T., Pietikäinen, M., & Harwood, D. (2001). A comparative study of texture measures with classification based on featured distributions. Pattern recognition, 29(1), 51-59.
- Librería LBP: https://github.com/carolinepacheco/lbplibrary

## Notas

- Si usas imágenes propias, asegúrate de que la ruta sea correcta
- Para mejor rendimiento en cámara web, el tamaño se redimensiona a 640x480
- Los histogramas muestran la distribución de los patrones binarios locales
- Cada algoritmo tiene características específicas para diferentes aplicaciones

## Autor

Proyecto de demostración para el curso de Visión por Computador
