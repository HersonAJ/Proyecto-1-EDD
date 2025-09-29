#include "ListaLibros.h"
#include "ArbolB.h"

ListaLibros *ArbolB::buscarPorRango(int fechaInicio, int fechaFin) {
    ListaLibros* resultados = new ListaLibros();
    buscarPorRangoRecursivo(raiz, fechaInicio, fechaFin, resultados);
    return resultados;
}

void ArbolB::buscarPorRangoRecursivo(NodoB *nodo, int fechaInicio, int fechaFin, ListaLibros *resultados) {
    if (!nodo) return;

    int i = 0;

    //recorrer todas las claves de este nodo
    while (i < nodo->numClaves) {
        int fechaActual = nodo->claves[i]->fecha;

        //si la fecha esta dentro del rango, procesar todos sus libros
        if (fechaActual >= fechaInicio && fechaActual <= fechaFin) {
            //recorrer el avl interno de isbn de esta fecha y agregarlos a la lista
            recorrerAVLyAgregarLibros(nodo->claves[i]->indiceISBN.getRaiz(), resultados);
        }

        //si no es hoja podria haber mas claves en el ranfo en el hijo i
        if (!nodo->esHoja && fechaActual >= fechaInicio) {
            buscarPorRangoRecursivo(nodo->hijos[i], fechaInicio, fechaFin, resultados);
        }
        i++;
    }

    //ultimo hijo para claves mayores a la ultima clave de este nodo
    if (!nodo->esHoja) {
        buscarPorRangoRecursivo(nodo->hijos[i], fechaInicio, fechaFin, resultados);
    }
}

void ArbolB::recorrerAVLyAgregarLibros(NodoIndiceISBN* nodoAVL, ListaLibros* resultados) {
    if (!nodoAVL) return;

    //recorrido in-orden del avl para agregar toodos los libros
    recorrerAVLyAgregarLibros(nodoAVL->izquierdo, resultados);

    resultados->insertar(nodoAVL->libro);

    recorrerAVLyAgregarLibros(nodoAVL->derecho, resultados);
}

