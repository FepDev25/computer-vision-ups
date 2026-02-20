# Versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.10)

# Nombre del proyecto y versión
project(Geometria VERSION 1.0)

# Especificar el estándar de C++
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Agregar flags de compilación
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Agregar el ejecutable
add_executable(geometria 
    Operaciones.cpp 
    Principal.cpp
)

# Mensaje de información
message(STATUS "Configurando proyecto Geometria")
message(STATUS "Compilador C++: ${CMAKE_CXX_COMPILER}")
message(STATUS "Estándar C++: ${CMAKE_CXX_STANDARD}")
