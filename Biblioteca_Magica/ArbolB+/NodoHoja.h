#ifndef BIBLIOTECA_MAGICA_NODOHOJA_H
#define BIBLIOTECA_MAGICA_NODOHOJA_H

#include "NodoBPlus.h"
#include "../AVL_Auxiliar/IndiceISBN.h"

class NodoHoja : public NodoBPlus {
    public:
    class EntradaGenero {
        public:
        std::string genero;
        IndiceISBN indiceISBN;

        EntradaGenero() : genero("") {}
        EntradaGenero(const std::string& g) : genero(g) {}
    };

    EntradaGenero* claves;
    NodoHoja* siguiente;
    NodoHoja* anterior;

    NodoHoja();
    ~NodoHoja();

    //implementacion de metodos heredados
    bool estaLeno() const override;
    bool estaVacio() const override;
    int encontrarPosicion(const std::string &genero) const override;

    //metodos propios
    bool insertarEntrada(const std::string& genero);
    IndiceISBN* buscarGenero(const std::string& genero);
};
#endif //BIBLIOTECA_MAGICA_NODOHOJA_H