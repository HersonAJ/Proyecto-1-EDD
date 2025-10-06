#include "ArbolBPlus.h"
#include <iostream>
#include <stdexcept>

//busqueda por genero
ListaLibros* ArbolBPlus::buscarPorGenero(const std::string& genero) const {
    try {
        if (genero.empty()) throw std::invalid_argument("Género vacío en buscarPorGenero");

        ListaLibros* resultados = new ListaLibros();

        NodoHoja* hoja = buscarHoja(genero);
        if (!hoja) {
            std::cerr << "Error: Hoja nula en buscarPorGenero para género: " << genero << std::endl;
            return resultados; // Devolver lista vacía
        }

        for (int i = 0; i < hoja->numClaves; i++) {
            if (hoja->entradas[i].genero == genero) {
                if (hoja->entradas[i].indiceISBN != nullptr) {
                    recorrerAVLyAgregarLibros(hoja->entradas[i].indiceISBN->getRaiz(), resultados);
                }
                break;
            }
        }

        return resultados;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::buscarPorGenero: " << e.what() << std::endl;
        return new ListaLibros(); // Devolver lista vacía en caso de error
    }
}


void ArbolBPlus::recorrerAVLyAgregarLibros(NodoIndiceISBN* nodoAVL, ListaLibros* resultados) const {
    if (!nodoAVL) return;

    try {
        recorrerAVLyAgregarLibros(nodoAVL->izquierdo, resultados);

        if (!nodoAVL->libro) {
            throw std::runtime_error("Libro nulo encontrado en AVL durante recorrido");
        }

        resultados->insertar(nodoAVL->libro);
        recorrerAVLyAgregarLibros(nodoAVL->derecho, resultados);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::recorrerAVLyAgregarLibros: " << e.what() << std::endl;
        throw;
    }
}


//metodo auxiliar de busqueda para la eliminacion
bool ArbolBPlus::buscarGeneroAux(const std::string& genero, NodoHoja** hojaOut, int& pos) const {
    try {
        if (genero.empty()) throw std::invalid_argument("Género vacío en buscarGeneroAux");
        if (!hojaOut) throw std::invalid_argument("Puntero de salida nulo en buscarGeneroAux");

        //localizar la hoja
        NodoHoja* hoja = buscarHoja(genero);
        if (!hoja) {
            *hojaOut = nullptr;
            pos = -1;
            return false;
        }

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
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::buscarGeneroAux: " << e.what() << std::endl;
        if (hojaOut) *hojaOut = nullptr;
        pos = -1;
        return false;
    }
}