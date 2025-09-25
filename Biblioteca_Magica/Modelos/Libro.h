#ifndef BIBLIOTECA_MAGICA_LIBRO_H
#define BIBLIOTECA_MAGICA_LIBRO_H

#include  <string>

class Libro {
private:
    int cantidad;
    std::string titulo;
    std::string isbn;
    std::string genero;
    std::string fecha;
    std::string autor;

public:
    Libro();
    Libro(const std::string& titulo, const std::string& isbn, const std::string& genero, const std::string& fecha, const std::string& autor);

    //getters
    std::string getTitulo() const;
    std::string getIsbn() const;
    std::string getGenero() const;
    std::string getFecha() const;
    std::string getAutor() const;
    int getCantidad() const;

    //setters
    void setTitulo(const std::string& titulo);
    void setIsbn(const std::string& isbn);
    void setGenero(const std::string& genero);
    void setFecha(const std::string& fecha);
    void setAutor(const std::string& autor);

    void incrementarCantidad();

    std::string toString() const;

    //comparaciones para organizar los nodos
    int compararPorTitulo(const Libro& libro) const;
    int compararPorIsbn(const Libro& libro) const;
    int compararPorFecha(const Libro& libro) const;

    //sobrecarga para usar la fecha como entero
    int getFechaInt() const;
};

#endif //BIBLIOTECA_MAGICA_LIBRO_H