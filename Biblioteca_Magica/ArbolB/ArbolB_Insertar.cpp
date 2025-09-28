#include "ArbolB.h"
#include <iostream>
#include <functional>

//destructor
ArbolB::~ArbolB() {
    destruirRecursivo(raiz);
}

void ArbolB::destruirRecursivo(NodoB* nodo) {
    if (nodo) {
        //si no es hoja, destruir hijos primero
        if (!nodo->esHoja) {
            for (int i = 0; i <= nodo->numClaves; i++) {
                destruirRecursivo(nodo->hijos[i]);
            }
        }

        delete nodo;
    }
}

//metodo publico de insercion
void ArbolB::insertar(Libro* libro) {
    int fechaInt = libro->getFechaInt();

    if (raiz == nullptr) {
        raiz = new NodoB(true);
        raiz->claves[0] = new EntradaFecha(fechaInt);
        raiz->claves[0]->indiceISBN.insertar(libro->getIsbn(), libro);
        raiz->numClaves = 1;
    } else {
        if (raiz->numClaves == 2*T - 1) {
            NodoB* nuevaRaiz = new NodoB(false);
            nuevaRaiz->hijos[0] = raiz;
            dividirHijo(nuevaRaiz, 0);
            raiz = nuevaRaiz;

            int i = 0;
            if (fechaInt > nuevaRaiz->claves[0]->fecha) {
                i++;
            }
            insertarNoLleno(nuevaRaiz->hijos[i], fechaInt, libro);
        } else {
            insertarNoLleno(raiz, fechaInt, libro);
        }
    }
}

//dividir hijo lleno
void ArbolB::dividirHijo(NodoB* padre, int indice) {
    NodoB* hijo = padre->hijos[indice];
    NodoB* nuevoHijo = new NodoB(hijo->esHoja);
    nuevoHijo->numClaves = T - 1;

    for (int j = 0; j < T - 1; j++) {
        nuevoHijo->claves[j] = hijo->claves[j + T];
    }

    if (!hijo->esHoja) {
        for (int j = 0; j < T; j++) {
            nuevoHijo->hijos[j] = hijo->hijos[j + T];
        }
    }

    hijo->numClaves = T - 1;

    for (int j = padre->numClaves; j >= indice + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[indice + 1] = nuevoHijo;

    for (int j = padre->numClaves - 1; j >= indice; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }

    padre->claves[indice] = hijo->claves[T - 1];
    padre->numClaves++;
}

// Insertar en nodo no lleno
void ArbolB::insertarNoLleno(NodoB* nodo, int fecha, Libro* libro) {
    int i = nodo->numClaves - 1;

    if (nodo->esHoja) {
        // Buscar si la fecha ya existe
        int j = 0;
        while (j < nodo->numClaves && fecha > nodo->claves[j]->fecha) {
            j++;
        }

        if (j < nodo->numClaves && nodo->claves[j]->fecha == fecha) {
            //Fecha ya existe → insertar en su índice ISBN
            nodo->claves[j]->indiceISBN.insertar(libro->getIsbn(), libro);
        } else {
            // Nueva fecha → desplazar y crear nueva entrada
            for (int k = nodo->numClaves; k > j; k--) {
                nodo->claves[k] = nodo->claves[k - 1];
            }
            nodo->claves[j] = new EntradaFecha(fecha);
            nodo->claves[j]->indiceISBN.insertar(libro->getIsbn(), libro);
            nodo->numClaves++;
        }
    } else {
        // Buscar hijo adecuado
        while (i >= 0 && fecha < nodo->claves[i]->fecha) {
            i--;
        }
        i++;

        if (nodo->hijos[i]->numClaves == 2*T - 1) {
            dividirHijo(nodo, i);
            if (fecha > nodo->claves[i]->fecha) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], fecha, libro);
    }
}

#include <queue>

// Método para imprimir el Árbol B y los ISBN de cada fecha
void ArbolB::imprimirParaPrueba() {
    if (!raiz) {
        std::cout << "Árbol B vacío\n";
        return;
    }

    std::queue<NodoB*> cola;
    cola.push(raiz);
    int nivel = 0;

    while (!cola.empty()) {
        int nodosEnNivel = cola.size();
        std::cout << "Nivel " << nivel << ": ";

        for (int i = 0; i < nodosEnNivel; i++) {
            NodoB* actual = cola.front();
            cola.pop();

            // Imprimir claves válidas
            std::cout << "[";
            for (int j = 0; j < actual->numClaves; j++) {
                EntradaFecha* entrada = actual->claves[j];
                if (!entrada) continue;

                std::cout << entrada->fecha;

                // Mostrar también los ISBN de esta fecha
                std::cout << " {ISBNs: ";
                //índice ISBN en inOrden
                std::function<void(NodoIndiceISBN*)> inOrden = [&](NodoIndiceISBN* nodo) {
                    if (!nodo) return;
                    inOrden(nodo->izquierdo);
                    std::cout << nodo->isbn << " ";
                    inOrden(nodo->derecho);
                };
                inOrden(entrada->indiceISBN.getRaiz());
                std::cout << "}";

                if (j < actual->numClaves - 1) std::cout << ", ";
            }
            std::cout << "] ";

            // Encolar hijos existentes
            if (!actual->esHoja) {
                for (int j = 0; j <= actual->numClaves; j++) {
                    if (actual->hijos[j]) {
                        cola.push(actual->hijos[j]);
                    }
                }
            }
        }
        std::cout << "\n";
        nivel++;
    }
}