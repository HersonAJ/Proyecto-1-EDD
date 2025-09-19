#ifndef BIBLIOTECA_MAGICA_ARBOLB_H
#define BIBLIOTECA_MAGICA_ARBOLB_H

#include  "NodoB.h"
#include "ListaLibros.h"

class ArbolB {
private:
    NodoB* raiz;

    //metodos internos
    void dividirHijo(NodoB* padre, int i, NodoB* hijo);
    void insertarNoLleno(NodoB* nodo, Libro* libro);
    void buscarRangoRecursivo(NodoB* nodo, int inicio, int fin, ListaLibros& resultados);
    void recorrer(NodoB* nodo);
    //Libro* buscar(NodoB* nodo, int fecha);

public:
    ArbolB(): raiz(nullptr) {}

    void insertar(Libro* libro);
    ListaLibros buscarPorRango(int inicio, int fin);
    void imprimir();

    NodoB* getRaiz() const { return raiz; }
};

#endif //BIBLIOTECA_MAGICA_ARBOLB_H