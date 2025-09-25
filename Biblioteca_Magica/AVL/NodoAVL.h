#ifndef BIBLIOTECA_MAGICA_NODOAVL_H
#define BIBLIOTECA_MAGICA_NODOAVL_H

#include "../Modelos/Libro.h"
#include "../include/ListaRepetidos.h"

class NodoAVL {
public:
    Libro* libro; //libro principal en el nodo
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(Libro* libro) : libro(libro) , izquierdo(nullptr), derecho(nullptr), altura(1) {}

    int contarEjemplares() const {
        return libro ? libro->getCantidad() : 0;
    }
};

#endif //BIBLIOTECA_MAGICA_NODOAVL_H