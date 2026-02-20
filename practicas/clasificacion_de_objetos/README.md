# Práctica de Clasificación y Optimización de Objetos (CPU vs GPU)

Este repositorio contiene el desarrollo experimental para evaluar y comparar el rendimiento de diferentes arquitecturas de hardware (CPU vs. GPU) y paradigmas de programación (Python High-Level vs. C++ Low-Level) en tareas modernas de visión por computador.

El proyecto se divide en tres módulos principales que abarcan desde el entrenamiento de modelos de segmentación hasta la implementación de pipelines optimizados en memoria VRAM.

## Estructura del Proyecto

### Parte 1A: Segmentación de Instancias (YOLOv11/v8)

Entrenamiento y validación de una red neuronal para la segmentación precisa de frutas en agricultura de precisión.

- **Dataset:** Fruit Instance Segmentation (Bitter Melon, Fig, Cucumber, etc.).
- **Tecnología:** Python, Ultralytics YOLO.
- **Objetivo:** Generar máscaras de segmentación y validar inferencia en tiempo real.

### Parte 1B: Benchmarking Deep Learning (Super Resolución)

Prueba de estrés computacional combinando detección de objetos y escalado de imagen mediante IA generativa.

- **Pipeline:** YOLO (Detección) + Real-ESRGAN (Upscaling x2).
- **Comparativa:** Rendimiento crítico entre `torch.device('cpu')` vs `torch.device('cuda')`.
- **Tecnología:** Python, PyTorch, BasicsR.

### Parte 1C: Pipeline Optimizado en C++ (CUDA)

Implementación de bajo nivel utilizando la API de OpenCV CUDA para procesamiento de imágenes clásico.

- **Pipeline:** Upload -> Gaussian Blur -> Histograma -> Morfología -> Canny -> Download.
- **Optimización:** Gestión manual de memoria en GPU (`cv::cuda::GpuMat`) para minimizar transferencias PCIe.
- **Tecnología:** C++, OpenCV 4.x (con soporte CUDA), CMake.

---

## Requisitos del Sistema

- **Hardware:**
  - CPU: Intel Core i9-12900K (o equivalente).
  - GPU: NVIDIA RTX 3070 (Requerido para aceleración CUDA).
- **Software:**
  - Linux (Arch Linux / Ubuntu).
  - Python 3.10+.
  - CUDA Toolkit 11.x / 12.x.
  - OpenCV compilado con `WITH_CUDA=ON`.
  - Compilador C++ (GCC/G++).

---

## Instalación y Uso

### Parte 1A: Entrenamiento y Predicción

```bash
cd 1A
pip install ultralytics opencv-python

# 1. Preparar el dataset (Descarga y estructura)
python preparar_dataset_full.py

# 2. Entrenar el modelo (Local)
python train.py

# 3. Probar inferencia con Webcam
python inference_webcam.py

```

### Parte 1B: Super Resolución (Python)

```bash
cd 1B
pip install -r requirements.txt

# Ejecución en GPU (Recomendado)
python main.py video.mp4 --output salida-cuda.mp4 --device cuda

# Ejecución en CPU (Para evidenciar cuello de botella)
python main.py video.mp4 --output salida-cpu.mp4 --device cpu

```

### Parte 1C: Pipeline C++ (OpenCV CUDA)

```bash
cd 1C
mkdir build && cd build

# Compilar el proyecto
cmake ..
make

# Ejecutar Pipeline en GPU
./vision_app video.mp4 --gpu

# Ejecutar Pipeline en CPU (Comparativa)
./vision_app video.mp4 --cpu

```

---

## Resultados y Benchmarks

Resumen de rendimiento obtenido durante las pruebas experimentales:

| Módulo | Tarea | Dispositivo (CPU) | Dispositivo (GPU RTX 3070) | Factor de Mejora |
| --- | --- | --- | --- | --- |
| **Parte 1B** | Super Resolución (x2) | ~1 FPS (Saturado) | ~12 FPS (Estable) | **12x** |
| **Parte 1C** | Filtrado Gaussiano + Canny | ~400 FPS | ~800+ FPS | **2x** |

> **Nota:** En la Parte 1B, la CPU (i9-12900K) alcanzó el 99% de uso en un solo núcleo, demostrando la inviabilidad de ejecutar redes generativas sin aceleración dedicada. En la Parte 1C, el uso de GPU liberó casi totalmente la carga del procesador central.
