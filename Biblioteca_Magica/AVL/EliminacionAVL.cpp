#include "EliminacionAVL.h"

void EliminacionAVL::eliminar(ArbolAVL& arbol, const Libro& libro) {
    arbol.setRaiz(eliminarNodo(arbol.getRaiz(), libro, arbol));
}

NodoAVL* EliminacionAVL::eliminarNodo(NodoAVL* nodo, const Libro& libro, ArbolAVL& arbol) {
    if (nodo == nullptr) return nodo; // no encontrado

    int cmp = libro.compararPorTitulo(nodo->dato);
    if (cmp < 0) {
        nodo->izquierdo = eliminarNodo(nodo->izquierdo, libro, arbol);
    } else if (cmp > 0) {
        nodo->derecho = eliminarNodo(nodo->derecho, libro, arbol);
    } else {
        // Caso 1: sin hijos
        if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
            delete nodo;
            return nullptr;
        }
        // Caso 2: un solo hijo
        else if (nodo->izquierdo == nullptr) {
            NodoAVL* temp = nodo->derecho;
            delete nodo;
            return temp;
        }
        else if (nodo->derecho == nullptr) {
            NodoAVL* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        }
        // Caso 3: dos hijos
        else {
            NodoAVL* sucesor = nodoMinimo(nodo->derecho);
            nodo->dato = sucesor->dato;
            nodo->derecho = eliminarNodo(nodo->derecho, sucesor->dato, arbol);
        }
    }
    return nodo;
}

NodoAVL* EliminacionAVL::nodoMinimo(NodoAVL* nodo) {
    NodoAVL* actual = nodo;
    while (actual && actual->izquierdo != nullptr) {
        actual = actual->izquierdo;
    }
    return actual;
}
