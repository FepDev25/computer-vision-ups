#include "Geometria.hpp"
#include <cmath>

const double PI = 3.14159265358979323846;

// Implementación de la clase Circulo
Circulo::Circulo(double r) : radio(r) {}

double Circulo::calcularArea() {
    return PI * radio * radio;
}

double Circulo::calcularPerimetro() {
    return 2 * PI * radio;
}

double Circulo::getRadio() {
    return radio;
}

// Implementación de la clase Rectangulo
Rectangulo::Rectangulo(double b, double a) : base(b), altura(a) {}

double Rectangulo::calcularArea() {
    return base * altura;
}

double Rectangulo::calcularPerimetro() {
    return 2 * (base + altura);
}
