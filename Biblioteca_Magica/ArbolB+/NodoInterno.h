#ifndef BIBLIOTECA_MAGICA_NODOINTERNO_H
#define BIBLIOTECA_MAGICA_NODOINTERNO_H

#include "NodoBPlus.h"

class NodoInterno: public NodoBPlus {
    public:
    std::string* claves;
    NodoBPlus** hijos;

    NodoInterno();
    ~NodoInterno();

    //implementacion de metodos de herencia
    bool estaLeno() const override;
    bool estaVacio() const override;
    int encontrarPosicion(const std::string &genero) const override;
};

#endif //BIBLIOTECA_MAGICA_NODOINTERNO_H
