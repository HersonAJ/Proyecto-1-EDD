#ifndef PROYECTO1QT_ARBOLAVL_H
#define PROYECTO1QT_ARBOLAVL_H

#include "NodoAVL.h"
#include <string>
#include "ListaEncontrados.h" //lista simple para la busqueda
#include  "../ArbolB/ListaLibros.h" //lista para la vista ordenada alfaveticamente

class IndiceISBN;

class ArbolAVL {
private:
    NodoAVL* raiz;

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
    void buscarTodosPorTituloRecursivo(NodoAVL* nodo, const std::string& titulo, ListaEncontados* lista);

    void recorrerEnOrdenRecursivo(NodoAVL* nodo, ListaLibros* lista) const;

public:
    ArbolAVL();
    ~ArbolAVL();

    void insertar(Libro* libro);
    //void eliminarPorISBN(const std::string& isbn, IndiceISBN& indiceGlobal);
    void eliminarPorISBN(const std::string& isbn, const std::string& titulo);
    //NodoAVL* buscar(const std::string& titulo, const std::string& isbn);
    NodoAVL* buscar(const std::string& titulo, const std::string& isbn = "");

    bool estaVacio() const { return raiz == nullptr; }
    NodoAVL* getRaiz() const { return raiz; }

    void guardarComoDOT(const std::string& ruta) const;

    Libro* buscarPorTitulo(const std::string& titulo);
    ListaEncontados* buscarTodosPorTitulo(const std::string& titulo);
    ListaLibros* obtenerLibrosEnOrdenAlfabetico() const;

};

#endif // PROYECTO1QT_ARBOLAVL_H
