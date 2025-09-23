#ifndef PROYECTO1QT_ARBOLAVL_H
#define PROYECTO1QT_ARBOLAVL_H

#include "NodoAVL.h"
#include <string>

class ArbolAVL {
private:
    NodoAVL* raiz;

    int altura(NodoAVL* nodo) const;
    int factorBalance(NodoAVL* nodo) const;
    NodoAVL* nodoMinimo(NodoAVL* nodo) const;

    NodoAVL* insertarNodo(NodoAVL* nodo, Libro* libro);
    NodoAVL* eliminarNodo(NodoAVL* nodo, Libro* libro);
    NodoAVL* buscarNodo(NodoAVL* nodo, const std::string& titulo, const std::string& isbn);

    void destruir(NodoAVL* nodo);

    //rotaciones
    NodoAVL* rotarIzquierda(NodoAVL* nodo);
    NodoAVL* rotarDerecha(NodoAVL* nodo);
    NodoAVL* rotacionDobleIzquierda(NodoAVL* nodo);
    NodoAVL* rotacionDobleDerecha(NodoAVL* nodo);

    //balanceo
    NodoAVL* balancear(NodoAVL* nodo);

    // Comparación por título + ISBN
    int compararLibros(Libro* a, Libro* b) const;
    int compararIsbn(Libro* a, Libro* b) const;


public:
    ArbolAVL();
    ~ArbolAVL();

    void insertar(Libro* libro);
    void eliminar(Libro* libro);
    //NodoAVL* buscar(const std::string& titulo, const std::string& isbn);
    NodoAVL* buscar(const std::string& titulo, const std::string& isbn = "");

    bool estaVacio() const { return raiz == nullptr; }
    NodoAVL* getRaiz() const { return raiz; }

    void guardarComoDOT(const std::string& ruta) const;
};

#endif // PROYECTO1QT_ARBOLAVL_H
