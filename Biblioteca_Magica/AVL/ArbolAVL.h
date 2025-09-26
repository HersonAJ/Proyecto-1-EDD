#ifndef PROYECTO1QT_ARBOLAVL_H
#define PROYECTO1QT_ARBOLAVL_H

#include "NodoAVL.h"
#include "AVL_Auxiliar.h"
#include <string>
#include "ListaEncontrados.h"

class ArbolAVL {
private:
    NodoAVL* raiz;
    AVL_Auxiliar arbolAuxiliar;

    int altura(NodoAVL* nodo) const;
    int factorBalance(NodoAVL* nodo) const;
    NodoAVL* nodoMinimo(NodoAVL* nodo) const;

    NodoAVL* insertarNodo(NodoAVL* nodo, Libro* libro);
    NodoAVL* eliminarNodoEficiente(NodoAVL* nodo, const std::string& titulo, const std::string& isbn);
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

    Libro* buscarPorTituloRecursivo(NodoAVL* nodo, const std::string& titulo);
    void buscarTodosPorTituloRecursivo(NodoAVL* nodo, const std::string& titulo, ListaEcontados* lista);


public:
    ArbolAVL();
    ~ArbolAVL();

    void insertar(Libro* libro);
    void eliminarPorISBN(const std::string& isbn);
    //NodoAVL* buscar(const std::string& titulo, const std::string& isbn);
    NodoAVL* buscar(const std::string& titulo, const std::string& isbn = "");

    bool estaVacio() const { return raiz == nullptr; }
    NodoAVL* getRaiz() const { return raiz; }

    void guardarComoDOT(const std::string& ruta) const;

    Libro* buscarPorTitulo(const std::string& titulo);
    ListaEcontados* buscarTodosPorTitulo(const std::string& titulo);

};

#endif // PROYECTO1QT_ARBOLAVL_H
