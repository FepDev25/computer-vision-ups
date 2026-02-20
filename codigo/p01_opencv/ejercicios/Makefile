# Makefile para los ejercicios de OpenCV básico
# Uso:
#   make        - Compila todos los ejercicios
#   make ej1    - Compila solo el ejercicio 1
#   make clean  - Elimina todos los ejecutables
#   make run1   - Compila y ejecuta ejercicio 1 (requiere imagen)

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Ruta donde están los archivos .h de OpenCV (headers/includes)
CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"

# Ruta donde están las bibliotecas compiladas de OpenCV (.so o .a)
LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"

# Bibliotecas que se enlazarán al programa (OpenCV y GStreamer)
LDLIBS = $(shell pkg-config --libs-only-l opencv4) \
         $(shell pkg-config --libs gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# Archivos fuente
SRC1 = ejercicio1.cpp
SRC2 = ejercicio2.cpp
SRC3 = ejercicio3.cpp
SRC4 = ejercicio4.cpp

# Ejecutables
TARGET1 = ejercicio1
TARGET2 = ejercicio2
TARGET3 = ejercicio3
TARGET4 = ejercicio4

# Targets
.PHONY: all clean help ej1 ej2 ej3 ej4

# Compilar todos los ejercicios
all: $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)
	@echo "✓ Todos los ejercicios compilados exitosamente"
	@echo ""
	@echo "Ejecuta los ejercicios así:"
	@echo "  ./ejercicio1 <imagen.jpg>"
	@echo "  ./ejercicio2 <imagen1.jpg> <imagen2.jpg> ..."
	@echo "  ./ejercicio3 <imagen.jpg>"
	@echo "  ./ejercicio4 <imagen.jpg>"

# Compilar ejercicio 1
ej1: $(TARGET1)

$(TARGET1): $(SRC1)
	@echo "Compilando ejercicio 1..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC1) -o $(TARGET1) $(LDFLAGS) $(LDLIBS)
	@echo "✓ Ejercicio 1 compilado: ./$(TARGET1)"

# Compilar ejercicio 2
ej2: $(TARGET2)

$(TARGET2): $(SRC2)
	@echo "Compilando ejercicio 2..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC2) -o $(TARGET2) $(LDFLAGS) $(LDLIBS)
	@echo "✓ Ejercicio 2 compilado: ./$(TARGET2)"

# Compilar ejercicio 3
ej3: $(TARGET3)

$(TARGET3): $(SRC3)
	@echo "Compilando ejercicio 3..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC3) -o $(TARGET3) $(LDFLAGS) $(LDLIBS)
	@echo "✓ Ejercicio 3 compilado: ./$(TARGET3)"

# Compilar ejercicio 4
ej4: $(TARGET4)

$(TARGET4): $(SRC4)
	@echo "Compilando ejercicio 4..."
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC4) -o $(TARGET4) $(LDFLAGS) $(LDLIBS)
	@echo "✓ Ejercicio 4 compilado: ./$(TARGET4)"

# Limpiar ejecutables
clean:
	@echo "Limpiando ejecutables..."
	rm -f $(TARGET1) $(TARGET2) $(TARGET3) $(TARGET4)
	@echo "✓ Limpieza completada"

# Ayuda
help:
	@echo "════════════════════════════════════════════════════════════"
	@echo "  Makefile - Ejercicios OpenCV Básico"
	@echo "════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make          - Compila todos los ejercicios"
	@echo "  make ej1      - Compila solo el ejercicio 1"
	@echo "  make ej2      - Compila solo el ejercicio 2"
	@echo "  make ej3      - Compila solo el ejercicio 3"
	@echo "  make ej4      - Compila solo el ejercicio 4"
	@echo "  make clean    - Elimina los ejecutables"
	@echo "  make help     - Muestra esta ayuda"
	@echo ""
	@echo "Ejemplos de ejecución:"
	@echo "  ./ejercicio1 imagen.jpg"
	@echo "  ./ejercicio2 foto1.jpg foto2.png foto3.jpg"
	@echo "  ./ejercicio3 imagen.jpg"
	@echo "  ./ejercicio4 imagen.jpg"
	@echo ""
	@echo "════════════════════════════════════════════════════════════"

# Targets de ejecución rápida (requieren una imagen de prueba)
# Nota: Estas reglas asumen que tienes una imagen llamada 'test.jpg'
test-all: all
	@echo ""
	@echo "⚠️  Estas pruebas requieren una imagen llamada 'test.jpg'"
	@echo "   Crea una o copia una imagen con ese nombre antes de ejecutar."
	@echo ""
	@if [ -f test.jpg ]; then \
		echo "✓ test.jpg encontrada, ejecutando pruebas..."; \
		echo ""; \
		echo "═══ Ejercicio 1 ═══"; \
		./ejercicio1 test.jpg; \
		echo ""; \
		echo "═══ Ejercicio 3 ═══"; \
		./ejercicio3 test.jpg; \
		echo ""; \
		echo "═══ Ejercicio 4 ═══"; \
		./ejercicio4 test.jpg; \
	else \
		echo "✗ test.jpg no encontrada"; \
		echo "  Crea una imagen de prueba: cp tu_imagen.jpg test.jpg"; \
	fi
