#include "ArbolAVL.h"
#include "../include/ExportadorDOT.h"
#include <iostream>
#include <stdexcept>

ArbolAVL::ArbolAVL()
    : raiz(nullptr) {
    // Constructor: inicializa la raíz en nullptr
}

ArbolAVL::~ArbolAVL() {
    //Destructor: liberar memoria del árbol
    try {
        destruir(raiz);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::~ArbolAVL: " << e.what() << std::endl;
    }
}

//Método privado para liberar memoria recursivamente
void ArbolAVL::destruir(NodoAVL* nodo) {
    if (!nodo) return;
    try {
        destruir(nodo->izquierdo);
        destruir(nodo->derecho);

        if (nodo->libro) {
            delete nodo->libro;
        }
        delete nodo;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::destruir: " << e.what() << std::endl;
        throw;
    }
}

void ArbolAVL::guardarComoDOT(const std::string& ruta) const {
    try {
        if (ruta.empty()) throw std::invalid_argument("Ruta vacía para exportar DOT");
        ExportadorDOT<NodoAVL> exportador(raiz, ruta);
        exportador.exportar();
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::guardarComoDOT: " << e.what() << std::endl;
        throw;
    }
}

//metodos para el balanceo

//altura
int ArbolAVL::altura(NodoAVL *nodo) const {
    return nodo ? nodo->altura : 0;
}

//factor de balanceo = altura derecha - altura izquierda
int ArbolAVL::factorBalance(NodoAVL *nodo) const {
    return nodo ? altura(nodo->derecho) - altura(nodo->izquierdo) : 0;
}

//rotacion simple a la izquierda
NodoAVL *ArbolAVL::rotarIzquierda(NodoAVL *nodo) {
    if (!nodo || !nodo->derecho) {
        throw std::invalid_argument("rotarIzquierda requiere hijo derecho no nulo");
    }

    NodoAVL* nuevaRaiz = nodo->derecho;
    NodoAVL* aux = nuevaRaiz->izquierdo;

    nuevaRaiz->izquierdo = nodo;
    nodo->derecho = aux;

    //actualizar la nueva altura
    nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;
    nuevaRaiz->altura = std::max(altura(nuevaRaiz->izquierdo), altura(nuevaRaiz->derecho)) + 1;

    return nuevaRaiz;
}

//rotacion simple a la derecha
NodoAVL *ArbolAVL::rotarDerecha(NodoAVL *nodo) {
    if (!nodo || !nodo->izquierdo) {
        throw std::invalid_argument("rotarDerecha requiere hijo izquierdo no nulo");
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
    try {
        if (!nodo || !nodo->derecho) {
            throw std::invalid_argument("rotacionDobleIzquierda requiere nodo y hijo derecho no nulos");
        }
        nodo->derecho = rotarDerecha(nodo->derecho);
        return rotarIzquierda(nodo);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::rotacionDobleIzquierda: " << e.what() << std::endl;
        throw;
    }
}

//rotacion doble a la derecha (izquierda + derecha)
NodoAVL *ArbolAVL::rotacionDobleDerecha(NodoAVL *nodo) {
    try {
        if (!nodo || !nodo->izquierdo) {
            throw std::invalid_argument("rotacionDobleDerecha requiere nodo y hijo izquierdo no nulos");
        }
        nodo->izquierdo = rotarIzquierda(nodo->izquierdo);
        return rotarDerecha(nodo);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::rotacionDobleDerecha: " << e.what() << std::endl;
        throw;
    }
}

//balanceo
NodoAVL *ArbolAVL::balancear(NodoAVL *nodo) {
    if (!nodo) return nullptr;

    try {
        //actualizar la altura antes del balanceo
        nodo->altura = std::max(altura(nodo->izquierdo), altura(nodo->derecho)) + 1;

        int fb = factorBalance(nodo);

        //desbalanceo hacia la derecha
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
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::balancear: " << e.what() << std::endl;
        return nodo; // Devolver el nodo original en caso de error
    }
}

void ArbolAVL::recorrerEnOrdenRecursivo(NodoAVL* nodo, ListaLibros* lista) const {
    if (nodo == nullptr) return;

    try {
        //recorrer sub arbol izquierdo menores
        recorrerEnOrdenRecursivo(nodo->izquierdo, lista);

        //agregar libro actual a la lista
        if (nodo->libro) {
            lista->insertar(nodo->libro);
        }

        //recorrer sub arbol derecho mayores
        recorrerEnOrdenRecursivo(nodo->derecho, lista);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::recorrerEnOrdenRecursivo: " << e.what() << std::endl;
        throw;
    }
}

ListaLibros *ArbolAVL::obtenerLibrosEnOrdenAlfabetico() const {
    try {
        ListaLibros* lista = new ListaLibros();
        recorrerEnOrdenRecursivo(raiz, lista);
        return lista;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolAVL::obtenerLibrosEnOrdenAlfabetico: " << e.what() << std::endl;
        return new ListaLibros(); // Devolver lista vacía en caso de error
    }
}