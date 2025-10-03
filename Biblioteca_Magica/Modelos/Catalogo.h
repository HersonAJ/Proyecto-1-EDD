#ifndef BIBLIOTECA_MAGICA_CATALOGO_H
#define BIBLIOTECA_MAGICA_CATALOGO_H

#include "../include/Nodo.h"
#include "Libro.h"

class Catalogo {
    private:
    Nodo* cabeza;
    Nodo* cola;
    int tamaño;

    public:
    Catalogo();
    ~Catalogo();

    //operaciones basicas
    void agregarLibro(Libro* libro);
    bool eliminarLibroPorISBN(const std::string& isbn);
    bool estaVacio() const;
    int getTamaño() const;

    //busquedas secuenciasel para la medicion de desempeño
    Libro* buscarTituloSecuencial(const std::string& titulo);
    Libro* buscarISBNSecuencial(const std::string& isbn);

    //iterador
    class Iterador {
        private:
        Nodo* actual;
        public:
        Iterador(Nodo* nodo) : actual(nodo) {}
        bool tieneSiguiente() const { return actual != nullptr;}
        Libro* siguiente() {
            Libro* libro = actual->libro;
            actual = actual->siguiente;
            return libro;
        }
    };

    Iterador obtenerIterador() const { return Iterador(cabeza); }
};

#endif //BIBLIOTECA_MAGICA_CATALOGO_H