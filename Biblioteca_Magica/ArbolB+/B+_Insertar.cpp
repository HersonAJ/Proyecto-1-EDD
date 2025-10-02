#include "ArbolBPlus.h"
#include <iostream>

// Constructor y destructor
ArbolBPlus::ArbolBPlus() {
    raiz = new NodoHoja();   // al inicio la raíz es una hoja
    primeraHoja = (NodoHoja*)raiz;
}

ArbolBPlus::~ArbolBPlus() {
    liberarRecursivo(raiz);
}

void ArbolBPlus::liberarRecursivo(NodoBPlus* nodo) {
    if (!nodo) return;

    if (!nodo->esHoja) {
        NodoInterno* interno = (NodoInterno*)nodo;
        for (int i = 0; i <= interno->numClaves; i++) {
            liberarRecursivo(interno->hijos[i]);
        }
        delete[] interno->claves;
        delete[] interno->hijos;
        delete interno;
    } else {
        NodoHoja* hoja = (NodoHoja*)nodo;
        delete[] hoja->entradas;
        delete hoja;
    }
}

void ArbolBPlus::insertarSoloGenero(const std::string &genero) {
    NodoHoja* hoja = buscarHoja(genero);

    //buscar si el genero ya existe
    int pos = 0;
    while (pos < hoja->numClaves && genero > hoja->entradas[pos].genero) {
        pos++;
    }

    bool generoExiste = (pos < hoja->numClaves && hoja->entradas[pos].genero == genero);

    if (!generoExiste && hoja->numClaves == 2 * T_BPLUS - 1) {
        dividirHoja(hoja);
        hoja = buscarHoja(genero);
    }

    //insertar solo el genero en el B+, no los libros
    if (!generoExiste) {
        int i = 0;
        while (i < hoja->numClaves && genero > hoja->entradas[i].genero) {
            i++;
        }

        //desplazar entradas
        for (int j = hoja->numClaves; j > i; j--) {
            hoja->entradas[j] = hoja->entradas[j - 1];
        }

        //insertar entrada solo de genero
        hoja->entradas[i].genero = genero;
        //indiceISBN (avl) vacio
        hoja->numClaves++;
    }
}

// Buscar hoja adecuada
NodoHoja* ArbolBPlus::buscarHoja(const std::string& genero) const {
    NodoBPlus* nodo = raiz;

    while (!nodo->esHoja) {
        NodoInterno* interno = (NodoInterno*)nodo;
        int i = 0;
        while (i < interno->numClaves && genero >= interno->claves[i]) {
            i++;
        }
        nodo = interno->hijos[i];
    }

    return (NodoHoja*)nodo;
}

// Insertar en hoja
void ArbolBPlus::insertarEnHoja(NodoHoja* hoja, Libro* libro) {
    std::string genero = libro->getGenero();

    int i = 0;
    while (i < hoja->numClaves && genero > hoja->entradas[i].genero) {
        i++;
    }

    if (i < hoja->numClaves && hoja->entradas[i].genero == genero) {
        // Género ya existe -> solo insertar en AVL
        hoja->entradas[i].indiceISBN.insertar(libro->getIsbn(), libro);
    } else {
        // NUEVO género -> hacer espacio
        // Desplazar entradas a la derecha
        for (int j = hoja->numClaves; j > i; j--) {
            hoja->entradas[j] = hoja->entradas[j - 1];
        }

        // Insertar nueva entrada
        hoja->entradas[i].genero = genero;
        hoja->entradas[i].indiceISBN.insertar(libro->getIsbn(), libro);
        hoja->numClaves++;
    }
}

// Dividir hoja
void ArbolBPlus::dividirHoja(NodoHoja* hoja) {
    int mitad = hoja->numClaves / 2;//clave que sube en este caso 5/2 = 2 entonces es la mitad exacta

    NodoHoja* nuevaHoja = new NodoHoja();
    nuevaHoja->numClaves = hoja->numClaves - mitad;

    // Copiar la mitad superior a la nueva hoja
    for (int i = 0; i < nuevaHoja->numClaves; i++) {
        nuevaHoja->entradas[i].genero = hoja->entradas[mitad + i]. genero;
    }

    hoja->numClaves = mitad;

    // Ajustar enlaces entre hojas
    nuevaHoja->siguiente = hoja->siguiente;
    if (nuevaHoja->siguiente) {
        nuevaHoja->siguiente->anterior = nuevaHoja;
    }
    hoja->siguiente = nuevaHoja;
    nuevaHoja->anterior = hoja;

    // Subir clave al padre
    std::string claveSubir = nuevaHoja->entradas[0].genero;

    if (hoja == raiz) {
        // Crear nueva raíz
        NodoInterno* nuevaRaiz = new NodoInterno();
        nuevaRaiz->claves[0] = claveSubir;
        nuevaRaiz->hijos[0] = hoja;
        nuevaRaiz->hijos[1] = nuevaHoja;
        nuevaRaiz->numClaves = 1;
        raiz = nuevaRaiz;
    } else {
        // Insertar en el padre existente
        NodoInterno* padre = buscarPadre(raiz, hoja);

        if (!padre) {
            std::cout << "Error: No se encontro padre de la hoja" << std::endl;
            return;
        }
        int i = 0;
        while (i < padre->numClaves && claveSubir > padre->claves[i]) {
            i++;
        }
        for (int j = padre->numClaves; j > i; j--) {
            padre->claves[j] = padre->claves[j - 1];
            padre->hijos[j + 1] = padre->hijos[j];
        }
        padre->claves[i] = claveSubir;
        padre->hijos[i + 1] = nuevaHoja;
        padre->numClaves++;

        if (padre->numClaves == 2 * T_BPLUS) {
            dividirInterno(padre);
        }
    }
}

// Dividir interno
void ArbolBPlus::dividirInterno(NodoInterno* interno) {
    int mitad = interno->numClaves / 2;

    NodoInterno* nuevoInterno = new NodoInterno();
    nuevoInterno->numClaves = interno->numClaves - mitad - 1;

    // Copiar claves e hijos a nuevo interno
    for (int i = 0; i < nuevoInterno->numClaves; i++) {
        nuevoInterno->claves[i] = interno->claves[mitad + 1 + i];
    }
    for (int i = 0; i <= nuevoInterno->numClaves; i++) {
        nuevoInterno->hijos[i] = interno->hijos[mitad + 1 + i];
    }

    std::string claveSubir = interno->claves[mitad];
    interno->numClaves = mitad;

    if (interno == raiz) {
        NodoInterno* nuevaRaiz = new NodoInterno();
        nuevaRaiz->claves[0] = claveSubir;
        nuevaRaiz->hijos[0] = interno;
        nuevaRaiz->hijos[1] = nuevoInterno;
        nuevaRaiz->numClaves = 1;
        raiz = nuevaRaiz;
    } else {
        NodoInterno* padre = buscarPadre(raiz, interno);

        if (!padre) {
            std::cout << "Error: No se encontro padre del nodo interno" << std::endl;
            return;
        }

        int i = 0;
        while (i < padre->numClaves && claveSubir > padre->claves[i]) {
            i++;
        }
        for (int j = padre->numClaves; j > i; j--) {
            padre->claves[j] = padre->claves[j - 1];
            padre->hijos[j + 1] = padre->hijos[j];
        }
        padre->claves[i] = claveSubir;
        padre->hijos[i + 1] = nuevoInterno;
        padre->numClaves++;

        if (padre->numClaves == 2 * T_BPLUS) {
            dividirInterno(padre);
        }
    }
}

// Buscar padre (auxiliar)
NodoInterno* ArbolBPlus::buscarPadre(NodoBPlus* actual, NodoBPlus* hijo) const {
    if (!actual || actual->esHoja) return nullptr;

    NodoInterno* interno = (NodoInterno*)actual;
    for (int i = 0; i <= interno->numClaves; i++) {
        if (interno->hijos[i] == hijo) {
            return interno;
        }
        NodoInterno* posible = buscarPadre(interno->hijos[i], hijo);
        if (posible) return posible;
    }
    return nullptr;
}

void ArbolBPlus::insertarLibroEnGenero(Libro *libro) {
    std::string genero = libro->getGenero();
    NodoHoja* hoja = buscarHoja(genero);

    //buscar la entrada exacta al genero
    for (int i = 0; i < hoja->numClaves; i++) {
        if (hoja->entradas[i].genero == genero) {
            //insetar en el indiceISBN de esta entrada espeficica
            hoja->entradas[i].indiceISBN.insertar(libro->getIsbn(), libro);
            break;
        }
    }
}
