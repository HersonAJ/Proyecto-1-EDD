#include "ArbolAVL.h"
#include "../AVL_Auxiliar/IndiceISBN.h"

void ArbolAVL::eliminarPorISBN(const std::string& isbn, IndiceISBN& indiceGlobal) {
    // 1. Buscar en AVL auxiliar por ISBN (O(log n))
    //pasando al indice global isbn
    Libro* libroEliminar = indiceGlobal.buscar(isbn);

    if (!libroEliminar) return; // No encontrado

    // 2. Obtener el título del libro a eliminar
    std::string titulo = libroEliminar->getTitulo();

    // 3. Eliminar del AVL principal usando título + ISBN (búsqueda eficiente)
    raiz = eliminarNodoEficiente(raiz, titulo, isbn);


}

NodoAVL* ArbolAVL::eliminarNodoEficiente(NodoAVL* nodo, const std::string& titulo, const std::string& isbn) {
    if (!nodo) return nullptr;

    // Búsqueda binaria normal por título
    int cmp = titulo.compare(nodo->libro->getTitulo());

    if (cmp < 0) {
        nodo->izquierdo = eliminarNodoEficiente(nodo->izquierdo, titulo, isbn);
    }
    else if (cmp > 0) {
        nodo->derecho = eliminarNodoEficiente(nodo->derecho, titulo, isbn);
    }
    else {
        // Títulos iguales, buscar por ISBN específico
        if (isbn == nodo->libro->getIsbn()) {
            // Eliminar este nodo específico
            if (!nodo->izquierdo && !nodo->derecho) {
                delete nodo->libro;
                delete nodo;
                return nullptr;
            }
            else if (!nodo->izquierdo) {
                NodoAVL* temp = nodo->derecho;
                delete nodo->libro;
                delete nodo;
                return temp;
            }
            else if (!nodo->derecho) {
                NodoAVL* temp = nodo->izquierdo;
                delete nodo->libro;
                delete nodo;
                return temp;
            }
            else {
                NodoAVL* sucesor = nodoMinimo(nodo->derecho);
                delete nodo->libro;
                nodo->libro = new Libro(*sucesor->libro);
                nodo->derecho = eliminarNodoEficiente(nodo->derecho,
                    sucesor->libro->getTitulo(), sucesor->libro->getIsbn());
            }
        }
        else {
            // Mismo título pero diferente ISBN, seguir buscando
            // (esto maneja libros con mismo título pero diferente ISBN)
            int cmpISBN = isbn.compare(nodo->libro->getIsbn());
            if (cmpISBN < 0) {
                nodo->izquierdo = eliminarNodoEficiente(nodo->izquierdo, titulo, isbn);
            } else {
                nodo->derecho = eliminarNodoEficiente(nodo->derecho, titulo, isbn);
            }
        }
    }

    nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
    return balancear(nodo);
}
NodoAVL* ArbolAVL::nodoMinimo(NodoAVL* nodo) const {
    NodoAVL* actual = nodo;
    while (actual && actual->izquierdo != nullptr) {
        actual = actual->izquierdo;
    }
    return actual;
}
