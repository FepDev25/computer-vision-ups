# Visión por Computador

Repositorio de consolidación académica correspondiente a la asignatura de Visión por Computador, correspondiente al séptimo ciclo de la carrera de Computación. Este espacio integra el desarrollo de prácticas de laboratorio, proyectos integradores, material de investigación y recursos desarrollados durante el período lectivo.

La estructura del repositorio refleja la progresión temática de la materia, desde fundamentos de procesamiento digital de imágenes hasta técnicas avanzadas de deep learning y visión por computador en tiempo real, incluyendo el uso de librerías especializadas y frameworks de vanguardia en el campo.

## Estructura del Repositorio

### Proyectos Integradores

Los proyectos de mayor envergadura desarrollados durante el ciclo se mantienen como submódulos vinculados, permitiendo su gestión independiente mientras se mantienen referenciados desde este repositorio central.

#### Interciclo: Medical Vision App - Sistema de Análisis de Imágenes CT

Aplicación de escritorio desarrollada en C++ con Qt para el procesamiento y análisis avanzado de imágenes médicas DICOM, específicamente tomografías computarizadas. El sistema integra técnicas clásicas de visión por computador con aprendizaje profundo para proporcionar herramientas profesionales de segmentación anatómica.

**Funcionalidades principales:**
- **Lectura DICOM nativa:** Integración con ITK (Insight Toolkit) para carga y navegación de datasets completos con soporte para imágenes de 16-bit.
- **Preprocesamiento inteligente:** Reducción de ruido mediante red neuronal DnCNN (vía servidor Flask o modelo ONNX local), filtros clásicos (Gaussiano, Mediana, Bilateral) y técnicas de mejora de contraste (CLAHE, Ecualización de Histograma).
- **Segmentación anatómica automática:** Detección de pulmones (rango -1024 a -400 HU), huesos (400-3071 HU) y aorta (120-300 HU) mediante umbralización adaptativa basada en Unidades Hounsfield, con algoritmos especializados de filtrado por posición anatómica y morfología matemática.
- **Operaciones morfológicas avanzadas:** Implementación completa de erosion, dilatación, apertura, cierre, gradiente, top hat y black hat con kernels configurables y algoritmo de relleno de huecos.
- **Sistema de métricas y validación:** Cálculo automático de área, densidad media en HU, SNR, PSNR e IoU contra Ground Truth para evaluación cuantitativa de segmentaciones.
- **Exportación de resultados:** Generación de imágenes PNG para todas las etapas del pipeline (original, preprocesada, máscaras, overlays, visualizaciones finales).

**Stack tecnológico:** C++17, Qt 6.10, OpenCV 4.10, ITK 6.0, libcurl, Flask (servidor DnCNN).

#### Proyecto Final: Sistema Híbrido de Detección de Peatones con Análisis de Postura

Sistema de visión por computador en tiempo real para detección de peatones con capacidad de identificación de posturas corporales. Implementa una arquitectura distribuida de tres niveles que combina técnicas clásicas (HOG, LBP) con aprendizaje profundo (YOLOv8-pose) mediante un pipeline de validación heurística de múltiples etapas.

**Arquitectura del sistema:**
- **Nivel 1 - Aplicación de Escritorio (C++/Qt):** Captura de video en tiempo real (640x480 @ 30 FPS) con detector híbrido HOG (peatones de pie) + LBP (posturas agachadas/sentadas). Implementa pipeline de 6 filtros heurísticos (varianza de intensidad, densidad de bordes Canny, gradientes Sobel, transiciones LBP, relación de aspecto, color de piel HSV) que reducen falsos positivos en un 83% comparado con HOG estándar.
- **Nivel 2 - Servidor Flask (Python):** Cola de procesamiento con worker thread dedicado, inferencia mediante YOLOv8-pose (17 keypoints COCO), generación de imágenes anotadas con esqueleto y métricas, y creación de GIF animado de 6 segundos con buffer de 30 frames.
- **Nivel 3 - Bot de Telegram:** Recepción de imágenes procesadas vía Bot API, envío de tres salidas (imagen original, anotada con esqueleto, GIF animado) y notificación con métricas de rendimiento.

**Características técnicas destacadas:**
- **Clasificador LBP entrenado específicamente:** Entrenamiento con MPII Human Pose Dataset (7,300 imágenes positivas, 4,500 negativas) para detección de posturas no erguidas, con ventana de 32x32 píxeles y 15 etapas de cascade.
- **Detección multi-postura:** Capacidad de detectar personas de pie, agachadas, sentadas e inclinadas mediante fusión de detectores.
- **Rendimiento optimizado:** 26-33 FPS en CPU para detección, 26 FPS con análisis completo en GPU, precisión promedio de 85.8% AP.
- **Análisis de postura:** Esqueleto completo de 17 keypoints COCO con confianza promedio del 86-96%.

**Stack tecnológico:** C++17, Qt 5.15, OpenCV 4.5, libcurl, Python 3, PyTorch, Ultralytics YOLOv8, Flask, python-telegram-bot.

### Prácticas de Laboratorio

Conjunto de prácticas desarrolladas en el componente práctico de la asignatura, abarcando desde fundamentos teóricos hasta implementaciones avanzadas con paralelización en GPU.

#### Práctica 1: Introducción al Procesamiento Digital de Imágenes

Estudio fundamentado en el cálculo de entropía de Shannon como métrica cuantitativa para evaluar el contenido informativo de imágenes digitales y su degradación ante la presencia de ruido. La práctica establece las bases para el análisis estadístico de imágenes mediante el procesamiento de un dataset de imágenes florales organizado en tres categorías (Lilly, Lotus, Orchid).

**Desarrollo técnico:**
- **Generación de variantes degradadas:** Sistema de inyección de ruido gaussiano con varianzas controladas (0.01 leve, 0.05 moderado, 0.2 alto) aplicado a dataset de 11,996 imágenes, generando cuatro versiones por imagen (original + 3 niveles de ruido).
- **Cálculo de entropía de Shannon:** Implementación de histograma de 256 bins, normalización de probabilidades y aplicación de la fórmula H = -Σ p(x) log₂ p(x) para imágenes en espacio RGB y escala de grises.
- **Análisis estadístico comparativo:** Evaluación de la sensibilidad diferencial entre espacios de color, demostrando que la entropía en escala de grises presenta mayor discriminación ante niveles crecientes de ruido que la entropía calculada sobre canales RGB concatenados.
- **Visualización de distribuciones:** Generación de diagramas de caja y bigotes (boxplots) y estimaciones de densidad kernel (KDE) para identificar patrones de dispersión, medianas y comportamiento de outliers ante la degradación por ruido.

**Resultados clave:** Confirmación de correlación positiva entre varianza del ruido gaussiano y valor de entropía, con la escala de grises identificada como espacio métrico más robusto para detección de alteraciones en imágenes naturales.

#### Práctica 3: Análisis Comparativo de Descriptores de Forma y Clasificación en Tiempo Real

Evaluación experimental comparativa de descriptores globales para el reconocimiento de formas geométricas bajo condiciones de transformaciones geométricas y ruido sintético, complementada con la implementación de un sistema de reconocimiento en tiempo real mediante firmas espectrales para despliegue en dispositivos móviles.

**Parte 1 - Análisis de Robustez (Python/Kaggle):**
- **Dataset:** UPS-Writing-Skills con clases Circle, Triangle, Rectangle sometidas a rotaciones aleatorias (0°-360°) e inyección de ruido Gaussiano y Salt & Pepper en tres niveles de intensidad.
- **Extracción de características:**
  - Momentos de Hu: 7 momentos invariantes con transformación logarítmica para mitigación de diferencias de orden de magnitud.
  - Momentos de Zernike: Polinomios ortogonales de grado n=8 para captura de detalles finos de la forma.
- **Clasificación y evaluación:** Entrenamiento de clasificadores KNN y SVM, generación de matrices de confusión por nivel de ruido para comparativa de degradación de descriptores.

**Parte 2 - Shape Signature en Tiempo Real (C++/Android):**
- **Representación de contorno:** Conversión del contorno de la forma a señal compleja s(n) = (x(n) - xc) + j(y(n) - yc), donde (xc, yc) representa el centroide.
- **Pipeline de procesamiento:** Extracción de contornos mediante umbralización adaptativa y findContours, interpolación lineal a 1024 puntos para estandarización, cálculo de centroide, construcción de señal compleja, aplicación de DFT (Transformada Discreta de Fourier) mediante cv::dft.
- **Normalización e invarianza:** División de coeficientes por |F(1)| para invarianza a escala, eliminación de componente de fase para invarianza a rotación, uso de 15 armónicos como descriptor compacto.
- **Clasificación:** Comparación por distancia euclídea contra corpus de entrenamiento con almacenamiento en CSV para referencia en aplicación móvil nativa.

**Hallazgos:** Los Momentos de Zernike demostraron superior robustez ante ruido debido a su base ortogonal, mientras que la FFT de coordenadas complejas proporcionó descriptores compactos con inferencia menor a 10ms en dispositivos móviles de gama media.

#### Práctica 4: Clasificación y Optimización de Objetos (CPU vs GPU)

Estudio comparativo exhaustivo de rendimiento entre arquitecturas de hardware (CPU vs GPU) y paradigmas de programación (Python high-level vs C++ low-level) en tareas representativas de visión por computador moderna, abarcando segmentación de instancias, super-resolución mediante IA generativa y procesamiento clásico con aceleración CUDA.

**Parte 1A - Segmentación de Instancias con YOLO:**
- Entrenamiento de red neuronal YOLOv11/v8 para segmentación precisa de frutas en contexto de agricultura de precisión.
- Dataset Fruit Instance Segmentation con múltiples categorías (Bitter Melon, Fig, Cucumber, entre otras).
- Generación de máscaras de segmentación pixel-perfect y validación de inferencia en tiempo real con webcam.

**Parte 1B - Benchmarking de Super-Resolución:**
- Pipeline combinado de detección de objetos (YOLO) y escalado de imagen mediante IA generativa (Real-ESRGAN).
- Evaluación de estrés computacional comparando rendimiento crítico entre torch.device('cpu') y torch.device('cuda').
- Resultados: 1 FPS en CPU (saturación de núcleo único al 99%) vs 12 FPS estable en GPU RTX 3070, demostrando la inviabilidad de redes generativas sin aceleración dedicada.

**Parte 1C - Pipeline Optimizado en C++ con CUDA:**
- Implementación de bajo nivel utilizando API nativa de OpenCV CUDA para procesamiento clásico de imágenes.
- Pipeline: Upload a GPU → Gaussian Blur → Cálculo de Histograma → Operaciones Morfológicas → Canny Edge Detection → Download.
- Gestión manual de memoria en GPU mediante cv::cuda::GpuMat para minimizar transferencias PCIe y maximizar throughput.
- Resultados: ~400 FPS en CPU vs ~800+ FPS en GPU para filtrado Gaussiano + Canny, liberando carga del procesador central.

### Prueba Práctica: Calibración por Fuerza Bruta

Evaluación técnica de procesamiento de imágenes mediante algoritmo de detección de regiones cromáticas con cálculo dinámico de anchos y procesamiento pixel-wise. El ejercicio consistió en desarrollar un sistema que identifique franjas de calibrado coloridas (verde y magenta) en imágenes de examen y aplique transformaciones geométricas de enmascaramiento basadas en la detección de dichas regiones.

**Implementación técnica:**
- **Detección cromática:** Algoritmo de umbralización por diferencia de canales con purezas calibradas (Verde: G > R+40 && G > B+40; Magenta: R > G+49 && B > G+49).
- **Cálculo dinámico de ancho:** Medición automática del ancho de la franja de calibrado mediante análisis de fila de referencia (fila n-4 para evitar ruido de borde), permitiendo adaptación a diferentes resoluciones de imagen.
- **Procesamiento por filas:** Iteración pixel-wise fila por fila para detección del borde izquierdo de la franja, con cálculo de límite de corte (posición borde + ancho detectado + offset de seguridad).
- **Enmascaramiento:** Pintado de negro (0,0,0) de todos los píxeles desde el inicio de la imagen hasta el límite de corte calculado, preservando el contenido útil de la imagen.

**Stack tecnológico:** C++, OpenCV 4.x, procesamiento directo de Mat con acceso pixel-wise mediante Vec3b.

### Material de Estudio

El directorio `materia/` contiene recursos teóricos, apuntes de clase, implementaciones de referencia e investigaciones sobre técnicas específicas de visión por computador. Incluye documentación sobre descriptores locales y globales (SIFT, HOG, LBP, Momentos de Hu, Zernike), técnicas de matching y correspondencia, así como fundamentos de deep learning aplicado a visión por computador.

### Recursos Adicionales

El directorio `codigo/` alberga implementaciones, scripts de utilidad y ejemplos desarrollados durante el ciclo que complementan las prácticas y proyectos principales.

### Dependencias Externas

El repositorio hace uso de librerías de terceros especializadas en procesamiento de imágenes y visión por computador, mantenidas como dependencias externas ignoradas por el control de versiones principal. Estas incluyen el toolkit ITK (Insight Toolkit) para procesamiento de imágenes médicas y el framework OpenCV junto con sus módulos contribuidos para desarrollo de aplicaciones de visión por computador.

---

Este repositorio representa el cuerpo de trabajo académico desarrollado durante el ciclo de Visión por Computador, documentando la progresión desde conceptos fundamentales hasta implementaciones avanzadas en entornos de producción.
