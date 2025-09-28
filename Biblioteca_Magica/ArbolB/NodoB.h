#ifndef BIBLIOTECA_MAGICA_NODOB_H
#define BIBLIOTECA_MAGICA_NODOB_H

#include  "../Modelos/Libro.h"
#include "../AVL_Auxiliar/IndiceISBN.h"

const int T = 3; //grado minimo del arbol

class EntradaFecha {
    public:
    int fecha;
    IndiceISBN indiceISBN;

    EntradaFecha() : fecha(0){}
    EntradaFecha(int f) : fecha(f) {}
};


class NodoB {
    public:
    int numClaves;
    EntradaFecha* claves[2 * T - 1]; // cada clave es una fecha con su índice ISBN
    NodoB* hijos[2 * T];
    bool esHoja;

    NodoB(bool hoja) {
        esHoja = hoja;
        numClaves = 0;
        for (int i = 0; i < 2 * T; i++) {
            hijos[i] = nullptr;
            if (i < 2 * T - 1) {
                claves[i] = nullptr;
            }
        }
    }
};

#endif //BIBLIOTECA_MAGICA_NODOB_H