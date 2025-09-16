#include "ArbolAVL.h"
#include "../include/ExportadorDOT.h"

ArbolAVL::ArbolAVL()
    : raiz(nullptr) {
    // Constructor: inicializa la raíz en nullptr
}

ArbolAVL::~ArbolAVL() {
    // Destructor: liberar memoria del árbol
    destruir(raiz);
}

//Método privado para liberar memoria recursivamente
void ArbolAVL::destruir(NodoAVL* nodo) {
    if (!nodo) return;
    destruir(nodo->izquierdo);
    destruir(nodo->derecho);
    delete nodo;
}
void ArbolAVL::guardarComoDOT(const std::string& ruta) const {
    ExportadorDOT<NodoAVL>::exportar(raiz, ruta);
}

//metodos para el balanceo

//altura
int ArbolAVL::altura(NodoAVL *nodo) const {
    return nodo ? nodo->altura :0;
}

//factor de balanceo = altura derecha - altura izquierda
int ArbolAVL::factorBalance(NodoAVL *nodo) const {
    return nodo ? altura(nodo->derecho) - altura(nodo->izquierdo) : 0;
}

//rotacion simple a la izquierda
NodoAVL *ArbolAVL::rotarIzquierda(NodoAVL *nodo) {
    if (!nodo || !nodo->derecho) {
        std::cerr << "Error: rotarIzquierda requiere hijo derecho no nulo " << std::endl;
        return nodo;
    }
    NodoAVL* nuevaRaiz = nodo->derecho;
    NodoAVL* aux = nuevaRaiz->izquierdo;

    nuevaRaiz->izquierdo = nodo;
    nodo->derecho = aux;

    //actulizar la nueva altura
    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    nuevaRaiz->altura = std::max(altura(nuevaRaiz->izquierdo), altura(nuevaRaiz->derecho)) + 1;

    return nuevaRaiz;
}

//rotacion simple a la derecha
NodoAVL *ArbolAVL::rotarDerecha(NodoAVL *nodo) {

    if (!nodo || !nodo->izquierdo) {
        std::cerr << "Error: rotarDerecha requiere hijo izquierdo no nulo " << std::endl;
        return nodo;
    }

    NodoAVL* nuevaRaiz = nodo->izquierdo;
    NodoAVL* aux = nuevaRaiz->derecho;

    nuevaRaiz->derecho = nodo;
    nodo->izquierdo = aux;

    //actualizar la nueva altura
    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    nuevaRaiz->altura = std::max(altura(nuevaRaiz->izquierdo), altura(nuevaRaiz->derecho)) + 1;

    return nuevaRaiz;
}

//rotacion doble a la izquierda (derecha + izquierda)
NodoAVL *ArbolAVL::rotacionDobleIzquierda(NodoAVL *nodo) {
    nodo->derecho = rotarDerecha(nodo->derecho);
    return  rotarIzquierda(nodo);
}

//rotacion doble a la derecha (izquierda + derehca)
NodoAVL *ArbolAVL::rotacionDobleDerecha(NodoAVL *nodo) {
    nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
    return  rotarDerecha(nodo);
}

//balanceo

NodoAVL *ArbolAVL::balancear(NodoAVL *nodo) {
    if (!nodo) return nullptr;

    //actualizar la altura antes del balanceo
    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;

    int fb = factorBalance(nodo);

    //deslabanceo hacia la derecha
    if (fb > 1) {
        if (factorBalance(nodo->derecho) < 0) {
            return rotacionDobleIzquierda(nodo);
        } else {
            return rotarIzquierda(nodo);
        }
    }

    //desbalanceo hacia la izquierda
    else if (fb < -1) {
        if (factorBalance(nodo->izquierdo) > 0) {
            return rotacionDobleDerecha(nodo);
        } else {
            return rotarDerecha(nodo);
        }
    }

    return nodo;
}