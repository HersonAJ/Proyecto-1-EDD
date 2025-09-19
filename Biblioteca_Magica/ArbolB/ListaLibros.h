#ifndef BIBLIOTECA_MAGICA_LISTALIBROS_H
#define BIBLIOTECA_MAGICA_LISTALIBROS_H

#include "../include/Nodo.h"

class ListaLibros {
    private:
    Nodo* cabeza;
    Nodo* cola;
    int tamaño;

    public:
    ListaLibros();
    ~ListaLibros();

    void insertar(Libro* libro);
    bool estaVacia() const;
    int getTamaño() const;
    Nodo* getCabeza() const;

    //iterar sobre la lista
    class Interador {
        private:
        Nodo* actual;
        public:
        Interador(Nodo* nodo) :actual(nodo) {}

        bool tieneSiguiente() const { return  actual != nullptr; }
        Libro* siguiente() {
            Libro* libro = actual->libro;
            actual = actual->siguiente;
            return libro;
        }
    };

    Interador obtenerIterador() const { return  Interador(cabeza); }
};

#endif //BIBLIOTECA_MAGICA_LISTALIBROS_H