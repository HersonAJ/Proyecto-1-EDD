#ifndef BIBLIOTECA_MAGICA_ARBOLB_H
#define BIBLIOTECA_MAGICA_ARBOLB_H

#include  "NodoB.h"
#include "ListaLibros.h"

class ArbolB {
private:
    NodoB* raiz;

    // Métodos que usan fecha como int internamente
    void dividirHijo(NodoB* padre, int i);
    void insertarNoLleno(NodoB* nodo, int fecha, Libro* libro);
    void buscarRangoRecursivo(NodoB* nodo, int inicio, int fin, ListaLibros& resultados);
    void destruirRecursivo(NodoB* nodo);

public:
    ArbolB() : raiz(nullptr) {}
    ~ArbolB();  // Destructor

    // Interfaz pública usa string (como documentación)
    void insertar(Libro* libro);
    ListaLibros buscarPorRangoFechas(const std::string& inicio, const std::string& fin);

    // Búsqueda individual
    Libro* buscarPorFecha(const std::string& fecha);

    void eliminar(const std::string& fecha);
    void imprimir();
    NodoB* getRaiz() const { return raiz; }

    void imprimirParaPrueba();
};
#endif //BIBLIOTECA_MAGICA_ARBOLB_H