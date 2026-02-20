# Análisis Comparativo de Descriptores de Forma y Clasificación en Tiempo Real

Este repositorio contiene el desarrollo experimental para la evaluación de descriptores globales en el reconocimiento de formas geométricas. El proyecto se centra en comparar la robustez de diferentes métodos de extracción de características ante transformaciones geométricas y ruido, así como implementar un sistema de clasificación en tiempo real mediante firmas espectrales.

El trabajo se divide en dos partes principales que abarcan desde el análisis estático de descriptores clásicos hasta la implementación de un sistema de reconocimiento eficiente para dispositivos móviles.

## Estructura del Proyecto

### Parte 1: Análisis de Robustez (Momentos de Hu vs. Zernike)

Evaluación comparativa de descriptores globales mediante notebook en Kaggle para determinar cuál presenta mayor invarianza ante rotaciones, escalas y diferentes niveles de ruido sintético.

- **Dataset:** UPS-Writing-Skills (círculos, triángulos, rectángulos).
- **Descriptores:** Momentos de Hu (7 momentos) vs. Momentos de Zernike (grado n=8).
- **Tecnología:** Python, OpenCV, scikit-learn, Kaggle Notebooks.
- **Análisis:** Inyección controlada de ruido Gaussiano y Salt & Pepper, clasificación con KNN/SVM, matrices de confusión por nivel de degradación.

### Parte 2: Sistema de Reconocimiento en Tiempo Real (Shape Signature)

Implementación de un clasificador de formas basado en la Transformada de Fourier de coordenadas complejas, diseñado para operar en tiempo real en dispositivos móviles.

- **Metodología:** Representación del contorno como señal compleja s(n) = (x(n) - xc) + j(y(n) - yc).
- **Pipeline:** Extracción de contornos, interpolación a 1024 puntos, FFT, normalización por el primer armónico.
- **Clasificación:** Comparación por distancia euclídea contra corpus de entrenamiento.
- **Tecnología:** C++, OpenCV 4.x, CMake, Android NDK (JNI).

---

## Requisitos del Sistema

- **Hardware:**
  - CPU: Intel Core i5 o superior (o equivalente ARM para Android).
  - Dispositivo Android con soporte para aplicaciones nativas (para la variante móvil).
- **Software:**
  - Python 3.10+ (para Parte 1).
  - C++ Compilador compatible con C++17.
  - OpenCV 4.x (con módulo de Android para la variante móvil).
  - CMake 3.10+.
  - Android Studio y NDK (para despliegue móvil).

---

## Instalación y Uso

### Parte 1: Notebook de Análisis (Kaggle)

El notebook se ejecuta directamente en la plataforma Kaggle clonando el dataset UPS-Writing-Skills.

```bash
# Dependencias necesarias
pip install opencv-python scikit-learn scikit-image pandas numpy matplotlib seaborn

# El notebook incluye:
# 1. Funciones de inyección de ruido (Gaussiano, Salt & Pepper)
# 2. Extracción de Momentos de Hu con transformación logarítmica
# 3. Extracción de Momentos de Zernike (n=8)
# 4. Entrenamiento de clasificadores KNN/SVM
# 5. Generación de matrices de confusión por nivel de ruido
```

Archivo: `parte1/analisis-comparativo-momentos-hu-zernike.ipynb`

### Parte 2: Aplicación C++ (Shape Signature)

Compilación del sistema de reconocimiento:

```bash
cd parte2
mkdir build && cd build

# Configurar con CMake
cmake ..

# Compilar
make

# Generar corpus de entrenamiento
./shape_app train

# Evaluar dataset de prueba
./shape_app test

# Clasificar imagen individual
./shape_app classify data/testing/circle/ejemplo.png
```

Estructura esperada del dataset:
```
data/
├── training/
│   ├── circle/
│   ├── triangle/
│   └── square/
├── testing/
│   ├── circle/
│   ├── triangle/
│   └── square/
└── corpus.csv (generado automáticamente)
```

---

## Resultados y Hallazgos

Resumen de los resultados obtenidos durante la evaluación experimental:

| Descriptor | Invarianza a Rotación | Robustez al Ruido | Precisión (bajo ruido) | Precisión (alto ruido) |
|------------|----------------------|-------------------|------------------------|------------------------|
| **Momentos de Hu** | Completa | Media | ~85% | ~65% |
| **Momentos de Zernike** | Completa | Alta | ~92% | ~78% |
| **FFT Coord. Complejas** | Completa | Alta | ~95% | ~88% |

### Conclusiones del Análisis

- Los Momentos de Zernike demostraron mayor robustez ante ruido gracias a su base ortogonal, manteniendo mejor discriminación entre clases en condiciones de alta degradación.
- La representación mediante coordenadas complejas para FFT proporciona una descripción compacta e invariante de la forma, ideal para implementaciones en tiempo real.
- La normalización por el primer armónico (|F(1)|) garantiza invarianza a escala, mientras que el uso de magnitudes elimina la dependencia de la fase (invarianza a rotación).
- El sistema de Shape Signature implementado alcanza tiempos de inferencia menores a 10ms por imagen en dispositivos móviles de gama media, demostrando viabilidad para aplicaciones interactivas.
