#include "ArbolAVL.h"
#include "../AVL_Auxiliar/IndiceISBN.h"
#include <iostream>
#include <stdexcept>

void ArbolAVL::eliminarPorISBN(const std::string& isbn, const std::string& titulo) {
    try {
        if (isbn.empty()) throw std::invalid_argument("ISBN vacío en eliminación");
        if (titulo.empty()) throw std::invalid_argument("Título vacío en eliminación");

        raiz = eliminarNodoEficiente(raiz, titulo, isbn);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::eliminarPorISBN: " << e.what() << std::endl;
        throw; // Re-lanzar para que el caller sepa que falló la eliminación
    }
}

NodoAVL* ArbolAVL::eliminarNodoEficiente(NodoAVL* nodo, const std::string& titulo, const std::string& isbn) {
    if (!nodo) return nullptr;

    // Validar que el nodo tenga libro válido
    if (!nodo->libro) {
        throw std::runtime_error("Nodo AVL con libro nulo encontrado durante eliminación");
    }
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
                // Validar que el sucesor sea válido
                if (!sucesor || !sucesor->libro) {
                    throw std::runtime_error("Sucesor inválido encontrado durante eliminación");
                }

                delete nodo->libro;
                nodo->libro = new Libro(*sucesor->libro);
                nodo->derecho = eliminarNodoEficiente(nodo->derecho,
                    sucesor->libro->getTitulo(), sucesor->libro->getIsbn());
            }
        }
        else {
            // Mismo título pero diferente ISBN, seguir buscando
            int cmpISBN = isbn.compare(nodo->libro->getIsbn());
            if (cmpISBN < 0) {
                nodo->izquierdo = eliminarNodoEficiente(nodo->izquierdo, titulo, isbn);
            } else {
                nodo->derecho = eliminarNodoEficiente(nodo->derecho, titulo, isbn);
            }
        }
    }

    // Actualizar altura solo si el nodo no fue eliminado
    if (nodo) {
        nodo->altura = 1 + std::max(altura(nodo->izquierdo), altura(nodo->derecho));
        return balancear(nodo);
    }

    return nullptr;
}

NodoAVL* ArbolAVL::nodoMinimo(NodoAVL* nodo) const {
    try {
        if (!nodo) return nullptr;

        NodoAVL* actual = nodo;
        while (actual && actual->izquierdo != nullptr) {
            // Validar integridad de la estructura
            if (!actual->izquierdo) {
                throw std::runtime_error("Estructura del árbol corrupta en nodoMinimo");
            }
            actual = actual->izquierdo;
        }
        return actual;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::nodoMinimo: " << e.what() << std::endl;
        return nullptr;
    }
}