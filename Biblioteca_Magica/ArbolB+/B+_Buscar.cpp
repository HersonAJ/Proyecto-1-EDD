#include "ArbolBPlus.h"
#include <iostream>

//busqueda por genero
ListaLibros* ArbolBPlus::buscarPorGenero(const std::string& genero) const {
    ListaLibros* resultados = new ListaLibros();

    NodoHoja* hoja = buscarHoja(genero);

    for (int i = 0; i < hoja->numClaves; i++) {
        if (hoja->entradas[i].genero == genero) {
            if (hoja->entradas[i].indiceISBN != nullptr) {
                recorrerAVLyAgregarLibros(hoja->entradas[i].indiceISBN->getRaiz(), resultados);
            }
            break;
        }
    }

    return resultados;
}


void ArbolBPlus::recorrerAVLyAgregarLibros(NodoIndiceISBN* nodoAVL, ListaLibros* resultados) const {
    if (!nodoAVL) return;

    recorrerAVLyAgregarLibros(nodoAVL->izquierdo, resultados);
    resultados->insertar(nodoAVL->libro);
    recorrerAVLyAgregarLibros(nodoAVL->derecho, resultados);
}


//metodo auxiliar de busqueda para la eliminacion
bool ArbolBPlus::buscarGeneroAux(const std::string& genero, NodoHoja** hojaOut, int& pos) const {
    //localizar la hoja
    NodoHoja* hoja = buscarHoja(genero);

    //buscar el genero dentro de la hoja
    for (int i = 0; i < hoja->numClaves; i++) {
        if (hoja->entradas[i].genero == genero) {
            *hojaOut = hoja;
            pos = i;
            return true;
        }
    }

    //no se encontro
    *hojaOut = nullptr;
    pos = -1;
    return false;
}