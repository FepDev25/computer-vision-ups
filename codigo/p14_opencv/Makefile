TARGET = main
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

CPPFLAGS = -I"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/include/opencv4"

LDFLAGS = -L"$(HOME)/Documentos/universidad/universidad 7mo/vision por computador/opencv-dev/install/lib"

LDLIBS = $(shell pkg-config --libs-only-l opencv4) \
         $(shell pkg-config --libs gstreamer-1.0 gstreamer-base-1.0 gstreamer-video-1.0 gstreamer-app-1.0)

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) main.cpp -o $(TARGET) $(LDFLAGS) $(LDLIBS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) captura_mano_*.jpg

debug: CXXFLAGS += -g -O0
debug: clean $(TARGET)
