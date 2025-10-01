#include "ArbolBPlus.h"
#include <iostream>

void ArbolBPlus::recorrerEstructura(NodoBPlus* nodo, int nivel) {
    if (!nodo) return;

    for (int i = 0; i < nivel; i++) std::cout << "   ";

    if (nodo->esHoja) {
        NodoHoja* hoja = (NodoHoja*)nodo;
        std::cout << "Hoja (nivel " << nivel << "): ";
        for (int i = 0; i < hoja->numClaves; i++) {
            std::cout << "[" << hoja->entradas[i].genero << "] ";
        }
        std::cout << "\n";
    } else {
        NodoInterno* interno = (NodoInterno*)nodo;
        std::cout << "Interno (nivel " << nivel << "): ";
        for (int i = 0; i < interno->numClaves; i++) {
            std::cout << interno->claves[i] << " ";
        }
        std::cout << "\n";

        for (int i = 0; i <= interno->numClaves; i++) {
            recorrerEstructura(interno->hijos[i], nivel + 1);
        }
    }
}

