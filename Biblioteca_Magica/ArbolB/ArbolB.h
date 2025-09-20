#ifndef BIBLIOTECA_MAGICA_ARBOLB_H
#define BIBLIOTECA_MAGICA_ARBOLB_H

#include  "NodoB.h"
#include "ListaLibros.h"

class ArbolB {
private:
    NodoB* raiz;

    //metodos internos
    void dividirHijo(NodoB* padre, int i, NodoB* hijo);
    void insertarNoLleno(NodoB* nodo, Libro* libro);
    void buscarRangoRecursivo(NodoB* nodo, int inicio, int fin, ListaLibros& resultados);
    void recorrer(NodoB* nodo);
    //Libro* buscar(NodoB* nodo, int fecha);
    //metodos para la eliminacion
    void eliminarRecursivo(NodoB* nodo, int fecha);
    void eliminarDeHoja(NodoB* nodo, int idx);
    void eliminarDeInterno(NodoB* nodo, int indx);
    Libro* obtenerPredecesor(NodoB* nodo, int idx);
    Libro* obtenerSucesor(NodoB* nodo, int idx);
    void llenar(NodoB* nodo, int idx);
    void prestarDeAnterior(NodoB* nodo, int idx);
    void prestarDeSiguiente(NodoB* nodo, int idx);
    void fusionar(NodoB* nodo, int idx);

public:
    ArbolB(): raiz(nullptr) {}

    void insertar(Libro* libro);
    ListaLibros buscarPorRango(int inicio, int fin);
    void imprimir();

    NodoB* getRaiz() const { return raiz; }
    void eliminar(int fecha);
};

#endif //BIBLIOTECA_MAGICA_ARBOLB_H