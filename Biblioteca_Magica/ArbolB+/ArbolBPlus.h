#ifndef BIBLIOTECA_MAGICA_ARBOLBPLUS_H
#define BIBLIOTECA_MAGICA_ARBOLBPLUS_H

#include "NodoBPlus.h"
#include "NodoInterno.h"
#include "NodoHoja.h"

class ArbolBPlus {
    private:
    NodoBPlus* raiz;

    public:
    ArbolBPlus();
    void insertar(const std::string& genero, Libro* libro);
};

#endif //BIBLIOTECA_MAGICA_ARBOLBPLUS_H