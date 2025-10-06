#include "ListaLibros.h"
#include "ArbolB.h"
#include <iostream>
#include <stdexcept>

ListaLibros *ArbolB::buscarPorRango(int fechaInicio, int fechaFin) {
    try {
        if (fechaInicio > fechaFin) {
            throw std::invalid_argument("Fecha inicio no puede ser mayor que fecha fin");
        }

        ListaLibros* resultados = new ListaLibros();
        buscarPorRangoRecursivo(raiz, fechaInicio, fechaFin, resultados);
        return resultados;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::buscarPorRango: " << e.what() << std::endl;
        return new ListaLibros(); // Devolver lista vacía en caso de error
    }
}

void ArbolB::buscarPorRangoRecursivo(NodoB *nodo, int fechaInicio, int fechaFin, ListaLibros *resultados) {
    if (!nodo) return;

    try {
        int i = 0;

        //recorrer todas las claves de este nodo
        while (i < nodo->numClaves) {
            // Validar que la clave no sea nula
            if (!nodo->claves[i]) {
                throw std::runtime_error("Clave nula encontrada durante búsqueda por rango");
            }

            int fechaActual = nodo->claves[i]->fecha;

            //si la fecha esta dentro del rango, procesar todos sus libros
            if (fechaActual >= fechaInicio && fechaActual <= fechaFin) {
                //recorrer el avl interno de isbn de esta fecha y agregarlos a la lista
                recorrerAVLyAgregarLibros(nodo->claves[i]->indiceISBN.getRaiz(), resultados);
            }

            //si no es hoja podria haber mas claves en el rango en el hijo i
            if (!nodo->esHoja && fechaActual >= fechaInicio) {
                buscarPorRangoRecursivo(nodo->hijos[i], fechaInicio, fechaFin, resultados);
            }
            i++;
        }

        //ultimo hijo para claves mayores a la ultima clave de este nodo
        if (!nodo->esHoja) {
            buscarPorRangoRecursivo(nodo->hijos[i], fechaInicio, fechaFin, resultados);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::buscarPorRangoRecursivo: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::recorrerAVLyAgregarLibros(NodoIndiceISBN* nodoAVL, ListaLibros* resultados) {
    if (!nodoAVL) return;

    try {
        //recorrido in-orden del avl para agregar todos los libros
        recorrerAVLyAgregarLibros(nodoAVL->izquierdo, resultados);

        // Validar que el libro no sea nulo antes de insertar
        if (!nodoAVL->libro) {
            throw std::runtime_error("Libro nulo encontrado en AVL interno durante búsqueda por rango");
        }

        resultados->insertar(nodoAVL->libro);

        recorrerAVLyAgregarLibros(nodoAVL->derecho, resultados);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::recorrerAVLyAgregarLibros: " << e.what() << std::endl;
        throw;
    }
}