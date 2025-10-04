#include "Catalogo.h"

Catalogo::Catalogo() : cabeza(nullptr), cola(nullptr), tamaño(0) {}

Catalogo::~Catalogo() {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        Nodo* temp = actual;
        actual = actual->siguiente;
        delete temp->libro;  //LIBERAR EL LIBRO
        delete temp;
    }
}

void Catalogo::agregarLibro(Libro* libro) {
    Nodo* nuevoNodo = new Nodo(libro);

    if (estaVacio()) {
        cabeza = nuevoNodo;
        cola = nuevoNodo;
    } else {
        cola->siguiente = nuevoNodo;
        cola = nuevoNodo;
    }
    tamaño++;
}

bool Catalogo::eliminarLibroPorISBN(const std::string& isbn) {
    if (estaVacio()) return false;

    // Caso especial: eliminar cabeza
    if (cabeza->libro->getIsbn() == isbn) {
        Nodo* temp = cabeza;
        cabeza = cabeza->siguiente;
        if (cabeza == nullptr) cola = nullptr; // Lista queda vacía
        delete temp;
        tamaño--;
        return true;
    }

    // Buscar en el resto de la lista
    Nodo* actual = cabeza;
    while (actual->siguiente != nullptr) {
        if (actual->siguiente->libro->getIsbn() == isbn) {
            Nodo* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;

            if (temp == cola) {
                cola = actual; // Actualizar cola si eliminamos el último
            }

            delete temp;
            tamaño--;
            return true;
        }
        actual = actual->siguiente;
    }

    return false;
}

bool Catalogo::estaVacio() const {
    return cabeza == nullptr;
}

int Catalogo::getTamaño() const {
    return tamaño;
}

Libro* Catalogo::buscarTituloSecuencial(const std::string& titulo) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->libro->getTitulo() == titulo) {
            return actual->libro;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}

Libro* Catalogo::buscarISBNSecuencial(const std::string& isbn) {
    Nodo* actual = cabeza;
    while (actual != nullptr) {
        if (actual->libro->getIsbn() == isbn) {
            return actual->libro;
        }
        actual = actual->siguiente;
    }
    return nullptr;
}