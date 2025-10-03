#ifndef BIBLIOTECA_MAGICA_INDICEISBN_H
#define BIBLIOTECA_MAGICA_INDICEISBN_H

#include "NodoIndiceISBN.h"
#include <string>

// Árbol AVL que funciona como índice global por ISBN
class IndiceISBN {
private:
    NodoIndiceISBN* raiz;

    int altura(const NodoIndiceISBN* nodo) const;
    int factorBalance(const NodoIndiceISBN* nodo) const;
    NodoIndiceISBN* balancear(NodoIndiceISBN* nodo);

    NodoIndiceISBN* rotarIzquierda(NodoIndiceISBN* nodo);
    NodoIndiceISBN* rotarDerecha(NodoIndiceISBN* nodo);

    NodoIndiceISBN* insertarNodo(NodoIndiceISBN* nodo, const std::string& isbn, Libro* libro);
    NodoIndiceISBN* eliminarNodo(NodoIndiceISBN* nodo, const std::string& isbn);
    NodoIndiceISBN* nodoMinimo(NodoIndiceISBN* nodo) const;

    void destruirRecursivo(const NodoIndiceISBN* nodo);

public:
    IndiceISBN();
    ~IndiceISBN();

    void insertar(const std::string& isbn, Libro* libro);
    void eliminar(const std::string& isbn);
    //Libro* buscar(const std::string& isbn);
    Libro* buscar(const std::string& isbn) const;

    bool estaVacio() const;
    NodoIndiceISBN* getRaiz() const { return raiz; }
};

#endif //BIBLIOTECA_MAGICA_INDICEISBN_H
