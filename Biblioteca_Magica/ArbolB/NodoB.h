#ifndef BIBLIOTECA_MAGICA_NODOB_H
#define BIBLIOTECA_MAGICA_NODOB_H

#include  "../Modelos/Libro.h"

const int T = 3; //grado minimo del arbol

class NodoB {
    public:
    int numClaves;  //numero actual de claves en el nodo
    Libro* claves[2*T -1]; // arreglo de punteros a libros (max. 2T - 1 claves)
    NodoB* hijos[2*T];  //punteros a hijos (max 2T hijos)
    bool estHoja;   //verifica si es hoja o no

    NodoB(bool hoja) {
        estHoja = hoja;
        numClaves = 0;
        for (int i = 0; i < 2*T; i++) {
            hijos[i] = nullptr;
        }

        for (int i = 0; i < 2*T -1; i++) {
            claves[i] = nullptr;
        }
    }
};

#endif //BIBLIOTECA_MAGICA_NODOB_H