#ifndef BIBLIOTECA_MAGICA_NODOINDICEISBN_H
#define BIBLIOTECA_MAGICA_NODOINDICEISBN_H

#include <string>
#include "../Modelos/Libro.h"

class NodoIndiceISBN {
public:
    std::string isbn;      // clave principal del índice
    Libro* libro;          // referencia directa al objeto Libro

    NodoIndiceISBN* izquierdo;
    NodoIndiceISBN* derecho;
    int altura;

    NodoIndiceISBN(const std::string& isbn, Libro* libro)
        : isbn(isbn),
          libro(libro),
          izquierdo(nullptr),
          derecho(nullptr),
          altura(1) {}
};

#endif //BIBLIOTECA_MAGICA_NODOINDICEISBN_H

