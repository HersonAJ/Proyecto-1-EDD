#ifndef BIBLIOTECA_MAGICA_NODOHOJA_H
#define BIBLIOTECA_MAGICA_NODOHOJA_H

#include "NodoBPlus.h"
#include "../AVL_Auxiliar/IndiceISBN.h"
#include <string>

// Nodo hoja de B+ — almacena claves reales (género) y datos (IndiceISBN)
// además mantiene enlaces a hojas vecinas para recorrido secuencial.
class NodoHoja : public NodoBPlus {
public:
    struct EntradaGenero {
        std::string genero;
        IndiceISBN indiceISBN;

        EntradaGenero() : genero("") {}
        explicit EntradaGenero(const std::string& g) : genero(g) {}
    };

    EntradaGenero* entradas; // tamaño (2*T_BPLUS - 1)
    NodoHoja* siguiente;      // enlace a la hoja siguiente (nullptr si no existe)
    NodoHoja* anterior;       // enlace a la hoja anterior (opcional)

    NodoHoja();
    ~NodoHoja() override;


    int buscarIndiceEntrada(const std::string& genero) const; // índice dónde insertar/buscar
};

#endif // BIBLIOTECA_MAGICA_NODOHOJA_H
