#ifndef BIBLIOTECA_MAGICA_EXPORTADORDOTB_H
#define BIBLIOTECA_MAGICA_EXPORTADORDOTB_H

#include <string>
#include <sstream>
#include <fstream>
#include "../ArbolB/ArbolB.h"
#include "../ArbolB/NodoB.h"

class ExportarDotB {
public:
    // Genera el contenido DOT como string.
    static std::string generar(const ArbolB& arbol) {
        std::ostringstream out;
        out << "digraph arbol_b {\n";
        out << "  node [shape=box, style=filled, fontname=\"Helvetica\", fontsize=10];\n\n";

        int idCounter = 0;
        NodoB* raiz = arbol.getRaiz();
        if (raiz) {
            dibujarNodo(raiz, out, idCounter);
        } else {
            out << "  vacio [label=\"Árbol B vacío\", shape=plaintext];\n";
        }

        out << "}\n";
        return out.str();
    }

    // Genera el archivo DOT directamente.
    static bool generarArchivo(const ArbolB& arbol, const std::string& ruta) {
        std::string dot = generar(arbol);
        std::ofstream f(ruta.c_str());
        if (!f.is_open()) return false;
        f << dot;
        f.close();
        return true;
    }

private:
    // Construye la etiqueta de un nodo con sus claves separadas por coma.
    static std::string etiquetaNodo(NodoB* nodo) {
        std::ostringstream lbl;
        for (int i = 0; i < nodo->numClaves; ++i) {
            if (i > 0) lbl << ", ";
            if (nodo->claves[i] != nullptr) {
                lbl << nodo->claves[i]->fecha; // solo mostramos la fecha
            } else {
                lbl << "<null>";
            }
        }
        if (nodo->numClaves == 0) {
            lbl << "(sin claves)";
        }
        return lbl.str();
    }

    // Dibuja el subárbol y devuelve el id asignado al nodo actual.
    static int dibujarNodo(NodoB* nodo, std::ostringstream& out, int& idCounter) {
        if (!nodo) return -1;

        int myId = idCounter++;
        out << "  n" << myId << " [label=\"" << etiquetaNodo(nodo) << "\"];\n";

        // Conectar hijos en orden
        if (!nodo->esHoja) {
            for (int i = 0; i <= nodo->numClaves; ++i) {
                if (nodo->hijos[i] != nullptr) {
                    int childId = dibujarNodo(nodo->hijos[i], out, idCounter);
                    if (childId >= 0) {
                        out << "  n" << myId << " -> n" << childId << ";\n";
                    }
                }
            }
        }
        return myId;
    }
};

#endif //BIBLIOTECA_MAGICA_EXPORTADORDOTB_H
