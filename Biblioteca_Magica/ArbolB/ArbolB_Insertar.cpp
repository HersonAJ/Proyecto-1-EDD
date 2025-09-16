#include  "ArbolB.h"
#include  <iostream>

void ArbolB::insertar(Libro *libro) {
    if (raiz == nullptr) {
        //si el arbol esta vacio, se crea la raiz
        raiz = new NodoB(true);
        raiz->claves[0] = libro;
        raiz->numClaves = 1;
    } else {
        //si la raiz esta llena se tiene que dividir
        if (raiz->numClaves == 2*T -1) {
            NodoB* nuevaRaiz = new NodoB(false);
            nuevaRaiz->hijos[0] = raiz;

            //dividir la raiz
            dividirHijo(nuevaRaiz, 0, raiz);

            //decidir en que hijo insertar
            int i = 0;
            if (libro->compararPorFecha(*nuevaRaiz->claves[0]) > 0) {
                i++;
            }
            insertarNoLleno(nuevaRaiz->hijos[i], libro);

            raiz = nuevaRaiz;
        } else {
            insertarNoLleno(raiz, libro);
        }
    }
}

//metodo para insertar en un nodo que no esta lleno
void ArbolB::insertarNoLleno(NodoB *nodo, Libro *libro) {
    int i = nodo->numClaves -1;

    if (nodo->estHoja) {
        //mover clase hacia la derecha hasta encontrar la posicion correcta
        while ( i >= 0 && libro->compararPorFecha(*nodo->claves[i]) < 0) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = libro;
        nodo->numClaves++;
    } else {
        //buscar el hijo adecuado
        while (i >= 0 && libro->compararPorFecha(*nodo->claves[i]) < 0) {
            i--;
        }
        i++;

            //si el hijo esta lleno se divide
            if (nodo->hijos[i]->numClaves == 2*T -1) {
                dividirHijo(nodo, i , nodo->hijos[i]);

                if (libro->compararPorFecha(*nodo->claves[i]) > 0) {
                    i++;
                }
            }
        insertarNoLleno(nodo->hijos[i], libro);
    }
}

//dividir un hijo lleno en dos nodos
void ArbolB::dividirHijo(NodoB *padre, int i, NodoB *hijo) {
    NodoB* nuevo = new NodoB(hijo->estHoja);
    nuevo->numClaves = T - 1;

    //copiar las ultimas T -1 claves al nuevo nodo
    for (int j = 0; j < T -1; j++) {
        nuevo->claves[j] = hijo->claves[j + T];
    }

    //copiar los hijos si no es hoja
    if (!hijo->estHoja) {
        for (int j = 0; j < T; j++) {
            nuevo->hijos[j] = hijo->hijos[j + T];
        }
    }

    hijo->numClaves = T -1;

    //mover los hijos del padrea para hacer espacio
    for (int j = padre->numClaves; j >=  i + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[i + 1] = nuevo;

    //mover las claves del padre
    for (int j = padre->numClaves - 1; j >= i; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }
    padre->claves[i] = hijo->claves[T - 1];
    padre->numClaves++;
}

