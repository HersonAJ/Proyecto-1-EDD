#include "Catalogo.h"

Catalogo::Catalogo() : idSiguiente(1) {}

Catalogo::~Catalogo() {
    //liberar los nodos existente
    NodoLibro* actual = libros.getCabeza();
    while (actual != nullptr) {
        delete actual->libro;
        actual = actual->siguiente;
    }
}

/*Libro *Catalogo::crearLibro(const std::string &titulo, const std::string &isbn, const std::string &genero, const std::string &fecha, const std::string &autor) {
    Libro* nuevo = new Libro(idSiguiente++, titulo, isbn, genero, fecha, autor);
    libros.insertar(nuevo);
    return nuevo;
}*/

bool Catalogo::destruirLibro(Libro* libro) {
    bool eliminado = libros.eliminar(libro);
    if (eliminado) {
        delete libro;
    }
    return eliminado;
}


const ListaRepetidos& Catalogo::getLibros() const {
    return libros;
}


