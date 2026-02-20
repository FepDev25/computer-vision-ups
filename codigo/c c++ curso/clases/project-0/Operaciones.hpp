#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib> // Para std::rand() y std::srand()

#include <sstream>
#include <fstream>

#include <filesystem> // C++17 para operaciones con el sistema de archivos

using namespace std; // Evitar usar "using namespace std;" en proyectos grandes

class Operaciones {

    private:
        string espacios(int n);

    public:
        Operaciones();
        void saludo(string = "Hola mundo desde C++");
        string piramideZoroastro();
        vector<string> listarDurectorio(string ruta = ".");

};

class Utilidades {
    public:
        string listarDirectorio(string ruta = ".");
};


