# Makefile para Tutorial Completo de OpenCV
# Procesamiento Digital de Imágenes

TARGET = tutorial
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Ruta donde están los archivos .h de OpenCV (headers/includes)
CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"

# Ruta donde están las bibliotecas compiladas de OpenCV (.so o .a)
LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"

# Bibliotecas que se enlazarán al programa (OpenCV y GStreamer)
LDLIBS = $(shell pkg-config --libs-only-l opencv4) \
         $(shell pkg-config --libs gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# Archivo fuente
SRC = main.cpp

# Targets
.PHONY: all clean help run

# Regla principal: construir el ejecutable
all: $(TARGET)
	@echo ""
	@echo "╔════════════════════════════════════════════════════════════════════╗"
	@echo "║  ✓ Tutorial compilado exitosamente                                ║"
	@echo "╚════════════════════════════════════════════════════════════════════╝"
	@echo ""
	@echo "📚 TUTORIAL COMPLETO DE PROCESAMIENTO DIGITAL DE IMÁGENES"
	@echo ""
	@echo "Uso:"
	@echo "  ./$(TARGET) <imagen.jpg>"
	@echo ""
	@echo "Ejemplos:"
	@echo "  ./$(TARGET) test.jpg"
	@echo "  ./$(TARGET) ~/Imágenes/foto.png"
	@echo ""
	@echo "Contenido del tutorial (13 secciones):"
	@echo "  1.  Carga y verificación de imágenes"
	@echo "  2.  Información y propiedades"
	@echo "  3.  Espacios de color (BGR, HSV, LAB, GRAY)"
	@echo "  4.  Operaciones básicas (brillo, contraste, rotación)"
	@echo "  5.  Filtrado y suavizado"
	@echo "  6.  Detección de bordes"
	@echo "  7.  Transformaciones morfológicas"
	@echo "  8.  Umbralización y binarización"
	@echo "  9.  Histogramas y ecualización"
	@echo "  10. Detección de contornos"
	@echo "  11. Operaciones lógicas"
	@echo "  12. Segmentación por color"
	@echo "  13. Manipulación directa de píxeles (12 ejercicios)"
	@echo ""

# Cómo compilar el programa desde main.cpp
$(TARGET): $(SRC)
	@echo "════════════════════════════════════════════════════════════════════"
	@echo "  Compilando Tutorial de OpenCV..."
	@echo "════════════════════════════════════════════════════════════════════"
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

# Regla para limpiar archivos generados
clean:
	@echo "Limpiando archivos generados..."
	rm -f $(TARGET)
	@echo "✓ Limpieza completada"

# Ayuda
help:
	@echo "════════════════════════════════════════════════════════════════════"
	@echo "  Makefile - Tutorial Completo de PDI con OpenCV"
	@echo "════════════════════════════════════════════════════════════════════"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make          - Compila el tutorial"
	@echo "  make clean    - Elimina el ejecutable"
	@echo "  make help     - Muestra esta ayuda"
	@echo "  make run IMG=<archivo> - Compila y ejecuta con una imagen"
	@echo ""
	@echo "Uso del tutorial:"
	@echo "  ./$(TARGET) imagen.jpg"
	@echo ""
	@echo "Ejemplo con make run:"
	@echo "  make run IMG=test.jpg"
	@echo ""
	@echo "Secciones del tutorial:"
	@echo "  • 13 secciones principales"
	@echo "  • Más de 50 técnicas de PDI"
	@echo "  • 12 ejercicios de manipulación de píxeles"
	@echo "  • Completamente documentado"
	@echo ""
	@echo "════════════════════════════════════════════════════════════════════"

# Compilar y ejecutar (requiere especificar IMG=archivo)
run: $(TARGET)
	@if [ -z "$(IMG)" ]; then \
		echo "Error: Debes especificar una imagen con IMG=archivo"; \
		echo "Ejemplo: make run IMG=test.jpg"; \
		exit 1; \
	fi
	@if [ ! -f "$(IMG)" ]; then \
		echo "Error: La imagen '$(IMG)' no existe"; \
		exit 1; \
	fi
	@echo ""
	@echo "════════════════════════════════════════════════════════════════════"
	@echo "  Ejecutando tutorial con: $(IMG)"
	@echo "════════════════════════════════════════════════════════════════════"
	@echo ""
	./$(TARGET) $(IMG)

# Verificar dependencias
check:
	@echo "Verificando dependencias..."
	@echo ""
	@echo -n "OpenCV 4: "
	@pkg-config --exists opencv4 && echo "✓ Instalado" || echo "✗ No encontrado"
	@pkg-config --exists opencv4 && echo "  Versión: $$(pkg-config --modversion opencv4)"
	@echo ""
	@echo -n "GStreamer: "
	@pkg-config --exists gstreamer-1.0 && echo "✓ Instalado" || echo "✗ No encontrado"
	@pkg-config --exists gstreamer-1.0 && echo "  Versión: $$(pkg-config --modversion gstreamer-1.0)"
	@echo ""
	@echo -n "Compilador C++: "
	@which $(CXX) > /dev/null && echo "✓ $(CXX) disponible" || echo "✗ No encontrado"
	@which $(CXX) > /dev/null && echo "  Versión: $$($(CXX) --version | head -1)"
	@echo ""
