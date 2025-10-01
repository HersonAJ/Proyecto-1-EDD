#include "NodoHoja.h"

NodoHoja::NodoHoja() : NodoBPlus(true) {
    numClaves = 0;
    entradas = new EntradaGenero[2 * T_BPLUS - 1];
    siguiente = nullptr;
    anterior = nullptr;
}
NodoHoja::~NodoHoja() {
    delete[] entradas;
}
