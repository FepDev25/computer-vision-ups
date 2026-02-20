#include <iostream>
#include "Calculadora.hpp"

using namespace std;

int main() {
    Calculadora calc;
    
    cout << "=== Calculadora Simple ===" << endl;
    cout << "5 + 3 = " << calc.sumar(5, 3) << endl;
    cout << "5 - 3 = " << calc.restar(5, 3) << endl;
    cout << "5 * 3 = " << calc.multiplicar(5, 3) << endl;
    cout << "5 / 3 = " << calc.dividir(5, 3) << endl;
    
    return 0;
}
