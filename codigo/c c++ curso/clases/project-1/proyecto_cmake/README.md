# Proyecto Geometría con CMake

Este es un proyecto simple de C++ que implementa cálculos geométricos usando CMake.

## Estructura del Proyecto

```bash
proyecto_cmake/
├── Geometria.hpp      # Archivo de cabecera con declaraciones de clases
├── Geometria.cpp      # Implementación de las clases geométricas
├── main.cpp           # Archivo principal del programa
├── CMakeLists.txt     # Archivo de configuración de CMake
└── README.md          # Este archivo
```

## Compilación

### Paso 1: Crear directorio de construcción

```bash
mkdir build
cd build
```

### Paso 2: Generar archivos de construcción con CMake

```bash
cmake ..
```

### Paso 3: Compilar el proyecto

```bash
cmake --build .
```

O simplemente:

```bash
make
```

## Ejecución

Desde el directorio `build`:

```bash
./geometria
```

## Limpiar

Para limpiar los archivos generados:

```bash
cd build
rm -rf *
```

O elimina el directorio `build` completo:

```bash
rm -rf build
```

## Flujo de trabajo completo

```bash
# Compilar
mkdir build && cd build
cmake ..
make

# Ejecutar
./geometria

# Limpiar
cd ..
rm -rf build
```
