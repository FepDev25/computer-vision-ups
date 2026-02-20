#include "Calculadora.hpp"

int Calculadora::sumar(int a, int b) {
    return a + b;
}

int Calculadora::restar(int a, int b) {
    return a - b;
}

int Calculadora::multiplicar(int a, int b) {
    return a * b;
}

double Calculadora::dividir(int a, int b) {
    if (b == 0) {
        return 0.0;
    }
    return static_cast<double>(a) / b;
}
