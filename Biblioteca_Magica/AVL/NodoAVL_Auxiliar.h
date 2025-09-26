#ifndef BIBLIOTECA_MAGICA_NODOAVL_AUXILIAR_H
#define BIBLIOTECA_MAGICA_NODOAVL_AUXILIAR_H

#include  <string>
#include "NodoAVL.h"

class NodoAVL_Auxiliar {
    public:
    std::string isbn;
    NodoAVL* referencia;
    NodoAVL_Auxiliar* izquierdo;
    NodoAVL_Auxiliar* derecho;
    int altura;

    NodoAVL_Auxiliar(const std::string& isbn, NodoAVL* ref) :
    isbn(isbn), referencia(ref), izquierdo(nullptr), derecho(nullptr), altura(1) {}
};

#endif //BIBLIOTECA_MAGICA_NODOAVL_AUXILIAR_H