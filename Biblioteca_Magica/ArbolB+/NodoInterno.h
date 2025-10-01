#ifndef BIBLIOTECA_MAGICA_NODOINTERNO_H
#define BIBLIOTECA_MAGICA_NODOINTERNO_H

#include "NodoBPlus.h"
#include <string>

// Nodo interno de B+ — almacena claves-guía y punteros a hijos (NodoBPlus*)
class NodoInterno : public NodoBPlus {
public:
    // claves: array de (2*T_BPLUS - 1) separadores; hijos: (2*T_BPLUS) punteros
    std::string* claves;
    NodoBPlus** hijos;

    NodoInterno();
    ~NodoInterno() override;

    int buscarIndiceClave(const std::string& clave) const; // posicion para descender
};

#endif // BIBLIOTECA_MAGICA_NODOINTERNO_H
