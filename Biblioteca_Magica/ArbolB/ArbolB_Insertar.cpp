#include "ArbolB.h"
#include <iostream>

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
void ArbolB::insertar(Libro *libro) {
    //convertir la la fecha string a int para el ordenamiento
    int fechaInt = libro->getFechaInt();

    //si es arbol esta vacio
    if (raiz == nullptr) {
        raiz = new NodoB(true);
        raiz->fechas[0] = fechaInt;
        raiz->libros[0] = libro;
        raiz->numClaves = 1;
    } else {
        //si la raiz esta llena, dividir
        if (raiz->numClaves == 2*T - 1) {
            NodoB* nuevaRaiz = new NodoB(false);
            nuevaRaiz->hijos[0] = raiz;
            dividirHijo(nuevaRaiz, 0);
            raiz = nuevaRaiz;

            //decidir qye hijo insertar
            int i = 0;
            if (fechaInt > nuevaRaiz->fechas[0]) {
                i++;
            }
            insertarNoLleno(nuevaRaiz->hijos[i], fechaInt, libro);
        } else {
            insertarNoLleno(raiz, fechaInt, libro);
        }
    }
}

//dividir hijo lleno
void ArbolB::dividirHijo(NodoB *padre, int indice) {
    NodoB* hijo = padre->hijos[indice];
    NodoB* nuevoHijo = new NodoB(hijo->esHoja);
    nuevoHijo->numClaves = T - 1;

    //copiar las ultimas T - 1 claves e hijos del hijo original al nuevo hijo
    // Copiar claves y libros
    for (int j = 0; j < T - 1; j++) {
        nuevoHijo->fechas[j] = hijo->fechas[j + T];
        nuevoHijo->libros[j] = hijo->libros[j + T];
    }

    // Copiar hijos si no es hoja
    if (!hijo->esHoja) {
        for (int j = 0; j < T; j++) {
            nuevoHijo->hijos[j] = hijo->hijos[j + T];
        }
    }


    hijo->numClaves = T - 1;

    //desplazar hijos del pabre para hacer espacio
    for (int j = padre->numClaves; j >= indice + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[indice + 1] = nuevoHijo;

    //desplazar clave del padre
    for (int j = padre->numClaves - 1; j >= indice; j--) {
        padre->fechas[j + 1] = padre->fechas[j];
        padre->libros[j + 1] = padre->libros[j];
    }

    //subir clave media del hijo al padre
    padre->fechas[indice] = hijo->fechas[T - 1];
    padre->libros[indice] = hijo->libros[T - 1];
    padre->numClaves++;
}

// Insertar en nodo no lleno
void ArbolB::insertarNoLleno(NodoB* nodo, int fecha, Libro* libro) {
    int i = nodo->numClaves - 1;

    if (nodo->esHoja) {
        // Encontrar posición e insertar
        while (i >= 0 && fecha < nodo->fechas[i]) {
            nodo->fechas[i + 1] = nodo->fechas[i];
            nodo->libros[i + 1] = nodo->libros[i];
            i--;
        }
        nodo->fechas[i + 1] = fecha;
        nodo->libros[i + 1] = libro;
        nodo->numClaves++;
    } else {
        // Encontrar hijo adecuado
        while (i >= 0 && fecha < nodo->fechas[i]) {
            i--;
        }
        i++;

        // Si el hijo está lleno, dividirlo
        if (nodo->hijos[i]->numClaves == 2*T - 1) {
            dividirHijo(nodo, i);
            if (fecha > nodo->fechas[i]) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], fecha, libro);
    }
}

#include <queue>//solo para el log para verificar la estructura del arbol

// Método simple para verificar la inserción
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

            //SOLO imprimir claves válidas
            std::cout << "[";
            for (int j = 0; j < actual->numClaves; j++) {
                std::cout << actual->fechas[j];
                if (j < actual->numClaves - 1) std::cout << ",";
            }
            std::cout << "] ";

            //SOLO encolar hijos existentes
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