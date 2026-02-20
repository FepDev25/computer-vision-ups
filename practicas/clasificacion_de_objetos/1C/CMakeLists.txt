cmake_minimum_required(VERSION 3.10)
project(VisionPipeline)

set(CMAKE_CXX_STANDARD 17)

# Configuración para OpenCV local (instalación personalizada)
set(OpenCV_DIR "$ENV{HOME}/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib/cmake/opencv4")

# Busca OpenCV en la ruta especificada
find_package(OpenCV REQUIRED)

# Muestra qué librerías encontró (útil para depurar)
message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(vision_app main.cpp)
target_link_libraries(vision_app ${OpenCV_LIBS})