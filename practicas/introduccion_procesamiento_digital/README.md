# Introducción al Procesamiento Digital de Imágenes

Este repositorio contiene el desarrollo experimental de la primera práctica de laboratorio de Visión por Computador, enfocada en los fundamentos del procesamiento digital de imágenes y la cuantificación de cambios mediante el cálculo de entropía.

El proyecto establece las bases teóricas y prácticas para el análisis de imágenes digitales, explorando conceptos fundamentales como la representación de imágenes en diferentes espacios de color, la inyección controlada de ruido y la medición cuantitativa de la información contenida en una imagen mediante la entropía de Shannon.

## Estructura del Proyecto

### Parte 1: Alteración de Imágenes con Ruido Gaussiano

Sistema de generación de variantes degradadas de un dataset de imágenes florales, aplicando tres niveles de ruido gaussiano controlado para estudiar el efecto de la degradación en la información visual.

- **Dataset:** Imágenes de flores organizadas en tres categorías (Lilly, Lotus, Orchid).
- **Niveles de Ruido:**
  - Leve (varianza = 0.01): Ruido apenas perceptible.
  - Moderado (varianza = 0.05): Ruido visible pero la imagen mantiene claridad.
  - Alto (varianza = 0.2): Ruido significativo que degrada notablemente la calidad visual.
- **Tecnología:** Python, scikit-image, NumPy.
- **Resultado:** Dataset expandido con versiones modificadas para análisis comparativo.

### Parte 2: Cálculo y Análisis de Entropía

Implementación del cálculo de entropía de Shannon para cuantificar la cantidad de información presente en las imágenes originales y sus variantes con ruido, tanto en espacio de color RGB como en escala de grises.

- **Metodología:** Cálculo de histograma, normalización y aplicación de la fórmula de Shannon.
- **Comparación:** Entropía de imágenes a color vs. entropía de imágenes en escala de grises.
- **Visualización:** Diagramas de caja y bigotes, gráficos de densidad (KDE) para análisis de distribuciones.
- **Exportación:** Resultados estructurados en formato CSV para análisis posterior.

### Parte 3: Visualización y Análisis Estadístico

Generación de visualizaciones que permiten identificar patrones en la distribución de la entropía según el nivel de modificación y el tipo de imagen.

- **Boxplots:** Comparación de medianas y dispersión por nivel de ruido.
- **KDE Plots:** Análisis de la forma de las distribuciones y superposición entre categorías.
- **Análisis de Outliers:** Identificación de imágenes con valores atípicos de entropía.

---

## Requisitos del Sistema

- **Hardware:**
  - CPU: Intel Core i3 o superior.
  - Memoria RAM: 4GB mínimo (8GB recomendado para procesamiento de batches grandes).
- **Software:**
  - Python 3.8+.
  - Jupyter Notebook o JupyterLab.
  - Librerías de procesamiento de imágenes y análisis de datos.

---

## Instalación y Uso

### Configuración del Entorno

```bash
# Instalación de dependencias
pip install scikit-image numpy pandas matplotlib seaborn scipy
```

### Ejecución de la Práctica

El desarrollo completo se encuentra en el notebook `practica.ipynb`, que incluye:

```python
# 1. Importación de librerías
import os
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from skimage import io, measure
from skimage.util import random_noise
from skimage.color import rgb2gray
from scipy.stats import entropy as scipy_entropy

# 2. Función de alteración de imágenes
# Aplica ruido gaussiano con varianza controlada

# 3. Procesamiento por lotes
# Itera sobre categorías y niveles de ruido

# 4. Cálculo de entropía
# Para imágenes a color y en escala de grises

# 5. Visualización
# Generación de gráficos estadísticos

# 6. Exportación
# Resultados a CSV
```

Estructura del dataset:
```
dataset2/
├── original/
│   ├── Lilly/
│   ├── Lotus/
│   └── Orchid/
├── mod-leve/
│   ├── Lilly/
│   ├── Lotus/
│   └── Orchid/
├── mod-moderado/
│   ├── Lilly/
│   ├── Lotus/
│   └── Orchid/
└── mod-alto/
    ├── Lilly/
    ├── Lotus/
    └── Orchid/
```

---

## Resultados y Hallazgos

Resumen de los hallazgos obtenidos durante el análisis experimental:

| Nivel de Modificación | Entropía Color (media) | Entropía Grises (media) | Tendencia |
|----------------------|------------------------|-------------------------|-----------|
| **Original** | ~6.5 | ~6.2 | Baseline |
| **Leve (var=0.01)** | ~7.2 | ~7.8 | Incremento moderado |
| **Moderado (var=0.05)** | ~7.5 | ~8.4 | Incremento significativo |
| **Alto (var=0.2)** | ~7.6 | ~8.8 | Saturación en color, continuo en grises |

### Conclusiones Principales

- **Correlación Ruido-Entropía:** Se confirmó una correlación positiva directa entre el nivel de ruido gaussiano añadido y la entropía de la imagen. A medida que aumenta la varianza del ruido, la entropía incrementa consistentemente, validando la entropía de Shannon como métrica efectiva para cuantificar desorden en imágenes.

- **Sensibilidad del Espacio de Color:** La entropía calculada sobre imágenes en escala de grises demostró ser un indicador más sensible para detectar alteraciones que la entropía de imágenes a color. Los gráficos de densidad mostraron una separación más nítida entre las distribuciones de diferentes niveles de ruido para el caso de escala de grises.

- **Normalización de Outliers:** Las imágenes originales presentaban outliers con entropía muy baja, correspondientes a imágenes visualmente simples. La adición de ruido, incluso a nivel leve, normalizó estas imágenes eliminando los outliers y homogeneizando la distribución.

- **Aplicación Práctica:** La entropía en escala de grises se identifica como la métrica más robusta para la detección de cambios y análisis de textura, sugiriendo que la conversión a escala de grises es un paso de preprocesamiento beneficioso para análisis de calidad de imagen.

---

## Referencias

- Shannon, C. E. (1948). A Mathematical Theory of Communication. Bell System Technical Journal.
- scikit-image documentation: https://scikit-image.org/
- Gonzalez, R. C., & Woods, R. E. Digital Image Processing (4th Edition).
