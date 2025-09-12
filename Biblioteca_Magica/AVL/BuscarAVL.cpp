#include "ArbolAVL.h"
#include "NodoAVL.h"


NodoAVL* ArbolAVL::buscar(const Libro& libro, NodoAVL* nodo) {
    if (estaVacio()) return nullptr; // árbol vacío

    NodoAVL* aux = nodo;

    while (aux != nullptr) {
        int comparar = libro.compararPorTitulo(aux->dato);
        if (comparar == 0) {
            return aux; // encontrado
        } else if (comparar < 0) {
            aux = aux->izquierdo;
        } else {
            aux = aux->derecho;
        }
    }
    return nullptr; // no encontrado
}

