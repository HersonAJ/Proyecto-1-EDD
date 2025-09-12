#include "ArbolAVL.h"

ArbolAVL::ArbolAVL()
    : raiz(nullptr) {
    // Constructor: inicializa la raíz en nullptr
}

ArbolAVL::~ArbolAVL() {
    // Destructor: liberar memoria del árbol
    destruir(raiz);
}

// Método privado para liberar memoria recursivamente
void ArbolAVL::destruir(NodoAVL* nodo) {
    if (!nodo) return;
    destruir(nodo->izquierdo);
    destruir(nodo->derecho);
    delete nodo;
}
