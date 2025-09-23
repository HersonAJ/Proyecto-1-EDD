#include "ArbolAVL.h"

void ArbolAVL::eliminar(Libro* libro) {
    raiz = eliminarNodo(raiz, libro);
}

NodoAVL* ArbolAVL::eliminarNodo(NodoAVL* nodo, Libro* libro) {
    if (nodo == nullptr) return nodo;

    int cmp = compararLibros(libro, nodo->libro);
    if (cmp < 0) {
        nodo->izquierdo = eliminarNodo(nodo->izquierdo, libro);
    } else if (cmp > 0) {
        nodo->derecho = eliminarNodo(nodo->derecho, libro);
    } else {
        // Encontrado el nodo a eliminar

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
            nodo->libro = sucesor->libro;
            nodo->derecho = eliminarNodo(nodo->derecho, sucesor->libro);
        }
    }

    // Actualizar altura y balancear
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    return balancear(nodo);
}

NodoAVL* ArbolAVL::nodoMinimo(NodoAVL* nodo) const {
    NodoAVL* actual = nodo;
    while (actual && actual->izquierdo != nullptr) {
        actual = actual->izquierdo;
    }
    return actual;
}
