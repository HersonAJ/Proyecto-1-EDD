#include "NodoInterno.h"

NodoInterno::NodoInterno() : NodoBPlus(false) {
    numClaves = 0;
    claves = new std::string[2 * T_BPLUS - 1];
    hijos = new NodoBPlus*[2 * T_BPLUS];
    for (int i = 0; i < 2 * T_BPLUS; i++) {
        hijos[i] = nullptr;
    }
}
NodoInterno::~NodoInterno() {
    delete[] claves;
    delete[] hijos;
}
