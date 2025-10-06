#include <iostream>
#include "ArbolAVL.h"

// Comparación por título + ISBN
int ArbolAVL::compararLibros(Libro* a, Libro* b) const {
    if (!a || !b) throw std::invalid_argument("Libro nulo en comparación");
    int cmp = a->getTitulo().compare(b->getTitulo());
    if (cmp == 0) {
        return a->getIsbn().compare(b->getIsbn());
    }
    return cmp;
}

// Insertar nodo recursivo
NodoAVL* ArbolAVL::insertarNodo(NodoAVL* nodo, Libro* libro) {
    if (!libro) throw std::invalid_argument("Libro nulo en insertarNodo");
    if (nodo == nullptr) {
        return new NodoAVL(libro);
    }

    int cmp = compararLibros(libro, nodo->libro);

    if (cmp < 0) {
        nodo->izquierdo = insertarNodo(nodo->izquierdo, libro);
    } else if (cmp > 0) {
        nodo->derecho = insertarNodo(nodo->derecho, libro);
    } else {
        // Libro repetido, incrementar ejemplares
        nodo->libro->incrementarCantidad();
        return nodo;
    }

    // Actualizar altura
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return balancear(nodo);
}

// Método público
void ArbolAVL::insertar(Libro* libro) {
    try {
        if (!libro) throw std::invalid_argument("Intento de insertar libro nulo");
        raiz = insertarNodo(raiz, libro);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::insertar: " << e.what() << std::endl;
        throw;
    }
}