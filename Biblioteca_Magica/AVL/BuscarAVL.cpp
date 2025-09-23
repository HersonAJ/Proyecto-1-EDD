#include "ArbolAVL.h"
#include "NodoAVL.h"


/*NodoAVL* ArbolAVL::buscar(const std::string& titulo, const std::string& isbn) {
    return buscarNodo(raiz, titulo, isbn);
}
*/
NodoAVL* ArbolAVL::buscar(const std::string& titulo, const std::string& isbn) {
    NodoAVL* aux = raiz;

    while (aux != nullptr) {
        int cmp = titulo.compare(aux->libro->getTitulo());
        if (cmp == 0) {
            if (isbn.empty() || isbn == aux->libro->getIsbn()) {
                return aux; // encontrado
            }
            aux = aux->derecho; // seguir buscando si hay duplicados
        } else if (cmp < 0) {
            aux = aux->izquierdo;
        } else {
            aux = aux->derecho;
        }
    }
    return nullptr;
}


NodoAVL* ArbolAVL::buscarNodo(NodoAVL* nodo, const std::string& titulo, const std::string& isbn) {
    if (nodo == nullptr) return nullptr;

    int cmp = titulo.compare(nodo->libro->getTitulo());
    if (cmp == 0) {
        int cmpIsbn = isbn.compare(nodo->libro->getIsbn());
        if (cmpIsbn == 0) return nodo;
        else if (cmpIsbn < 0) return buscarNodo(nodo->izquierdo, titulo, isbn);
        else return buscarNodo(nodo->derecho, titulo, isbn);
    } else if (cmp < 0) {
        return buscarNodo(nodo->izquierdo, titulo, isbn);
    } else {
        return buscarNodo(nodo->derecho, titulo, isbn);
    }
}


