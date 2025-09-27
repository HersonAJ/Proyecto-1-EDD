#ifndef BIBLIOTECA_MAGICA_RECORRIDOS_H
#define BIBLIOTECA_MAGICA_RECORRIDOS_H

#include <iostream>

#include "../ArbolB/NodoB.h"

// Clase de recorridos genéricos para árboles binarios
template <typename Nodo>
class RecorridosAVL {
public:
    // Inorden: Izquierda - Raíz - Derecha
    static void inOrden(Nodo* nodo) {
        if (!nodo) return;
        inOrden(nodo->izquierdo);
        std::cout << nodo->libro->toString() << "\n";
        inOrden(nodo->derecho);
    }

    // Preorden: Raíz - Izquierda - Derecha
    static void preOrden(Nodo* nodo) {
        if (!nodo) return;
        std::cout << nodo->libro->toString() << "\n";
        preOrden(nodo->izquierdo);
        preOrden(nodo->derecho);
    }

    // Postorden: Izquierda - Derecha - Raíz
    static void postOrden(Nodo* nodo) {
        if (!nodo) return;
        postOrden(nodo->izquierdo);
        postOrden(nodo->derecho);
        std::cout << nodo->libro->toString() << "\n";
    }
};


//recorrido para el arbolB prueba
/*
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
#include <iostream>
#include <queue>

class RecorridosB2 {
public:
    static void imprimirEstructura(NodoB* raiz) {
        if (!raiz) return;

        // Cola de pares (nodo, nivel)
        std::queue<std::pair<NodoB*, int>> q;
        q.push({raiz, 0});

        int nivelActual = 0;
        std::cout << "Nivel " << nivelActual << ": ";

        while (!q.empty()) {
            auto [nodo, nivel] = q.front();
            q.pop();

            // Cuando cambiamos de nivel, imprimimos salto y cabecera
            if (nivel > nivelActual) {
                nivelActual = nivel;
                std::cout << "\nNivel " << nivelActual << ": ";
            }

            // Imprimir todas las claves de este nodo
            for (int i = 0; i < nodo->numClaves; ++i) {
                std::cout << nodo->claves[i]->getFechaInt() << " ";
            }

            // Encolar hijos para el siguiente nivel
            if (!nodo->esHoja) {
                for (int i = 0; i <= nodo->numClaves; ++i) {
                    if (nodo->hijos[i])
                        q.push({nodo->hijos[i], nivel + 1});
                }
            }
        }

        std::cout << std::endl;
    }
};

class ArbolB3 {
public:
    static void imprimir(NodoB* raiz) {
        if (!raiz) {
            std::cout << "El arbol esta vacio\n";
            return;
        }

        std::queue<std::pair<NodoB*, int>> cola;
        cola.push({raiz, 0});
        int nivelActual = 0;

        std::cout << "Nivel 0: ";

        while (!cola.empty()) {
            auto [nodo, nivel] = cola.front();
            cola.pop();

            // Cambiamos de nivel
            if (nivel != nivelActual) {
                nivelActual = nivel;
                std::cout << "\nNivel " << nivelActual << ": ";
            }

            // Mostrar claves de este nodo en un bloque
            std::cout << "[";
            for (int i = 0; i < nodo->numClaves; i++) {
                std::cout << nodo->claves[i]->getFecha();
                if (i < nodo->numClaves - 1) std::cout << ", ";
            }
            std::cout << "] ";

            // Agregar hijos si no es hoja
            if (!nodo->esHoja) {
                for (int i = 0; i <= nodo->numClaves; i++) {
                    if (nodo->hijos[i] != nullptr) {
                        cola.push({nodo->hijos[i], nivel + 1});
                    }
                }
            }
        }

        std::cout << std::endl;
    }
};*/

#endif //BIBLIOTECA_MAGICA_RECORRIDOS_H