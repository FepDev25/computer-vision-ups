#include <iostream>
#include <iomanip>
#include "Geometria.hpp"

using namespace std;

int main() {
    cout << fixed << setprecision(2);
    
    cout << "=== Cálculos Geométricos ===" << endl << endl;
    
    // Crear un círculo
    Circulo miCirculo(5.0);
    cout << "Círculo con radio " << miCirculo.getRadio() << ":" << endl;
    cout << "  Área: " << miCirculo.calcularArea() << endl;
    cout << "  Perímetro: " << miCirculo.calcularPerimetro() << endl << endl;
    
    // Crear un rectángulo
    Rectangulo miRectangulo(4.0, 6.0);
    cout << "Rectángulo con base 4.0 y altura 6.0:" << endl;
    cout << "  Área: " << miRectangulo.calcularArea() << endl;
    cout << "  Perímetro: " << miRectangulo.calcularPerimetro() << endl;
    
    return 0;
}
