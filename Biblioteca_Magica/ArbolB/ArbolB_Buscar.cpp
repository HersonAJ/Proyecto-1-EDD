#include "ArbolB.h"
#include <string>

ListaLibros ArbolB::buscarPorRango(int inicio, int fin) {
    ListaLibros resultados;
    if (raiz != nullptr) {
        buscarRangoRecursivo(raiz, inicio, fin, resultados);
    }
    return resultados;
}

void ArbolB::buscarRangoRecursivo(NodoB *nodo, int inicio, int fin, ListaLibros& resultados) {
    int i = 0;

    // encontrar el primer índice donde la clave puede estar en el rango
    while (i < nodo->numClaves) {
        int añoActual;
        try {
            añoActual = std::stoi(nodo->claves[i]->getFecha());
        } catch (const std::exception& e) {
            añoActual = 0;
        }

        if (añoActual >= inicio) {
            break; // aquí se corrige
        }
        i++;
    }

    // si no es hoja, recorrer los hijos relevantes
    if (!nodo->esHoja) {
        for (int j = 0; j <= i; j++) {
            if (nodo->hijos[j] != nullptr) {
                buscarRangoRecursivo(nodo->hijos[j], inicio, fin, resultados);
            }
        }
    }

    // procesar las claves de este nodo que están en el rango
    while (i < nodo->numClaves) {
        int añoActual;
        try {
            añoActual = std::stoi(nodo->claves[i]->getFecha());
        } catch (const std::exception& e) {
            añoActual = 0;
        }

        if (añoActual > fin) {
            break; // fuera del rango, ya no sigo
        }

        if (añoActual >= inicio) {
            resultados.insertar(nodo->claves[i]);
        }
        i++;
    }

    // si no es hoja, recorrer los hijos restantes
    if (!nodo->esHoja) {
        for (int j = i; j <= nodo->numClaves; j++) {
            if (nodo->hijos[j] != nullptr) {
                buscarRangoRecursivo(nodo->hijos[j], inicio, fin, resultados);
            }
        }
    }
}
