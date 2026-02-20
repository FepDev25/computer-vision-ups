# Operaciones Morfológicas en OpenCV

## Demostración Completa en Tiempo Real

---

## 📖 Descripción

Este programa demuestra **todas las operaciones morfológicas** vistas en clase de Visión por Computador, aplicadas en tiempo real sobre el video de una cámara web. Todas las operaciones se muestran simultáneamente en una sola ventana organizada en formato grid 3x4.

---

## 🎯 Operaciones Morfológicas Implementadas

### **1. Imagen Original (Escala de Grises)**

Base para todas las operaciones morfológicas.

### **2. Erosión** 🔻

- **Función**: `erode()`
- **Efecto**: Elimina los bordes de los objetos, reduce su tamaño
- **Uso**: Eliminar ruido pequeño, separar objetos conectados
- **Fórmula**: Mínimo de los píxeles en el área del kernel

### **3. Dilatación** 🔺

- **Función**: `dilate()`
- **Efecto**: Engrosa las áreas, aumenta el tamaño de los objetos
- **Uso**: Rellenar huecos pequeños, conectar regiones cercanas
- **Fórmula**: Máximo de los píxeles en el área del kernel

### **4. Opening (Apertura)** 🔓

- **Función**: `morphologyEx(..., MORPH_OPEN, ...)`
- **Operación**: Erosión seguida de Dilatación
- **Efecto**: Elimina objetos pequeños (ruido) manteniendo la forma de objetos grandes
- **Uso**: Limpieza de ruido sin perder el tamaño original

### **5. Closing (Cierre)** 🔒

- **Función**: `morphologyEx(..., MORPH_CLOSE, ...)`
- **Operación**: Dilatación seguida de Erosión
- **Efecto**: Cierra huecos internos y conecta regiones cercanas
- **Uso**: Rellenar agujeros dentro de objetos

### **6. Gradiente Morfológico** 📈

- **Función**: `morphologyEx(..., MORPH_GRADIENT, ...)`
- **Operación**: Dilatación - Erosión
- **Efecto**: Resalta los bordes de los objetos
- **Uso**: Detección de contornos

### **7. Top Hat** 🎩

- **Función**: `morphologyEx(..., MORPH_TOPHAT, ...)`
- **Operación**: Imagen Original - Opening
- **Efecto**: Resalta objetos claros más pequeños que el elemento estructurante
- **Uso**: Detectar características brillantes en fondo oscuro

### **8. Black Hat** 🎓

- **Función**: `morphologyEx(..., MORPH_BLACKHAT, ...)`
- **Operación**: Closing - Imagen Original
- **Efecto**: Resalta objetos oscuros más pequeños que el elemento estructurante
- **Uso**: Detectar características oscuras en fondo claro

### **9. Diferencia (Top Hat - Black Hat)** ⚖️

- **Operación**: Top Hat - Black Hat
- **Efecto**: Resalta tanto objetos claros como oscuros
- **Uso**: Preparación para mejora de contraste

### **10. Contraste Mejorado** ✨

- **Operación**: Original + (Top Hat - Black Hat)
- **Efecto**: Mejora el contraste general de la imagen
- **Uso**: Realzar detalles y mejorar la calidad visual
- **Fórmula**: `contraste = original + (tophat - blackhat)`

---

## 🎛️ Controles Interactivos

### **Trackbars Disponibles:**

| Control | Rango | Descripción |
|---------|-------|-------------|
| **Kernel Size** | 0-10 | Tamaño del elemento estructurante (se convierte a impar: 1, 3, 5, ..., 21) |
| **Elem** | 0-2 | Tipo de elemento: 0=Cruz, 1=Rectángulo, 2=Elipse |
| **Iteraciones** | 0-5 | Número de veces que se aplica la operación (mínimo 1) |

### **Teclas:**

- **ESC**: Salir del programa

---

## 🔧 Elementos Estructurantes

### **Cruz (MORPH_CROSS)**

```bash
  ⬜
⬜⬛⬜
  ⬜
```

- Conectividad de 4 vecinos
- Mejor para estructuras lineales

### **Rectángulo (MORPH_RECT)**

```bash
⬜⬜⬜
⬜⬛⬜
⬜⬜⬜
```

- Conectividad de 8 vecinos
- Más agresivo, preserva esquinas

### **Elipse (MORPH_ELLIPSE)**

```bash
  ⬜⬜
⬜⬛⬛⬜
⬜⬛⬛⬜
  ⬜⬜
```

- Forma circular
- Mejor para objetos redondeados

---

## 📊 Comparación Visual

| Operación | Objetos Claros | Objetos Oscuros | Bordes | Ruido |
|-----------|----------------|-----------------|--------|-------|
| Erosión | Reduce | Agranda | Adelgaza | Elimina claros |
| Dilatación | Agranda | Reduce | Engrosa | Elimina oscuros |
| Opening | Elimina pequeños | Mantiene | Suaviza | Elimina claros |
| Closing | Mantiene | Elimina pequeños | Suaviza | Elimina oscuros |
| Gradiente | Detecta | Detecta | Resalta | Amplifica |
| Top Hat | Resalta | Oculta | - | Detecta claros |
| Black Hat | Oculta | Resalta | - | Detecta oscuros |

---

## 🚀 Compilación y Ejecución

### **Usando Makefile (si existe):**

```bash
make
./operaciones_morfologicas
```

### **Compilación manual:**

```bash
g++ main.cpp -o morfologicas `pkg-config --cflags --libs opencv4` -std=c++17
./morfologicas
```

---

## 📋 Requisitos

- **OpenCV 4.x**: Para procesamiento de imágenes
- **C++17**: Estándar de C++ utilizado
- **Cámara web**: Para captura de video en tiempo real
- **pkg-config**: Para compilación con OpenCV

### **Instalación de dependencias (Ubuntu/Debian):**

```bash
sudo apt update
sudo apt install build-essential cmake
sudo apt install libopencv-dev
```

---

## 🎓 Conceptos Teóricos

### **¿Qué son las Operaciones Morfológicas?**

Las operaciones morfológicas son técnicas de procesamiento de imágenes que procesan imágenes basándose en **formas**. Utilizan un **elemento estructurante** que se desliza sobre la imagen realizando operaciones lógicas.

### **Aplicaciones Prácticas:**

1. **Preprocesamiento**: Eliminación de ruido antes de segmentación
2. **Extracción de características**: Bordes, esqueletos
3. **Mejora de contraste**: Top Hat + Black Hat
4. **Segmentación**: Separación de objetos
5. **Reconocimiento de patrones**: Matching morfológico

### **Propiedades Importantes:**

- **Erosión y Dilatación** son duales
- **Opening y Closing** son idempotentes (aplicarlas múltiples veces = aplicarlas una vez)
- **Opening** es antiextensiva (resultado ≤ original)
- **Closing** es extensiva (resultado ≥ original)

---

## 💡 Consejos de Uso

1. **Kernel pequeño (1-3)**: Para detalles finos
2. **Kernel mediano (5-9)**: Para objetos regulares
3. **Kernel grande (11-21)**: Para estructuras grandes

**Experimentar con:**

- Diferentes tipos de elementos (Cruz, Rectángulo, Elipse)
- Número de iteraciones para efectos más pronunciados
- Combinaciones de operaciones

---

## 📸 Ejemplos de Resultados

### **Mejora de Contraste:**

```bash
Original → Top Hat (resalta claros) → Black Hat (resalta oscuros)
         → Diferencia → Contraste Mejorado
```

### **Limpieza de Ruido:**

```bash
Imagen con ruido → Opening → Imagen limpia (ruido blanco eliminado)
                 → Closing → Imagen limpia (ruido negro eliminado)
```

### **Detección de Bordes:**

```bash
Imagen → Gradiente Morfológico → Bordes resaltados
```

---

## 🔍 Diferencias Clave

### **Erosión vs Dilatación**

- **Erosión**: Elimina píxeles en los bordes (hace más pequeño)
- **Dilatación**: Agrega píxeles en los bordes (hace más grande)

### **Opening vs Closing**

- **Opening**: Erosión + Dilatación (elimina protuberancias)
- **Closing**: Dilatación + Erosión (rellena entrantes)

### **Top Hat vs Black Hat**

- **Top Hat**: Detecta objetos claros pequeños
- **Black Hat**: Detecta objetos oscuros pequeños

---

## 📚 Referencias

- [OpenCV Morphological Transformations](https://docs.opencv.org/4.x/d9/d61/tutorial_py_morphological_ops.html)
- [Digital Image Processing - Gonzalez & Woods](https://www.imageprocessingplace.com/)
- Documentación de clase: Operaciones Morfológicas

---

## 🐛 Solución de Problemas

### **Error: "No se pudo abrir la cámara"**

- Verifica que la cámara esté conectada
- Prueba cambiar `VideoCapture(0)` a `VideoCapture(1)` o `VideoCapture(2)`
- Verifica permisos de acceso a la cámara

### **Ventana muy grande o muy pequeña**

- Usa `WINDOW_NORMAL` y redimensiona manualmente
- Ajusta el tamaño de las imágenes en el código (línea de `resize`)

### **FPS bajo**

- Reduce el tamaño de las imágenes
- Disminuye el tamaño del kernel
- Reduce el número de iteraciones

---

## 👨‍💻 Autor

**Curso**: Visión por Computador  
**Tema**: Operaciones Morfológicas  
**Fecha**: Noviembre 2025

---

## 📝 Notas

Este programa es una herramienta educativa para entender visualmente las operaciones morfológicas. Es especialmente útil para:

- Estudiar para exámenes
- Experimentar con parámetros
- Comparar efectos en tiempo real
- Entender la teoría con práctica

**Tip**: Coloca tu mano u objetos frente a la cámara con diferentes formas para ver cómo reacciona cada operación! 🖐️
