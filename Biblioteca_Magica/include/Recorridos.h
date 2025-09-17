#ifndef BIBLIOTECA_MAGICA_RECORRIDOS_H
#define BIBLIOTECA_MAGICA_RECORRIDOS_H

#include <iostream>

#include "../ArbolB/NodoB.h"

// Clase de recorridos genéricos para árboles binarios
template <typename Nodo>
class Recorridos {
public:
    // Inorden: Izquierda - Raíz - Derecha
    static void inOrden(Nodo* nodo) {
        if (!nodo) return;
        inOrden(nodo->izquierdo);
        std::cout << nodo->dato.toString() << "\n";
        inOrden(nodo->derecho);
    }

    // Preorden: Raíz - Izquierda - Derecha
    static void preOrden(Nodo* nodo) {
        if (!nodo) return;
        std::cout << nodo->dato.toString() << "\n";
        preOrden(nodo->izquierdo);
        preOrden(nodo->derecho);
    }

    // Postorden: Izquierda - Derecha - Raíz
    static void postOrden(Nodo* nodo) {
        if (!nodo) return;
        postOrden(nodo->izquierdo);
        postOrden(nodo->derecho);
        std::cout << nodo->dato.toString() << "\n";
    }
};

//recorrido para el arbolB prueba
class RecorridosB {
public:
    static void inOrden(NodoB* nodo) {
        if (!nodo) return;

        int i;
        for (i = 0; i < nodo->numClaves; i++) {
            if (!nodo->esHoja) {
                inOrden(nodo->hijos[i]);
            }

            //imprimir la clase
            if (nodo->claves[i] != nullptr) {
                std::cout << nodo->claves[i]->toString() << "\n";
            }
        }

        //recorrer el ultimo hijo
        if (!nodo->esHoja) {
            inOrden(nodo->hijos[i]);
        }
    }
};

#endif //BIBLIOTECA_MAGICA_RECORRIDOS_H