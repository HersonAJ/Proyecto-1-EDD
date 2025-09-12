#include "ArbolAVL.h"

//insertar al arbol de forma simple
NodoAVL* ArbolAVL::insertarNodo(NodoAVL* nodo, const Libro& libro) {
    //validando que este vacio
    if (nodo == nullptr) {
        return  new NodoAVL(libro);
    }

    //primera comparacion
    if (libro.compararPorTitulo(nodo->dato) < 0) {
        nodo->izquierdo = insertarNodo(nodo->izquierdo, libro);
    } else if (libro.compararPorTitulo(nodo->dato) > 0) {
        nodo->derecho = insertarNodo(nodo->derecho, libro);
    } else {
        return nodo;
    }

    //actualizar la altura del arbol
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return nodo;
}

void ArbolAVL::insertar(const Libro& libro) {
    raiz = insertarNodo(raiz, libro);
}