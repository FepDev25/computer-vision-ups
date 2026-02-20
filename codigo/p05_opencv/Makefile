TARGET = main # nombre del programa ejecutable
CXX = g++ 			 # compilador de C++
CXXFLAGS = -std=c++17 -Wall -Wextra # estándar de C++ y flags de advertencias

# Ruta donde están los archivos .h de OpenCV (headers/includes)
CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"

# Ruta donde están las bibliotecas compiladas de OpenCV (.so o .a)
LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"

# Bibliotecas que se enlazarán al programa (OpenCV y GStreamer)
LDLIBS = $(shell pkg-config --libs-only-l opencv4) \
         $(shell pkg-config --libs gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# Regla principal: construir el ejecutable
all: $(TARGET)

# Cómo compilar el programa desde main.cpp
$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) main.cpp -o $(TARGET) $(LDFLAGS) $(LDLIBS)

# Regla para ejecutar el programa directamente
run: $(TARGET)
	./$(TARGET)

# Regla para limpiar archivos generados
clean:
	rm -f $(TARGET) captura_mano_*.jpg

# Regla para compilar con información de depuración
debug: CXXFLAGS += -g -O0
debug: clean $(TARGET)
