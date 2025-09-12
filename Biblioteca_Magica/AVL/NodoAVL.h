#ifndef BIBLIOTECA_MAGICA_NODOAVL_H
#define BIBLIOTECA_MAGICA_NODOAVL_H

#include "../Modelos/Libro.h"

class NodoAVL {
public:
    Libro dato;
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(const Libro& libro) : dato(libro) , izquierdo(nullptr), derecho(nullptr), altura(1) {}
};

#endif //BIBLIOTECA_MAGICA_NODOAVL_H