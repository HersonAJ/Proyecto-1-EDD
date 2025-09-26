#include <iostream>
#include "ArbolAVL.h"

// Comparación por título + ISBN
int ArbolAVL::compararLibros(Libro* a, Libro* b) const {
    int cmp = a->getTitulo().compare(b->getTitulo());
    if (cmp == 0) {
        return a->getIsbn().compare(b->getIsbn());
    }
    return cmp;
}

// Insertar nodo recursivo
NodoAVL* ArbolAVL::insertarNodo(NodoAVL* nodo, Libro* libro) {
    if (nodo == nullptr) {
        std::cout << "Insertando nuevo nodo: " << libro->getTitulo() << std::endl;
        return new NodoAVL(libro);
    }

    int cmp = compararLibros(libro, nodo->libro);

    if (cmp < 0) {
        nodo->izquierdo = insertarNodo(nodo->izquierdo, libro);
    } else if (cmp > 0) {
        nodo->derecho = insertarNodo(nodo->derecho, libro);
    } else {
        std::cout << "Libro repetido, incrementando ejemplares: " << libro->getTitulo() << std::endl;
        nodo->libro->incrementarCantidad();
        return nodo;
    }

    // Actualizar altura
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    //std::cout << "Balanceado nodo: " << nodo->libro->getTitulo() << std::endl;
    return balancear(nodo);
}

// Método público
void ArbolAVL::insertar(Libro* libro) {
    raiz = insertarNodo(raiz, libro);

    NodoAVL* nodoInsertado = buscar(libro->getTitulo(), libro->getIsbn());

    if (nodoInsertado) {
        arbolAuxiliar.insertar(libro->getIsbn(), nodoInsertado);
    }
}
