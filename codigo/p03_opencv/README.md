# Proyecto 03 - Detección de Rostros y Ojos en Tiempo Real 👁️

## Descripción

Este proyecto utiliza la cámara web para detectar rostros y ojos en tiempo real usando algoritmos Haar Cascade de OpenCV.

## Características

- ✅ Detección de múltiples rostros simultáneamente
- ✅ Detección de ojos dentro de cada rostro
- ✅ Procesamiento en tiempo real desde la cámara web
- ✅ Captura de fotos con las detecciones
- ✅ Modo escala de grises
- ✅ Contador de rostros en pantalla

## Compilación

```bash
make
```

## Uso

```bash
./detector_rostros
```

### Controles

- **ESC**: Salir del programa
- **C**: Capturar una foto (se guarda como `captura_N.jpg`)
- **G**: Activar/desactivar modo escala de grises

## Tecnologías

- **Haar Cascade Classifiers**: Para detección de rostros y ojos
- **VideoCapture**: Para captura de video en tiempo real
- **Equalización de Histograma**: Para mejorar la detección en diferentes condiciones de luz

## Notas

- Asegúrate de que tu cámara web esté conectada
- Los clasificadores Haar vienen incluidos con OpenCV
- El programa funciona mejor con buena iluminación

## Limpieza

```bash
make clean  # Elimina el ejecutable y las fotos capturadas
```
