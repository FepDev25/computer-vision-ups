# Teoría de C++

## ¿Qué es C++?

C++ es un lenguaje de programación de propósito general creado por Bjarne Stroustrup en 1979 como una extensión del lenguaje C. Es un lenguaje **compilado**, **multi-paradigma** y **de tipado estático**.

## Características Principales

### 1. Lenguaje Compilado

C++ es un lenguaje compilado, lo que significa que el código fuente debe ser traducido a código máquina antes de ejecutarse. Este proceso incluye:

- **Preprocesamiento**: Procesa directivas (#include, #define, etc.)
- **Compilación**: Convierte código C++ a código objeto
- **Enlazado (Linking)**: Une los archivos objeto y bibliotecas para crear el ejecutable

### 2. Multi-Paradigma

C++ soporta múltiples paradigmas de programación:

- **Programación Procedural**: Basada en funciones y procedimientos
- **Programación Orientada a Objetos (POO)**: Clases, objetos, herencia, polimorfismo
- **Programación Genérica**: Templates y metaprogramación
- **Programación Funcional**: Lambdas, functores, algoritmos STL

### 3. Tipado Estático y Fuerte

- Las variables deben declararse con un tipo específico
- El tipo se verifica en tiempo de compilación
- No se permiten conversiones implícitas inseguras sin advertencias

### 4. Gestión Manual de Memoria

- Control directo sobre la asignación y liberación de memoria
- Uso de punteros y referencias
- Posibilidad de usar memoria dinámica (heap) y estática (stack)
- Responsabilidad del programador para evitar memory leaks

## Conceptos Fundamentales

### Compilación y Ejecución

El proceso típico para ejecutar un programa C++:

```c++
código fuente (.cpp) → compilador → código objeto (.o) → enlazador → ejecutable
```

### Archivos de Código

- **Archivos de implementación (.cpp)**: Contienen el código ejecutable
- **Archivos de cabecera (.h/.hpp)**: Contienen declaraciones, prototipos y definiciones de interfaz
- **Separación de interfaz e implementación**: Buena práctica para organizar código

### Espacio de Nombres (Namespaces)

Mecanismo para organizar código y evitar conflictos de nombres. El namespace `std` contiene la biblioteca estándar.

### Biblioteca Estándar (STL)

Conjunto de clases y funciones que proporcionan:

- Contenedores (vector, list, map, set, etc.)
- Algoritmos (sort, find, copy, etc.)
- Iteradores
- Funciones de entrada/salida

### Tipos de Datos

C++ distingue entre:

- **Tipos fundamentales**: int, float, double, char, bool, etc.
- **Tipos derivados**: arrays, punteros, referencias
- **Tipos definidos por el usuario**: struct, class, enum, union

### Alcance (Scope)

- **Alcance de bloque**: Variables dentro de {}
- **Alcance de función**: Parámetros y variables locales
- **Alcance de archivo**: Variables globales y estáticas
- **Alcance de clase**: Miembros de clase

### Duración de Almacenamiento

- **Automática**: Variables locales (stack)
- **Dinámica**: Memoria asignada con new/delete (heap)
- **Estática**: Variables globales y static
- **Thread**: Variables thread_local

## Programación Orientada a Objetos en C++

### Conceptos Clave

1. **Encapsulación**: Ocultar detalles de implementación
2. **Abstracción**: Mostrar solo lo esencial
3. **Herencia**: Reutilización y extensión de código
4. **Polimorfismo**: Múltiples formas de un mismo comportamiento

### Modificadores de Acceso

- **public**: Accesible desde cualquier parte
- **private**: Solo accesible desde la misma clase
- **protected**: Accesible desde la clase y sus derivadas

### Constructores y Destructores

- **Constructor**: Inicializa objetos
- **Destructor**: Libera recursos cuando el objeto se destruye
- **Constructor de copia**: Crea copias de objetos
- **Constructor de movimiento**: Transfiere recursos (C++11+)

## Gestión de Memoria

### Stack vs Heap

- **Stack**: Memoria automática, rápida, tamaño limitado
- **Heap**: Memoria dinámica, más lenta, tamaño flexible

### Punteros y Referencias

- **Punteros**: Variables que almacenan direcciones de memoria
- **Referencias**: Alias de otras variables, no pueden ser nulas ni reasignadas

### RAII (Resource Acquisition Is Initialization)

Principio fundamental: los recursos se adquieren en el constructor y se liberan en el destructor.

## Templates (Plantillas)

Mecanismo de programación genérica que permite escribir código independiente del tipo:

- **Templates de función**: Funciones genéricas
- **Templates de clase**: Clases genéricas
- **Especialización**: Versiones específicas para ciertos tipos

## Manejo de Excepciones

Sistema para manejar errores en tiempo de ejecución:

- **try**: Bloque donde puede ocurrir una excepción
- **catch**: Captura y maneja excepciones
- **throw**: Lanza una excepción

## Características Modernas (C++11 y posteriores)

### C++11

- Auto type deduction
- Range-based for loops
- Lambda expressions
- Smart pointers (unique_ptr, shared_ptr)
- Move semantics
- nullptr

### C++14/17/20

- Generic lambdas
- Structured bindings
- std::optional, std::variant
- Concepts (C++20)
- Coroutines (C++20)
- Ranges (C++20)

## Modelo de Compilación

### Compilación Separada

- Cada archivo .cpp se compila independientemente
- Los archivos .h se incluyen mediante #include
- El enlazador combina todos los archivos objeto

### Guards de Inclusión

Previenen la inclusión múltiple de archivos de cabecera:

- #ifndef, #define, #endif
- #pragma once (no estándar pero ampliamente soportado)

## Filosofía de C++

### Principios de Diseño

1. **Zero-overhead abstraction**: No pagas por lo que no usas
2. **Rendimiento**: Comparable con código ensamblador optimizado
3. **Control**: El programador tiene el control total
4. **Compatibilidad**: Mantiene compatibilidad con C
5. **Expresividad**: Permite expresar intenciones claramente

### Ventajas

- Alto rendimiento
- Control preciso sobre recursos
- Amplia adopción en la industria
- Ecosistema maduro de bibliotecas
- Portabilidad

### Desafíos

- Curva de aprendizaje pronunciada
- Gestión manual de memoria propensa a errores
- Compilación puede ser lenta
- Sintaxis compleja
- Múltiples formas de hacer lo mismo

## Casos de Uso Típicos

C++ es ideal para:

- Sistemas operativos
- Motores de videojuegos
- Aplicaciones de alto rendimiento
- Software embebido
- Procesamiento en tiempo real
- Sistemas de trading financiero
- Navegadores web
- Bases de datos
- Compiladores

## Estándares de C++

- **C++98/03**: Estándar original
- **C++11**: Primera gran modernización
- **C++14**: Refinamiento de C++11
- **C++17**: Más características modernas
- **C++20**: Cambios significativos (concepts, coroutines, ranges)
- **C++23**: Último estándar (en desarrollo continuo)

## Conclusión

C++ es un lenguaje poderoso y versátil que ofrece un balance único entre:

- **Abstracción de alto nivel**: POO, templates, lambdas
- **Control de bajo nivel**: Punteros, gestión manual de memoria
- **Rendimiento**: Comparable con código ensamblador

Su complejidad es el precio de su flexibilidad y poder, pero dominar C++ proporciona una base sólida para entender cómo funcionan las computadoras y escribir software eficiente.
