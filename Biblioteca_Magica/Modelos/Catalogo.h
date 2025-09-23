#ifndef BIBLIOTECA_MAGICA_CATALOGO_H
#define BIBLIOTECA_MAGICA_CATALOGO_H

#include "Libro.h"
#include "../include/ListaRepetidos.h"

class Catalogo {
    private:
    int idSiguiente;
    ListaRepetidos libros;

    public:
    Catalogo();
    ~Catalogo();

    Libro* crearLibro(const std::string& titulo,
                      const std::string& isbn,
                      const std::string& genero,
                      const std::string& fecha,
                      const std::string& autor);

    bool destruirLibro(Libro* libro);
    Libro* getPorId(int id) const;
    const ListaRepetidos& getLibros() const;
};

#endif //BIBLIOTECA_MAGICA_CATALOGO_H