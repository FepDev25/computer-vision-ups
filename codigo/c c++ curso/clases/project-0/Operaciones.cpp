#include "Operaciones.hpp"
#include <sstream>

using namespace std;

Operaciones::Operaciones() {
    cout << "Objeto Operaciones creado." << endl;
}

void Operaciones::saludo(string mensaje) {
    cout << mensaje << endl;
}

string Operaciones::espacios(int n) {
    return string(n, ' ');
}

string Operaciones::piramideZoroastro(){
    stringstream ss;
    stringstream ss2;

    string linea = "";
    string total = "";

    for (int i = 1; i < 10; i++)
    {
        ss.str(""); // Limpiar el contenido del stringstream
        ss.clear();

        ss2.str("");
        ss2.clear();

        ss << i;
        
        int numeroLinea = stoi(linea + ss.str());
        int resultado = numeroLinea * 8 + i;
        
        ss2 << resultado;

        linea += ss.str();
        total += espacios(10-i) + linea + " * 8 + " + ss.str() + " = " + ss2.str() + "\n";

        
    }

    return total;
    
}

vector<string> Operaciones::listarDurectorio(string ruta) {
    vector<string> lista;

    for (const auto &entrada : filesystem::directory_iterator(ruta)) { // & 
        lista.push_back(entrada.path().string());
    }

    return lista;
}
