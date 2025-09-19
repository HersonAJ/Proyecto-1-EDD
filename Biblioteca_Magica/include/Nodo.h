#ifndef BIBLIOTECA_MAGICA_NODO_H
#define BIBLIOTECA_MAGICA_NODO_H

#include "../Modelos/Libro.h"

class Nodo {
    public:
    Libro* libro;
    Nodo* siguiente;

    Nodo(Libro* libro) : libro(libro), siguiente(nullptr) {}
};

#endif //BIBLIOTECA_MAGICA_NODO_H