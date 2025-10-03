#ifndef BIBLIOTECA_MAGICA_LISTAENCONTRADOS_H
#define BIBLIOTECA_MAGICA_LISTAENCONTRADOS_H

#include "../include/Nodo.h"
#include "../Modelos/Libro.h"

class ListaEncontados {
    private:
    Nodo* cabeza;

    public:
    ListaEncontados() : cabeza(nullptr) {}

    void agregar(Libro* libro) {
        Nodo* nuevo = new Nodo(libro);
        if (!cabeza) {
            cabeza = nuevo;
        } else {
            Nodo* actual = cabeza;
            while (actual->siguiente) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
    }

    Nodo* getCabeza() const { return cabeza; }

    ~ListaEncontados() {
        Nodo* actual = cabeza;
        while (actual) {
            Nodo* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }
};

#endif //BIBLIOTECA_MAGICA_LISTAENCONTRADOS_H