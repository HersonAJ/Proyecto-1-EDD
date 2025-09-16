#ifndef BIBLIOTECA_MAGICA_ARBOLB_H
#define BIBLIOTECA_MAGICA_ARBOLB_H

#include  "NodoB.h"

class ArbolB {
private:
    NodoB* raiz;

    //metodos internos
    void dividirHijo(NodoB* padre, int i, NodoB* hijo);
    void insertarNoLleno(NodoB* nodo, Libro* libro);
    void recorrer(NodoB* nodo);
    NodoB* buscar(NodoB nodo, int fecha);

public:
    ArbolB(): raiz(nullptr) {}

    void insertar(Libro* libro);
    Libro* buscar(int fecha);
    void imprimir();

    NodoB* getRaiz() const { return raiz; }
};

#endif //BIBLIOTECA_MAGICA_ARBOLB_H