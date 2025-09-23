#include "Libro.h"
#include <string>
#include  <sstream>

// Constructor vacío
Libro::Libro() : titulo(""), isbn(""), genero(""), fecha(""), autor("") {}
//constructor con parametros
Libro::Libro(const std::string &titulo,
    const std::string &isbn,
    const std::string &genero,
    const std::string &fecha,
    const std::string &autor)
    : titulo(titulo), isbn(isbn), genero(genero), fecha(fecha), autor(autor) {}

//getters
std::string Libro::getTitulo() const {
    return titulo;
}

std::string Libro::getIsbn() const {
    return isbn;
}
std::string Libro::getGenero() const {
    return genero;
}
std::string Libro::getFecha() const {
    return fecha;
}
std::string Libro::getAutor() const {
    return autor;
}

//setters
void Libro::setTitulo(const std::string& titulo) {
    this->titulo = titulo;
}
void Libro::setIsbn(const std::string& isbn) {
    this->isbn = isbn;
}
void Libro::setGenero(const std::string& genero) {
    this->genero = genero;
}
void Libro::setFecha(const std::string& fecha) {
    this->fecha = fecha;
}
void Libro::setAutor(const std::string& autor) {
    this->autor = autor;
}

std::string Libro::toString() const {
    std::ostringstream oss;
    oss << "Titulo: " << (titulo.empty() ? "<vacío>" : titulo)
        << " | ISBN: " << (isbn.empty() ? "<vacío>" : isbn)
        << " | Genero: " << (genero.empty() ? "<vacío>" : genero)
        << " | Fecha: " << (fecha.empty() ? "<vacío>" : fecha)
        << " | Autor: " << (autor.empty() ? "<vacío>" : autor);
    return oss.str();
}


int Libro::compararPorTitulo(const Libro& otro) const {
    return  titulo.compare(otro.titulo);
}

int Libro::compararPorIsbn(const Libro& otro) const {
    return isbn.compare(otro.isbn);
}

int Libro::compararPorFecha(const Libro& otro) const {
    try {
        int anio1 =fecha.empty() ? 0 : std::stoi(fecha);
        int anio2 =otro.fecha.empty() ? 0 : std::stoi(otro.fecha);
        return anio1 - anio2;
    } catch (const std::invalid_argument&) {
        return  fecha.compare(otro.fecha);
    }
}

int Libro::getFechaInt() const {
    try {
        return  fecha.empty() ? 0 : std::stoi(fecha);
    } catch (const std::invalid_argument&) {
        return 0;
    }
}


