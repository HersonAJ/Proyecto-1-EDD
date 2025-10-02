#ifndef BIBLIOTECA_MAGICA_ARBOLBPLUS_H
#define BIBLIOTECA_MAGICA_ARBOLBPLUS_H

#include <string>
#include "NodoBPlus.h"
#include "NodoInterno.h"
#include "NodoHoja.h"
#include "../Modelos/Libro.h"
#include "../ArbolB/ListaLibros.h"
#include "../AVL_Auxiliar/IndiceISBN.h"

class ArbolBPlus {
private:
    NodoBPlus* raiz;        // raíz del árbol (puede ser hoja o interno)
    NodoHoja* primeraHoja;  // puntero a la primera hoja (para recorridos secuenciales)

    // Métodos auxiliares para inserción
    NodoHoja* buscarHoja(const std::string& genero) const;   // encuentra la hoja donde insertar
    void insertarEnHoja(NodoHoja* hoja, Libro* libro);       // inserta un libro en la hoja
    void dividirHoja(NodoHoja* hoja);                        // divide una hoja llena
    void dividirInterno(NodoInterno* interno);               // divide un nodo interno lleno

    void liberarRecursivo(NodoBPlus* nodo);
    NodoInterno* buscarPadre(NodoBPlus* actual, NodoBPlus* hijo) const;
    void recorrerAVLyAgregarLibros(NodoIndiceISBN* nodoAVL, ListaLibros* resultados) const;
    bool buscarGeneroAux(const std::string& genero, NodoHoja** hojaOut, int& pos) const;//metodo auxiliar para la eliminacion

public:
    ArbolBPlus();
    ~ArbolBPlus();

    void insertarSoloGenero(const std::string& genero);
    void insertarLibroEnGenero(Libro* libro);

    // Accesores básicos
    NodoBPlus* getRaiz() const { return raiz; }
    NodoHoja* getPrimeraHoja() const { return primeraHoja; }

    static void recorrerEstructura(NodoBPlus* nodo, int nivel = 0);
    void eliminarPorISBN(const std::string& isbn, IndiceISBN& indiceGeneral);
    ListaLibros* buscarPorGenero(const std::string& genero) const; //implementaicion para la accion de busqueda del usuario
};

#endif // BIBLIOTECA_MAGICA_ARBOLBPLUS_H
