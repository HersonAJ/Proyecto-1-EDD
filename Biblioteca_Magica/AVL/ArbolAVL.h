#ifndef PROYECTO1QT_ARBOLAVL_H
#define PROYECTO1QT_ARBOLAVL_H

#include "NodoAVL.h"

class ArbolAVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* nodo) const;
    int factorBalance(NodoAVL* nodo) const;
    NodoAVL* nodoMinimo(NodoAVL* nodo) const;

    NodoAVL* insertarNodo(NodoAVL* nodo, const Libro& libro);
    void destruir(NodoAVL* nodo);

public:
    ArbolAVL();
    ~ArbolAVL();

    void insertar(const Libro& libro);
    void guardarComoDOT(const std::string& ruta) const;

    NodoAVL* getRaiz() const {
        return raiz;
    }

    bool estaVacio() const { return  raiz == nullptr;}

    NodoAVL* buscar(const Libro& libro, NodoAVL* nodo);
};

#endif //PROYECTO1QT_ARBOLAVL_H