#include "ArbolB.h"
#include <iostream>

void ArbolB::eliminar(int fecha) {
    if (!raiz) return;

    eliminarRecursivo(raiz, fecha);

    //si la raiz se queda sin clave se tiene que ajustar
    if (raiz->numClaves == 0) {
        NodoB* tmp = raiz;
        if (raiz->esHoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete tmp;
    }
}

void ArbolB::eliminarRecursivo(NodoB *nodo, int fecha) {
    int idx = 0;
    while (idx < nodo->numClaves && std::stoi(nodo->claves[idx]->getFecha())) {
        idx++;
    }

    //caso1: la clave esta en el nodo
    if (idx < nodo->numClaves && fecha == std::stoi(nodo->claves[idx]->getFecha())) {
        if (nodo->esHoja) {
            eliminarDeHoja(nodo, idx);
        } else {
            eliminarDeInterno(nodo, idx);
        }
    } else {
        //caso 2: la clave n esta en este nodo
        if (nodo->esHoja) {
            return;
        }

        bool ultimaClave = (idx == nodo->numClaves);

        //asegurar que el hijo tenga al menos T claves
        if (nodo->hijos[idx]->numClaves < T) {
            llenar(nodo, idx);
        }

        if (ultimaClave && idx > nodo->numClaves) {
            eliminarRecursivo(nodo->hijos[idx - 1], fecha);
        } else {
            eliminarRecursivo(nodo->hijos[idx], fecha);
        }
    }
}

void ArbolB::eliminarDeHoja(NodoB *nodo, int idx) {
    for (int i = idx + 1; i < nodo->numClaves; i++) {
        nodo->claves[i - 1] = nodo->claves[i];
    }
    nodo->numClaves--;
}

void ArbolB::eliminarDeInterno(NodoB *nodo, int idx) {
    int fecha = std::stoi(nodo->claves[idx]->getFecha());

    if (nodo->hijos[idx]->numClaves >= T) {
        Libro* pred = obtenerPredecesor(nodo, idx);
        nodo->claves[idx] = pred;
        eliminarRecursivo(nodo->hijos[idx], std::stoi(pred->getFecha()));
    } else if (nodo->hijos[idx + 1]->numClaves >= T) {
        Libro* succ = obtenerSucesor(nodo, idx);
        nodo->claves[idx] = succ;
        eliminarRecursivo(nodo->hijos[idx + 1], std::stoi(succ->getFecha()));
    } else {
        fusionar(nodo, idx);
        eliminarRecursivo(nodo->hijos[idx], fecha);
    }
}

//obtener el predecesor de la clave
Libro *ArbolB::obtenerPredecesor(NodoB *nodo, int idx) {
    NodoB* actual = nodo->hijos[idx];
    while (!actual->esHoja) {
        actual = actual->hijos[actual->numClaves];
    }
    return  actual->claves[actual->numClaves -1];
}


//obtener el suceros de la clave
Libro *ArbolB::obtenerSucesor(NodoB *nodo, int idx) {
    NodoB* actual = nodo->hijos[idx + 1];
    while (!actual->esHoja) {
        actual = actual->hijos[0];
    }
    return actual->claves[0];
}

//asegurar que el hijo tenga el minimo de claves
void ArbolB::llenar(NodoB *nodo, int idx) {
    if (idx != 0 && nodo->hijos[idx -1]->numClaves >= T) {
        prestarDeAnterior(nodo, idx);
    } else if (idx != nodo->numClaves && nodo->hijos[idx + 1]->numClaves >= T) {
        prestarDeSiguiente(nodo, idx);
    } else {
        if (idx != nodo->numClaves) {
            fusionar(nodo, idx);
        } else {
            fusionar(nodo, idx -1);
        }
    }
}

void ArbolB::prestarDeAnterior(NodoB *nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx - 1];

    //mover claves en hijo hacia adelante
    for (int i = hijo->numClaves - 1; i >= 0; i--) {
        hijo->claves[i + 1] = hijo->hijos[i];
    }
    if (!hijo->esHoja) {
        for (int i = hijo->numClaves; i >= 0; i--) {
            hijo->hijos[i + 1] = hijo->hijos[i];
        }
    }

    //traer clave del padre
}

