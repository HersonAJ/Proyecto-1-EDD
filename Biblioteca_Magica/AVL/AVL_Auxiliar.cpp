#include "AVL_Auxiliar.h"
#include <algorithm>
#include <iostream>
#include <ostream>


AVL_Auxiliar::AVL_Auxiliar() : raiz(nullptr) {}
AVL_Auxiliar::~AVL_Auxiliar() {
    destruirRecursivo(raiz);
}

void AVL_Auxiliar::destruirRecursivo(const NodoAVL_Auxiliar* nodo) {
    if (nodo) {
        destruirRecursivo(nodo->izquierdo);
        destruirRecursivo(nodo->derecho);
        delete nodo;
    }
}


int AVL_Auxiliar::altura(const NodoAVL_Auxiliar* nodo) const {
    return nodo ? nodo->altura : 0;
}

int AVL_Auxiliar::factorBalance(const NodoAVL_Auxiliar* nodo) const {
    return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0;
}

NodoAVL_Auxiliar* AVL_Auxiliar::nodoMinimo(NodoAVL_Auxiliar* nodo) const {
    NodoAVL_Auxiliar* actual = nodo;
    while (actual && actual->izquierdo) actual = actual->izquierdo;
    return actual;
}

//Rotaciones
NodoAVL_Auxiliar* AVL_Auxiliar::rotarIzquierda(NodoAVL_Auxiliar* ref) {
    NodoAVL_Auxiliar* nodo = ref->derecho;
    NodoAVL_Auxiliar* nodo1 = nodo->izquierdo;

    nodo->izquierdo = ref;
    ref->derecho = nodo1;

    ref->altura = 1 + std::max(altura(ref->izquierdo), altura(ref->derecho));
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return nodo;
}

NodoAVL_Auxiliar* AVL_Auxiliar::rotarDerecha(NodoAVL_Auxiliar* ref) {
    NodoAVL_Auxiliar* nodo = ref->izquierdo;
    NodoAVL_Auxiliar* nodo1 = nodo->derecho;

    nodo->derecho = ref;
    ref->izquierdo = nodo1;

    ref->altura = 1 + std::max(altura(ref->izquierdo), altura(ref->derecho));
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return nodo;
}

//Balanceo
NodoAVL_Auxiliar* AVL_Auxiliar::balancear(NodoAVL_Auxiliar* nodo) {
    int balance = factorBalance(nodo);

    if (balance > 1 && factorBalance(nodo->izquierdo) >= 0)
        return rotarDerecha(nodo);

    if (balance > 1 && factorBalance(nodo->izquierdo) < 0) {
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    }

    if (balance < -1 && factorBalance(nodo->derecho) <= 0)
        return rotarIzquierda(nodo);

    if (balance < -1 && factorBalance(nodo->derecho) > 0) {
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

//Inserción
NodoAVL_Auxiliar* AVL_Auxiliar::insertarNodo(NodoAVL_Auxiliar* nodo, const std::string& isbn, NodoAVL* ref) {
    if (!nodo) return new NodoAVL_Auxiliar(isbn, ref);

    if (isbn < nodo->isbn)
        nodo->izquierdo = insertarNodo(nodo->izquierdo, isbn, ref);
    else if (isbn > nodo->isbn)
        nodo->derecho = insertarNodo(nodo->derecho, isbn, ref);
    else
        return nodo; // ISBN ya existe, no duplicados

    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    return balancear(nodo);
}

void AVL_Auxiliar::insertar(const std::string& isbn, NodoAVL* ref) {
    raiz = insertarNodo(raiz, isbn, ref);
}

//Eliminación
NodoAVL_Auxiliar* AVL_Auxiliar::eliminarNodo(NodoAVL_Auxiliar* nodo, const std::string& isbn) {
    if (!nodo) return nullptr;

    if (isbn < nodo->isbn)
        nodo->izquierdo = eliminarNodo(nodo->izquierdo, isbn);
    else if (isbn > nodo->isbn)
        nodo->derecho = eliminarNodo(nodo->derecho, isbn);
    else {
        if (!nodo->izquierdo && !nodo->derecho) {
            delete nodo;
            return nullptr;
        }
        else if (!nodo->izquierdo) {
            NodoAVL_Auxiliar* temp = nodo->derecho;
            delete nodo;
            return temp;
        }
        else if (!nodo->derecho) {
            NodoAVL_Auxiliar* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        }
        else {
            NodoAVL_Auxiliar* sucesor = nodoMinimo(nodo->derecho);
            nodo->isbn = sucesor->isbn;
            nodo->referencia = sucesor->referencia;
            nodo->derecho = eliminarNodo(nodo->derecho, sucesor->isbn);
        }
    }

    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    return balancear(nodo);
}

void AVL_Auxiliar::eliminar(const std::string& isbn) {
    raiz = eliminarNodo(raiz, isbn);
}

//Busqueda
NodoAVL* AVL_Auxiliar::buscar(const std::string& isbn) {
    std::cout << "Buscando libro: " << isbn << std::endl;
    NodoAVL_Auxiliar* actual = raiz;
    while (actual) {
        if (isbn < actual->isbn)
            actual = actual->izquierdo;
        else if (isbn > actual->isbn)
            actual = actual->derecho;
        else
            return actual->referencia;
    }
    return nullptr;
}
