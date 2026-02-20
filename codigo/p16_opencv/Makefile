TARGET = hu # nombre del programa ejecutable
CXX = g++ 			 # compilador de C++
CXXFLAGS = -std=c++17 -Wall -Wextra # estándar de C++ y flags de advertencias

# Ruta donde están los archivos .h de OpenCV (headers/includes)
CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"

# Ruta donde están las bibliotecas compiladas de OpenCV (.so o .a)
LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"

# Bibliotecas que se enlazarán al programa (solo módulos básicos de OpenCV)
LDLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs \
         $(shell pkg-config --libs glib-2.0)

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
	rm -f $(TARGET) captura_*.jpg
