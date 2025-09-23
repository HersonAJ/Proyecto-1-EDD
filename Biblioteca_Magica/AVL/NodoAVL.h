#ifndef BIBLIOTECA_MAGICA_NODOAVL_H
#define BIBLIOTECA_MAGICA_NODOAVL_H

#include "../Modelos/Libro.h"
#include "../include/ListaRepetidos.h"

class NodoAVL {
public:
    Libro* libro; //libro principal en el nodo
    ListaRepetidos repetidos; //lista doble enlazada para libros repetidos
    NodoAVL* izquierdo;
    NodoAVL* derecho;
    int altura;

    NodoAVL(Libro* libro) : libro(libro) , izquierdo(nullptr), derecho(nullptr), altura(1) {}

    //metod auxiliar para agregar libro repetido a la lista
    void agregarRepetido(Libro* libro) {
        repetidos.insertar(libro);
    }

    bool eliminarRepetido(Libro* libro) {
        return  repetidos.eliminar(libro);
    }

    int contarEjemplares() const {
        return 1 + repetidos.size();
    }
};

#endif //BIBLIOTECA_MAGICA_NODOAVL_H