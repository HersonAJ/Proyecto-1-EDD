#include <iostream>
#include <ostream>
#include "ArbolB.h"
// Método para eliminar un libro por fecha
/*
void ArbolB::eliminar(int fecha) {
    if (raiz == nullptr) {
        std::cout << "El árbol está vacío" << std::endl;
        return;
    }

    eliminarRecursivo(raiz, fecha);

    // Si la raíz quedó vacía
    if (raiz->numClaves == 0) {
        NodoB* temp = raiz;
        if (raiz->esHoja) {
            raiz = nullptr;
        } else {
            raiz = raiz->hijos[0];
        }
        delete temp;
    }
}

// Eliminación recursiva
void ArbolB::eliminarRecursivo(NodoB* nodo, int fecha) {
    int idx = 0;

    // Buscar la primera clave mayor o igual que fecha
    while (idx < nodo->numClaves && fecha > std::stoi(nodo->claves[idx]->getFecha())) {
        idx++;
    }

    // Si encontramos la clave
    if (idx < nodo->numClaves && fecha == std::stoi(nodo->claves[idx]->getFecha())) {
        if (nodo->esHoja) {
            eliminarDeHoja(nodo, idx);
        } else {
            eliminarDeInterno(nodo, idx);
        }
    } else if (!nodo->esHoja) {
        // La clave no está en este nodo, buscar en el hijo apropiado
        bool bandera = (idx == nodo->numClaves);

        // Si el hijo tiene solo T-1 claves, llenarlo primero
        if (nodo->hijos[idx]->numClaves < T) {
            llenar(nodo, idx);
        }

        // Después de llenar, el índice podría cambiar
        if (bandera && idx > nodo->numClaves) {
            eliminarRecursivo(nodo->hijos[idx - 1], fecha);
        } else {
            eliminarRecursivo(nodo->hijos[idx], fecha);
        }
    }
}

// Eliminar clave de un nodo hoja
void ArbolB::eliminarDeHoja(NodoB* nodo, int idx) {
    // Mover todas las claves después de idx una posición atrás
    for (int i = idx + 1; i < nodo->numClaves; i++) {
        nodo->claves[i - 1] = nodo->claves[i];
    }
    nodo->claves[nodo->numClaves - 1] = nullptr;
    nodo->numClaves--;
}

// Eliminar clave de un nodo interno
void ArbolB::eliminarDeInterno(NodoB* nodo, int idx) {
    Libro* clave = nodo->claves[idx];

    if (nodo->hijos[idx]->numClaves >= T) {
        // Si el hijo izquierdo tiene al menos T claves
        Libro* predecesor = obtenerPredecesor(nodo, idx);
        nodo->claves[idx] = predecesor;
        eliminarRecursivo(nodo->hijos[idx], std::stoi(predecesor->getFecha()));
    } else if (nodo->hijos[idx + 1]->numClaves >= T) {
        // Si el hijo derecho tiene al menos T claves
        Libro* sucesor = obtenerSucesor(nodo, idx);
        nodo->claves[idx] = sucesor;
        eliminarRecursivo(nodo->hijos[idx + 1], std::stoi(sucesor->getFecha()));
    } else {
        // Si ambos hijos tienen T-1 claves, fusionar
        fusionar(nodo, idx);
        eliminarRecursivo(nodo->hijos[idx], std::stoi(clave->getFecha()));
    }
}

// Obtener el predecesor de la clave en idx
Libro* ArbolB::obtenerPredecesor(NodoB* nodo, int idx) {
    NodoB* actual = nodo->hijos[idx];
    while (!actual->esHoja) {
        actual = actual->hijos[actual->numClaves];
    }
    return actual->claves[actual->numClaves - 1];
}

// Obtener el sucesor de la clave en idx
Libro* ArbolB::obtenerSucesor(NodoB* nodo, int idx) {
    NodoB* actual = nodo->hijos[idx + 1];
    while (!actual->esHoja) {
        actual = actual->hijos[0];
    }
    return actual->claves[0];
}

void ArbolB::llenar(NodoB* nodo, int idx) {
    // Si el hermano anterior tiene al menos T claves, prestar de él
    if (idx != 0 && nodo->hijos[idx - 1]->numClaves >= T) {
        prestarDeAnterior(nodo, idx);
    }
    // Si el hermano siguiente tiene al menos T claves, prestar de él
    else if (idx != nodo->numClaves && nodo->hijos[idx + 1]->numClaves >= T) {
        prestarDeSiguiente(nodo, idx);
    }
    // Si ambos hermanos tienen T-1 claves, fusionar
    else {
        if (idx != nodo->numClaves) {
            fusionar(nodo, idx);
        } else {
            fusionar(nodo, idx - 1);
        }
    }
}

void ArbolB::prestarDeAnterior(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx - 1];

    // Mover una clave del padre al hijo
    for (int i = hijo->numClaves - 1; i >= 0; i--) {
        hijo->claves[i + 1] = hijo->claves[i];
    }

    if (!hijo->esHoja) {
        for (int i = hijo->numClaves; i >= 0; i--) {
            hijo->hijos[i + 1] = hijo->hijos[i];
        }
    }

    hijo->claves[0] = nodo->claves[idx - 1];

    if (!hijo->esHoja) {
        hijo->hijos[0] = hermano->hijos[hermano->numClaves];
    }

    nodo->claves[idx - 1] = hermano->claves[hermano->numClaves - 1];
    hermano->claves[hermano->numClaves - 1] = nullptr;

    hijo->numClaves++;
    hermano->numClaves--;
}

void ArbolB::prestarDeSiguiente(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx + 1];

    hijo->claves[hijo->numClaves] = nodo->claves[idx];

    if (!hijo->esHoja) {
        hijo->hijos[hijo->numClaves + 1] = hermano->hijos[0];
    }

    nodo->claves[idx] = hermano->claves[0];

    for (int i = 1; i < hermano->numClaves; i++) {
        hermano->claves[i - 1] = hermano->claves[i];
    }
    hermano->claves[hermano->numClaves - 1] = nullptr;

    if (!hermano->esHoja) {
        for (int i = 1; i <= hermano->numClaves; i++) {
            hermano->hijos[i - 1] = hermano->hijos[i];
        }
    }

    hijo->numClaves++;
    hermano->numClaves--;
}

// Fusionar el hijo en idx con su hermano
void ArbolB::fusionar(NodoB* nodo, int idx) {
    NodoB* hijo = nodo->hijos[idx];
    NodoB* hermano = nodo->hijos[idx + 1];

    // Copiar la clave del padre y el hermano al hijo
    hijo->claves[T - 1] = nodo->claves[idx];

    for (int i = 0; i < hermano->numClaves; i++) {
        hijo->claves[i + T] = hermano->claves[i];
    }

    if (!hijo->esHoja) {
        for (int i = 0; i <= hermano->numClaves; i++) {
            hijo->hijos[i + T] = hermano->hijos[i];
        }
    }

    // Mover las claves del padre
    for (int i = idx + 1; i < nodo->numClaves; i++) {
        nodo->claves[i - 1] = nodo->claves[i];
    }
    nodo->claves[nodo->numClaves - 1] = nullptr;

    // Mover los punteros a hijos
    for (int i = idx + 2; i <= nodo->numClaves; i++) {
        nodo->hijos[i - 1] = nodo->hijos[i];
    }
    nodo->hijos[nodo->numClaves] = nullptr;

    hijo->numClaves += hermano->numClaves + 1;
    nodo->numClaves--;

    delete hermano;
}*/