#ifndef BIBLIOTECA_MAGICA_AVL_AUXILIAR_H
#define BIBLIOTECA_MAGICA_AVL_AUXILIAR_H

#include "NodoAVL_Auxiliar.h"
#include <string>

class AVL_Auxiliar {
    private:
    NodoAVL_Auxiliar* raiz;

    int altura(const NodoAVL_Auxiliar* nodo) const;
    int factorBalance(const NodoAVL_Auxiliar* nodo) const;
    NodoAVL_Auxiliar* balancear(NodoAVL_Auxiliar* nodo);

    NodoAVL_Auxiliar* rotarIzquierda(NodoAVL_Auxiliar* nodo);
    NodoAVL_Auxiliar* rotarDerecha(NodoAVL_Auxiliar* nodo);

    NodoAVL_Auxiliar* insertarNodo(NodoAVL_Auxiliar* nodo, const std::string& isbn, NodoAVL* ref);
    NodoAVL_Auxiliar* eliminarNodo(NodoAVL_Auxiliar* nodo, const std::string& isbn);
    NodoAVL_Auxiliar* nodoMinimo(NodoAVL_Auxiliar* nodo) const;

    void destruirRecursivo(const NodoAVL_Auxiliar* nodo);

    public:
    AVL_Auxiliar();
    ~AVL_Auxiliar();

    void insertar(const std::string& isbn, NodoAVL* ref);
    void eliminar(const std::string& isbn);
    NodoAVL* buscar(const std::string& isbn);

    bool estaVacio() const { return  raiz == nullptr; }
};

#endif //BIBLIOTECA_MAGICA_AVL_AUXILIAR_H