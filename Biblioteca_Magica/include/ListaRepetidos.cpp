#include "ListaRepetidos.h"

ListaRepetidos::ListaRepetidos() : cabeza(nullptr), cola(nullptr), cantidad(0) {}

ListaRepetidos::~ListaRepetidos() {
    NodoLibro* actual = cabeza;
    while (actual != nullptr) {
        NodoLibro* temp = actual;
        actual = actual->siguiente;
        delete temp; //eliminacion de nodos no libros
    }
}

void ListaRepetidos::insertar(Libro* libro) {
    NodoLibro* nuevo = new NodoLibro(libro);
    if (cabeza == nullptr) {
        cabeza = nuevo;
        cola = nuevo;
    } else {
        cola->siguiente = nuevo;
        nuevo->anterior = cola;
        cola = nuevo;
    }
    cantidad++;
}

bool ListaRepetidos::eliminar(Libro* libro) {
    NodoLibro* actual = cabeza;
    while (actual != nullptr) {
        if (actual->libro == libro) {
            if (actual->anterior) actual->anterior->siguiente = actual->siguiente;
            else cabeza = actual->siguiente;

            if (actual->siguiente) actual->siguiente->anterior = actual->anterior;
            else cola = actual->anterior;

            delete actual;
            cantidad--;
            return true;
        }
        actual = actual->siguiente;
    }
    return false;
}



int ListaRepetidos::size() const {
    return cantidad;
}

NodoLibro *ListaRepetidos::getCabeza() const {
    return cabeza;
}