#ifndef BIBLIOTECA_MAGICA_NODOB_H
#define BIBLIOTECA_MAGICA_NODOB_H

#include  "../Modelos/Libro.h"

const int T = 3; //grado minimo del arbol

class NodoB {
    public:
    int numClaves;
    int fechas[2*T - 1];
    Libro* libros[2*T - 1];
    NodoB* hijos[2*T];
    bool esHoja;

    NodoB(bool hoja) {
        esHoja = hoja;
        numClaves = 0;
        for (int i = 0; i < 2*T; i++) {
            hijos[i] = nullptr;
            if (i < 2*T - 1) {
                fechas[i] = 0;
                libros[i] = nullptr;
            }
        }
    }
};

#endif //BIBLIOTECA_MAGICA_NODOB_H