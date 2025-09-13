#ifndef BIBLIOTECA_MAGICA_ELIMINACIONAVL_H
#define BIBLIOTECA_MAGICA_ELIMINACIONAVL_H

#include "../AVL/ArbolAVL.h"

class EliminacionAVL {
public:
    // Elimina un libro por título de forma recursiva (sin balancear)
    static void eliminar(ArbolAVL& arbol, const Libro& libro);

private:
    // Función recursiva que elimina y devuelve el nuevo subárbol
    static NodoAVL* eliminarNodo(NodoAVL* nodo, const Libro& libro, ArbolAVL& arbol);

    // Encuentra el nodo con el valor mínimo en un subárbol
    static NodoAVL* nodoMinimo(NodoAVL* nodo);
};

#endif //BIBLIOTECA_MAGICA_ELIMINACIONAVL_H
