TARGET = vision_app
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Ruta donde están los archivos .h de OpenCV (headers/includes)
CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"

# Ruta donde están las bibliotecas compiladas de OpenCV (.so o .a)
LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"

# Bibliotecas necesarias para este proyecto
LDLIBS = -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_imgcodecs \
         -lopencv_videoio -lopencv_video \
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
	rm -f $(TARGET)

# Ayuda
help:
	@echo "Uso:"
	@echo "  make       - Compila el programa"
	@echo "  make run   - Compila y ejecuta"
	@echo "  make clean - Elimina ejecutable"
