# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

# Nombre del ejecutable
TARGET = calculadora

# Archivos fuente y objetos
SOURCES = main.cpp Calculadora.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compilar archivos objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar archivos generados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
