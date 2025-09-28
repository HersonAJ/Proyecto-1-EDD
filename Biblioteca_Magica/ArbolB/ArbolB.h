#ifndef BIBLIOTECA_MAGICA_ARBOLB_H
#define BIBLIOTECA_MAGICA_ARBOLB_H

#include "NodoB.h"
#include "ListaLibros.h"

class ArbolB {
private:
    NodoB* raiz;

    // Métodos internos que trabajan con fecha como int
    void dividirHijo(NodoB* padre, int i);
    void insertarNoLleno(NodoB* nodo, int fecha, Libro* libro);
    void buscarRangoRecursivo(NodoB* nodo, int inicio, int fin, ListaLibros& resultados);
    void destruirRecursivo(NodoB* nodo);

public:
    ArbolB() : raiz(nullptr) {}
    ~ArbolB();  // Destructor

    // Inserción de un libro (usa fecha como clave única y lo guarda en el IndiceISBN de esa fecha)
    void insertar(Libro* libro);

    // Búsqueda por rango de fechas (devuelve lista de libros)
    ListaLibros buscarPorRangoFechas(const std::string& inicio, const std::string& fin);

    // Búsqueda individual por fecha (devuelve un libro cualquiera de esa fecha, o nullptr si no existe)
    Libro* buscarPorFecha(const std::string& fecha);

    void eliminar(const std::string& fecha);
    void imprimir();
    NodoB* getRaiz() const { return raiz; }
    void imprimirParaPrueba();
};
#endif // BIBLIOTECA_MAGICA_ARBOLB_H