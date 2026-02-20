# Proyecto 04 - Detección de Manos y Conteo de Dedos 🖐️✋

## Descripción

Este proyecto implementa un **sistema avanzado de detección de manos y reconocimiento de gestos** en tiempo real usando procesamiento de imágenes con OpenCV. Detecta la mano, cuenta los dedos levantados y reconoce gestos básicos.

## Características Principales

### 🎯 Detección Avanzada

- ✅ **Detección de piel en tiempo real** usando espacio de color HSV
- ✅ **Segmentación de la mano** con técnicas morfológicas
- ✅ **Cálculo de envolvente convexa** (Convex Hull)
- ✅ **Análisis de defectos de convexidad** (espacios entre dedos)

### 🔢 Conteo de Dedos Inteligente

- ✅ Cuenta dedos del 0 al 5
- ✅ Detecta espacios entre dedos usando geometría
- ✅ Usa ley de cosenos para calcular ángulos
- ✅ Filtra falsos positivos con umbrales de distancia y ángulo

### 🎨 Visualización Rica

- ✅ Contorno de la mano en verde
- ✅ Envolvente convexa en azul
- ✅ Centro de la palma marcado en rojo
- ✅ Defectos de convexidad (espacios entre dedos) en amarillo
- ✅ Rectángulo delimitador en magenta
- ✅ Información en tiempo real: dedos, área, gesto

### 👌 Reconocimiento de Gestos

- **0 dedos**: Puño cerrado
- **1 dedo**: Apuntar
- **2 dedos**: Paz / Victoria
- **3 dedos**: Tres
- **4 dedos**: Cuatro
- **5 dedos**: Mano abierta

## Compilación

```bash
make          # Compilar
make run      # Compilar y ejecutar
make debug    # Compilar con símbolos de depuración
```

## Uso

```bash
./detector_manos
```

### Instrucciones de Uso

1. **Posiciona tu mano** frente a la cámara
2. **Mueve tu mano** lentamente para ver la detección
3. **Levanta dedos** para ver el conteo en tiempo real
4. **Prueba gestos** diferentes (puño, paz, mano abierta, etc.)

### Controles

- **ESC**: Salir del programa
- **C**: Capturar imagen con detecciones
- **B**: Ajustar detección de color piel
- **H**: Mostrar/ocultar ayuda en pantalla

## Tecnología Utilizada

### Algoritmos Implementados

1. **Segmentación por Color (HSV)**
   - Convierte BGR → HSV para mejor detección de piel
   - Rango HSV optimizado para tonos de piel
   - Funciona con diferentes iluminaciones

2. **Operaciones Morfológicas**
   - Cierre morfológico (MORPH_CLOSE) - elimina pequeños huecos
   - Apertura morfológica (MORPH_OPEN) - elimina ruido
   - Kernel elíptico de 11x11 para suavizado

3. **Envolvente Convexa (Convex Hull)**
   - Encuentra el polígono convexo mínimo que rodea la mano
   - Útil para identificar la forma general

4. **Defectos de Convexidad**
   - Detecta los puntos más alejados entre el contorno y la envolvente
   - Estos defectos corresponden a los espacios entre dedos

5. **Geometría y Ley de Cosenos**
   - Calcula ángulos entre puntos usando: `cos(θ) = (a² + b² - c²) / (2ab)`
   - Filtra dedos válidos (ángulo < 90°)

6. **Momentos de Imagen**
   - Calcula el centroide (centro de masa) de la palma
   - Usado como punto de referencia para mediciones

## Ventanas del Programa

1. **"Deteccion de Manos - Original"**
   - Muestra el video con todas las visualizaciones
   - Contornos, envolvente, centro, dedos contados

2. **"Deteccion de Manos - Mascara"**
   - Muestra la máscara binaria de detección de piel
   - Útil para debug y ajustes

## Tips para Mejor Detección

### ✅ Mejores Resultados

- Usa buena iluminación frontal
- Fondo simple (pared clara o oscura)
- Mano perpendicular a la cámara
- Evita sombras proyectadas

### ⚠️ Ajustes Necesarios

Si la detección no funciona bien:

- Ajusta los valores HSV en el código (líneas 70-71)
- Modifica el área mínima (línea 102)
- Cambia los umbrales de ángulo y distancia

## Conceptos Aprendidos

- 🧠 Segmentación de imágenes por color
- 🧠 Morfología matemática
- 🧠 Análisis de contornos
- 🧠 Geometría computacional
- 🧠 Defectos de convexidad
- 🧠 Reconocimiento de gestos
- 🧠 Procesamiento en tiempo real

## Aplicaciones Prácticas

- 🎮 Control de videojuegos sin contacto
- 🎨 Arte interactivo
- 📊 Presentaciones interactivas
- 🤖 Interfaz humano-computadora
- 📱 Lenguaje de señas básico
- 🎯 Sistemas de realidad aumentada

## Limpieza

```bash
make clean  # Elimina ejecutable y capturas
```

## Próximos Pasos

Ideas para extender este proyecto:

- Agregar más gestos (OK, pulgar arriba, etc.)
- Implementar seguimiento temporal (tracking)
- Usar Machine Learning para mejor precisión
- Reconocer lenguaje de señas
- Agregar control de volumen o brillo con gestos

---

**¡Disfruta experimentando con la detección de manos!** 🖐️✨
