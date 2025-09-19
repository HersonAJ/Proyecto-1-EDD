#include "ListaLibros.h"

ListaLibros::ListaLibros() : cabeza(nullptr), cola(nullptr), tamaño(0) {}

ListaLibros::~ListaLibros() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* temp = actual;
        actual = actual->siguiente;
        delete temp;
    }
}

void ListaLibros::insertar(Libro *libro) {
    Nodo* nuevoNodo = new Nodo(libro);

    if (estaVacia()) {
        cabeza= nuevoNodo;
        cola = nuevoNodo;
    } else {
        cola->siguiente = nuevoNodo;
        cola = nuevoNodo;
    }
    tamaño++;
}

bool ListaLibros::estaVacia() const {
    return  cabeza == nullptr;
}

int ListaLibros::getTamaño() const {
    return tamaño;
}

Nodo *ListaLibros::getCabeza() const {
    return cabeza;
}



