#ifndef BIBLIOTECA_MAGICA_LISTADOBLE_H
#define BIBLIOTECA_MAGICA_LISTADOBLE_H

#include "../Modelos/Libro.h"

class NodoLibro {
    public:
    Libro* libro;
    NodoLibro* anterior;
    NodoLibro* siguiente;

    NodoLibro(Libro* libro) : libro(libro), anterior(nullptr), siguiente(nullptr) {}
};


class ListaRepetidos {
    private:
    NodoLibro* cabeza;
    NodoLibro* cola;
    int cantidad;

    public:
    ListaRepetidos();
    ~ListaRepetidos();

    void insertar(Libro* libro);
    bool eliminar(Libro* libro);
    int size() const;

    //recorrido
    NodoLibro* getCabeza()const;
};
#endif //BIBLIOTECA_MAGICA_LISTADOBLE_H