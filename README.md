# Visión por Computador

Repositorio de consolidación académica correspondiente a la asignatura de Visión por Computador, correspondiente al séptimo ciclo de la carrera de Computación. Este espacio integra el desarrollo de prácticas de laboratorio, proyectos integradores, material de investigación y recursos desarrollados durante el período lectivo.

La estructura del repositorio refleja la progresión temática de la materia, desde fundamentos de procesamiento digital de imágenes hasta técnicas avanzadas de deep learning y visión por computador en tiempo real, incluyendo el uso de librerías especializadas y frameworks de vanguardia en el campo.

## Estructura del Repositorio

### Proyectos Integradores

Los proyectos de mayor envergadura desarrollados durante el ciclo se mantienen como submódulos vinculados, permitiendo su gestión independiente mientras se mantienen referenciados desde este repositorio central.

**Interciclo:** Proyecto de evaluación parcial correspondiente al interciclo académico. Implementación de soluciones de visión por computador aplicadas a casos de estudio específicos.

**Proyecto Final:** Trabajo de integración final de la asignatura. Desarrollo completo de un sistema de visión por computador que consolida conocimientos adquiridos durante el ciclo lectivo.

### Prácticas de Laboratorio

Conjunto de prácticas desarrolladas en el componente práctico de la asignatura, cada una enfocada en dominios específicos del procesamiento de imágenes y la visión artificial.

**Introducción al Procesamiento Digital de Imágenes:** Fundamentos de análisis de imágenes mediante cálculo de entropía de Shannon. Estudio de la relación entre ruido gaussiano y contenido informativo en imágenes digitales, incluyendo análisis comparativo entre espacios de color RGB y escala de grises. Visualización estadística de distribuciones de entropía mediante técnicas de análisis de datos.

**Análisis Comparativo de Descriptores de Forma y Clasificación en Tiempo Real:** Evaluación experimental de descriptores globales para reconocimiento de formas geométricas. Comparativa de robustez entre Momentos de Hu y Momentos de Zernike ante transformaciones geométricas y ruido. Implementación de sistema de reconocimiento en tiempo real mediante Transformada de Fourier de coordenadas complejas (Shape Signature) con aplicación nativa en dispositivos móviles.

**Clasificación y Optimización de Objetos (CPU vs GPU):** Estudio comparativo de rendimiento entre arquitecturas de hardware y paradigmas de programación en tareas de visión por computador. Incluye entrenamiento de modelos de segmentación de instancias con YOLO, benchmarking de super-resolución mediante IA generativa (Real-ESRGAN), e implementación de pipelines optimizados en C++ con aceleración CUDA.

### Material de Estudio

El directorio `materia/` contiene recursos teóricos, apuntes de clase, implementaciones de referencia e investigaciones sobre técnicas específicas de visión por computador. Incluye documentación sobre descriptores locales y globales (SIFT, HOG, LBP, Momentos de Hu, Zernike), técnicas de matching y correspondencia, así como fundamentos de deep learning aplicado a visión por computador.

### Recursos Adicionales

El directorio `codigo/` alberga implementaciones, scripts de utilidad y ejemplos desarrollados durante el ciclo que complementan las prácticas y proyectos principales.

### Dependencias Externas

El repositorio hace uso de librerías de terceros especializadas en procesamiento de imágenes y visión por computador, mantenidas como dependencias externas ignoradas por el control de versiones principal. Estas incluyen el toolkit ITK (Insight Toolkit) para procesamiento de imágenes médicas y el framework OpenCV junto con sus módulos contribuidos para desarrollo de aplicaciones de visión por computador.

---

Este repositorio representa el cuerpo de trabajo académico desarrollado durante el ciclo de Visión por Computador, documentando la progresión desde conceptos fundamentales hasta implementaciones avanzadas en entornos de producción.
