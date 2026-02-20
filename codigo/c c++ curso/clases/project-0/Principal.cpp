#include <iostream>
#include "Operaciones.hpp"

#include <sstream>
#include <cmath>

using namespace std;

int main() {
    // Crear objeto de la clase Operaciones
    Operaciones op;
    
    // Usar el método saludo
    op.saludo();
    op.saludo("¡Hola desde el programa principal!");

    int x2 = 33;
    stringstream ss;

    cout << op.piramideZoroastro() << endl;

    vector<string> archivos = op.listarDurectorio(".");
    cout << "Archivos en el directorio actual:" << endl;
    for (const auto& archivo : archivos) {
        cout << archivo << endl;
    }

    int x = 23;
    int *px = &x;
    cout << "Valor de x: " << x << ", Dirección de x: " << px << ", Valor apuntado por px: " << *px << endl;

    Operaciones *oobj = new Operaciones();
    oobj->saludo("Saludos desde el objeto dinámico.");

    int arreglo[3];
    arreglo[0] = 10;
    arreglo[1] = 20;
    arreglo[2] = 30;

    cout << "Arreglo estático: ";
    for (int i = 0; i < 3; i++) {
        cout << arreglo[i] << " ";
    }
    cout << endl;

    int *pa = new int[3];
    pa[0] = 100;
    pa[1] = 200;
    pa[2] = 300;
    *(pa + 1) = 250;
    *(pa + 2) = 11;

    cout << "Arreglo dinámico: ";
    for (int i = 0; i < 3; i++) {
        cout << *(pa + i) << " ";
    }
    cout << endl;

    ofstream salida("Datos.csv", ios::app);
    salida << "Cont, Valor" << endl;

    for (int i = 0; i < 100; i++)
    {
        salida << i << "; " << cos(((double) i)/10) << endl;
    }
    salida.close();
    


    delete[] pa;
    delete oobj;

    return 0;
}
