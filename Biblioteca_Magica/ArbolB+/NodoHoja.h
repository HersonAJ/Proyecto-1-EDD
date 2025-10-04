#ifndef BIBLIOTECA_MAGICA_NODOHOJA_H
#define BIBLIOTECA_MAGICA_NODOHOJA_H

#include "NodoBPlus.h"
#include "../AVL_Auxiliar/IndiceISBN.h"
#include <string>
#include <memory>

// Nodo hoja de B+ — almacena claves reales (género) y datos (IndiceISBN)
// además mantiene enlaces a hojas vecinas para recorrido secuencial.
class NodoHoja : public NodoBPlus {
public:
    struct EntradaGenero {
        std::string genero;
        std::unique_ptr<IndiceISBN> indiceISBN;

        EntradaGenero() : genero(""), indiceISBN(nullptr) {}
        EntradaGenero(const std::string& g) : genero(g), indiceISBN(std::make_unique<IndiceISBN>()) {}

        // Implementamos move-only semantics (evita copia peligrosa)
        EntradaGenero(EntradaGenero&& other) noexcept
            : genero(std::move(other.genero)),
              indiceISBN(std::move(other.indiceISBN)) {}

        EntradaGenero& operator=(EntradaGenero&& other) noexcept {
            if (this != &other) {
                genero = std::move(other.genero);
                indiceISBN = std::move(other.indiceISBN);
            }
            return *this;
        }

        // Elimina copy (no permitido por defecto)
        EntradaGenero(const EntradaGenero&) = delete;
        EntradaGenero& operator=(const EntradaGenero&) = delete;
    };

    EntradaGenero* entradas; // tamaño (2*T_BPLUS - 1)
    NodoHoja* siguiente;      // enlace a la hoja siguiente (nullptr si no existe)
    NodoHoja* anterior;       // enlace a la hoja anterior (opcional)

    NodoHoja();
    ~NodoHoja() override;


    int buscarIndiceEntrada(const std::string& genero) const; // índice dónde insertar/buscar
};

#endif // BIBLIOTECA_MAGICA_NODOHOJA_H
