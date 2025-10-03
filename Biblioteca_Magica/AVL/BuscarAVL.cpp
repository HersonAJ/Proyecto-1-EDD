#include "ArbolAVL.h"
#include "ListaEncontrados.h"

//buscar el primer libro con el titulo para devolver solo 1 resultado
Libro* ArbolAVL::buscarPorTitulo(const std::string& titulo) {
    return buscarPorTituloRecursivo(raiz, titulo);
}

Libro* ArbolAVL::buscarPorTituloRecursivo(NodoAVL* nodo, const std::string& titulo) {
    if (!nodo) return nullptr;

    int cmp = titulo.compare(nodo->libro->getTitulo());

    if (cmp == 0) {
        return nodo->libro; //primera concidencia encontrada
    } else if (cmp < 0) {
        return buscarPorTituloRecursivo(nodo->izquierdo, titulo);
    } else {
        return  buscarPorTituloRecursivo(nodo->derecho, titulo);
    }
}

ListaEncontados* ArbolAVL::buscarTodosPorTitulo(const std::string& titulo) {
    ListaEncontados* lista = new ListaEncontados();
    buscarTodosPorTituloRecursivo(raiz, titulo, lista);
    return lista;
}

void ArbolAVL::buscarTodosPorTituloRecursivo(NodoAVL* nodo, const std::string& titulo, ListaEncontados* lista) {
    if (!nodo) return ;

    //recorrido inorden para encontrar todos
    buscarTodosPorTituloRecursivo(nodo->izquierdo, titulo, lista);

    if (titulo.compare(nodo->libro->getTitulo()) == 0) {
        lista->agregar(nodo->libro);
    }

    buscarTodosPorTituloRecursivo(nodo->derecho, titulo, lista);
}

NodoAVL* ArbolAVL::buscarNodo(NodoAVL* nodo, const std::string& titulo, const std::string& isbn) {
    if (!nodo) return nullptr;

    int cmp = titulo.compare(nodo->libro->getTitulo());

    if (cmp == 0) {
        // Mismo título, verificar ISBN si se proporciona
        if (isbn.empty() || isbn.compare(nodo->libro->getIsbn()) == 0) {
            return nodo;
        }
    }

    if (cmp <= 0) {
        return buscarNodo(nodo->izquierdo, titulo, isbn);
    } else {
        return buscarNodo(nodo->derecho, titulo, isbn);
    }
}

NodoAVL *ArbolAVL::buscar(const std::string &titulo, const std::string &isbn) {
    return buscarNodo(raiz, titulo, isbn);
}
