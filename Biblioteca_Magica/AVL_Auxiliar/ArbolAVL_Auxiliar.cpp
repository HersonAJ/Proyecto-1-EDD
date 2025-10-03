#include "IndiceISBN.h"
#include <algorithm>
#include <iostream>
#include <ostream>

// Constructor / Destructor
IndiceISBN::IndiceISBN() : raiz(nullptr) {}
IndiceISBN::~IndiceISBN() {
    destruirRecursivo(raiz);
}

void IndiceISBN::destruirRecursivo(const NodoIndiceISBN* nodo) {
    if (nodo) {
        destruirRecursivo(nodo->izquierdo);
        destruirRecursivo(nodo->derecho);
        delete nodo;
    }
}

// Altura y balance
int IndiceISBN::altura(const NodoIndiceISBN* nodo) const {
    return nodo ? nodo->altura : 0;
}

int IndiceISBN::factorBalance(const NodoIndiceISBN* nodo) const {
    return nodo ? altura(nodo->izquierdo) - altura(nodo->derecho) : 0;
}

NodoIndiceISBN* IndiceISBN::nodoMinimo(NodoIndiceISBN* nodo) const {
    NodoIndiceISBN* actual = nodo;
    while (actual && actual->izquierdo) actual = actual->izquierdo;
    return actual;
}

// Rotaciones
NodoIndiceISBN* IndiceISBN::rotarIzquierda(NodoIndiceISBN* ref) {
    NodoIndiceISBN* nodo = ref->derecho;
    NodoIndiceISBN* nodo1 = nodo->izquierdo;

    nodo->izquierdo = ref;
    ref->derecho = nodo1;

    ref->altura = 1 + std::max(altura(ref->izquierdo), altura(ref->derecho));
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return nodo;
}

NodoIndiceISBN* IndiceISBN::rotarDerecha(NodoIndiceISBN* ref) {
    NodoIndiceISBN* nodo = ref->izquierdo;
    NodoIndiceISBN* nodo1 = nodo->derecho;

    nodo->derecho = ref;
    ref->izquierdo = nodo1;

    ref->altura = 1 + std::max(altura(ref->izquierdo), altura(ref->derecho));
    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));

    return nodo;
}

// Balanceo
NodoIndiceISBN* IndiceISBN::balancear(NodoIndiceISBN* nodo) {
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

// Inserción
NodoIndiceISBN* IndiceISBN::insertarNodo(NodoIndiceISBN* nodo, const std::string& isbn, Libro* libro) {
    if (!nodo) return new NodoIndiceISBN(isbn, libro);

    if (isbn < nodo->isbn)
        nodo->izquierdo = insertarNodo(nodo->izquierdo, isbn, libro);
    else if (isbn > nodo->isbn)
        nodo->derecho = insertarNodo(nodo->derecho, isbn, libro);
    else
        return nodo; // ISBN ya existe, no duplicados

    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    return balancear(nodo);
}

void IndiceISBN::insertar(const std::string& isbn, Libro* libro) {
    raiz = insertarNodo(raiz, isbn, libro);
}

// Eliminación
NodoIndiceISBN* IndiceISBN::eliminarNodo(NodoIndiceISBN* nodo, const std::string& isbn) {
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
            NodoIndiceISBN* temp = nodo->derecho;
            delete nodo;
            return temp;
        }
        else if (!nodo->derecho) {
            NodoIndiceISBN* temp = nodo->izquierdo;
            delete nodo;
            return temp;
        }
        else {
            NodoIndiceISBN* sucesor = nodoMinimo(nodo->derecho);
            // GUARDAR ISBN ANTES de eliminar
            std::string isbnSucesor = sucesor->isbn;

            // Eliminar el sucesor
            nodo->derecho = eliminarNodo(nodo->derecho, isbnSucesor);

            // Actualizar solo el ISBN
            nodo->isbn = isbnSucesor;
        }
    }

    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    return balancear(nodo);
}

void IndiceISBN::eliminar(const std::string& isbn) {
    std::cout << "[DEBUG] IndiceISBN::eliminar('" << isbn << "') - raiz antes: " << raiz << std::endl;
    raiz = eliminarNodo(raiz, isbn);
    std::cout << "[DEBUG] IndiceISBN::eliminar('" << isbn << "') - raiz después: " << raiz << std::endl;
}

// Búsqueda
Libro* IndiceISBN::buscar(const std::string& isbn) const {
    NodoIndiceISBN* actual = raiz;
    while (actual) {
        if (isbn < actual->isbn)
            actual = actual->izquierdo;
        else if (isbn > actual->isbn)
            actual = actual->derecho;
        else
            return actual->libro;
    }
    return nullptr;
}

bool IndiceISBN::estaVacio() const {
    bool vacio = (raiz == nullptr);
    std::cout << "[DEBUG] IndiceISBN::estaVacio() = " << vacio << std::endl;
    return vacio;
}
