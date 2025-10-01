#ifndef BIBLIOTECA_MAGICA_ARBOLBPLUS_H
#define BIBLIOTECA_MAGICA_ARBOLBPLUS_H

#include <string>
#include "NodoBPlus.h"
#include "NodoInterno.h"
#include "NodoHoja.h"
#include "../Modelos/Libro.h"

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

public:
    ArbolBPlus();
    ~ArbolBPlus();

    // Inserción pública
    void insertar(Libro* libro);

    // Accesores básicos
    NodoBPlus* getRaiz() const { return raiz; }
    NodoHoja* getPrimeraHoja() const { return primeraHoja; }

    static void recorrerEstructura(NodoBPlus* nodo, int nivel = 0);
};

#endif // BIBLIOTECA_MAGICA_ARBOLBPLUS_H
