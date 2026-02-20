#ifndef GEOMETRIA_HPP
#define GEOMETRIA_HPP

class Circulo {
private:
    double radio;
    
public:
    Circulo(double r);
    double calcularArea();
    double calcularPerimetro();
    double getRadio();
};

class Rectangulo {
private:
    double base;
    double altura;
    
public:
    Rectangulo(double b, double a);
    double calcularArea();
    double calcularPerimetro();
};

#endif // GEOMETRIA_HPP
