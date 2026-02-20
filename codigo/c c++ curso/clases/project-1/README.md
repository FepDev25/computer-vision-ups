# Proyectos de Ejemplo C++

Este directorio contiene dos proyectos básicos de C++ que demuestran diferentes sistemas de construcción.

## Proyectos Incluidos

### 1. proyecto_makefile - Calculadora

Un proyecto simple que implementa una calculadora básica usando **Makefile**.

**Características:**
- Clase `Calculadora` con operaciones aritméticas básicas
- Sistema de construcción con Makefile
- Comandos simples: `make`, `make run`, `make clean`

**Ir al proyecto:**
```bash
cd proyecto_makefile
```

### 2. proyecto_cmake - Geometría

Un proyecto que realiza cálculos geométricos usando **CMake**.

**Características:**
- Clases `Circulo` y `Rectangulo` para cálculos geométricos
- Sistema de construcción con CMake
- Construcción fuera del árbol de fuentes (out-of-source build)

**Ir al proyecto:**
```bash
cd proyecto_cmake
```

## Diferencias entre Makefile y CMake

### Makefile
- ✅ Simple y directo para proyectos pequeños
- ✅ No requiere herramientas adicionales
- ✅ Control granular del proceso de compilación
- ❌ Menos portable entre plataformas
- ❌ Puede volverse complejo en proyectos grandes

### CMake
- ✅ Altamente portable (Windows, Linux, macOS)
- ✅ Genera archivos de construcción nativos (Makefiles, Visual Studio, etc.)
- ✅ Excelente para proyectos grandes y complejos
- ✅ Manejo automático de dependencias
- ❌ Requiere CMake instalado
- ❌ Curva de aprendizaje más pronunciada

## Requisitos

### Para ambos proyectos:
- Compilador C++ (g++, clang++)
- Estándar C++11 o superior

### Para proyecto_makefile:
- `make` (usualmente preinstalado en Linux)

### Para proyecto_cmake:
- CMake (versión 3.10 o superior)

Para instalar CMake en Ubuntu/Debian:
```bash
sudo apt-get install cmake
```

## Estructura de Archivos

Ambos proyectos siguen la estructura estándar de C++:
- **`.hpp`**: Archivos de cabecera con declaraciones
- **`.cpp`**: Archivos de implementación
- **`main.cpp`**: Punto de entrada del programa
