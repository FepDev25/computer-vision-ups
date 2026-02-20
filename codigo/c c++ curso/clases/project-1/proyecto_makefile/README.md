# Proyecto Calculadora con Makefile

Este es un proyecto simple de C++ que implementa una calculadora básica usando Makefile.

## Estructura del Proyecto

```
proyecto_makefile/
├── Calculadora.hpp    # Archivo de cabecera con la declaración de la clase
├── Calculadora.cpp    # Implementación de la clase Calculadora
├── main.cpp           # Archivo principal del programa
├── Makefile           # Archivo de construcción
└── README.md          # Este archivo
```

## Compilación

Para compilar el proyecto, ejecuta:

```bash
make
```

## Ejecución

Para ejecutar el programa:

```bash
make run
```

O directamente:

```bash
./calculadora
```

## Limpiar

Para eliminar los archivos compilados:

```bash
make clean
```

## Comandos del Makefile

- `make` o `make all`: Compila el proyecto
- `make clean`: Elimina archivos objeto y el ejecutable
- `make run`: Compila y ejecuta el programa
