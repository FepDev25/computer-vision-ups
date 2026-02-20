CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Rutas de OpenCV personalizado
CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"
LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"
LDLIBS = $(shell pkg-config --libs-only-l opencv4) \
         $(shell pkg-config --libs gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

# Ruta a la librería LBP
LBP_DIR = ../../segundo-interciclo/local_binnary_patterns/lbp-library
LBP_INCLUDE = -I$(LBP_DIR)

# Archivos fuente de la librería LBP
LBP_SOURCES = $(LBP_DIR)/histogram.cpp \
              $(LBP_DIR)/package_lbp/olbp/OLBP.cpp \
              $(LBP_DIR)/package_lbp/elbp/ELBP.cpp \
              $(LBP_DIR)/package_lbp/varlbp/VARLBP.cpp \
              $(LBP_DIR)/package_lbp/cslbp/CSLBP.cpp \
              $(LBP_DIR)/package_lbp/csldp/CSLDP.cpp \
              $(LBP_DIR)/package_lbp/xcslbp/XCSLBP.cpp \
              $(LBP_DIR)/package_lbp/siltp/SILTP.cpp \
              $(LBP_DIR)/package_lbp/cssiltp/CSSILTP.cpp \
              $(LBP_DIR)/package_lbp/scslbp/SCSLBP.cpp \
              $(LBP_DIR)/package_lbp/bglbp/BGLBP.cpp \
              $(LBP_DIR)/package_lbp/oclbp/OCLBP.cpp

# Archivos objeto
LBP_OBJECTS = $(LBP_SOURCES:.cpp=.o)

TARGET = main
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCE) $(LBP_OBJECTS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LBP_INCLUDE) $(SOURCE) $(LBP_OBJECTS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

# Compilar archivos objeto de la librería LBP
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LBP_INCLUDE) -c $< -o $@

clean:
	rm -f $(TARGET) $(LBP_OBJECTS)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
