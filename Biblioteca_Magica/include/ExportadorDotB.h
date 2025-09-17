#ifndef BIBLIOTECA_MAGICA_EXPORTADORDOTB_H
#define BIBLIOTECA_MAGICA_EXPORTADORDOTB_H

#include <string>
#include <sstream>
#include <fstream>
#include "../ArbolB/ArbolB.h"
#include  "../ArbolB/NodoB.h"

class ExportarDotB {
public:
    // Genera el contenido DOT como string.
    static std::string generar(const ArbolB& arbol) {
        std::ostringstream out;
        out << "digraph BTree {\n";
        out << "  rankdir=TB;\n";
        out << "  node [shape=box, fontname=\"Helvetica\", fontsize=10];\n";
        out << "  edge [arrowhead=normal];\n\n";

        int idCounter = 0;
        NodoB* raiz = arbol.getRaiz();
        if (raiz) {
            dibujarNodo(raiz, out, idCounter);
        } else {
            out << "  vacio [label=\"Arbol B vacio\", shape=plaintext];\n";
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
    // Escapa caracteres especiales para DOT.
    static std::string escapar(const std::string& s) {
        std::string r;
        r.reserve(s.size() + 8);
        for (size_t i = 0; i < s.size(); ++i) {
            char c = s[i];
            switch (c) {
                case '\\': r += "\\\\"; break;
                case '"':  r += "\\\""; break;
                case '\n': r += "\\n";  break;
                case '\r': /* skip */   break;
                default:   r += c;      break;
            }
        }
        return r;
    }

    // Construye la etiqueta de un nodo con sus claves (una por línea).
    static std::string etiquetaNodo(NodoB* nodo) {
        // Cada clave: "[fecha] titulo"
        std::ostringstream lbl;
        for (int i = 0; i < nodo->numClaves; ++i) {
            if (i > 0) lbl << "\\n";
            if (nodo->claves[i] != nullptr) {
                const std::string fecha = nodo->claves[i]->getFecha();
                const std::string titulo = nodo->claves[i]->getTitulo();
                lbl << "[" << escapar(fecha) << "] " << escapar(titulo);
            } else {
                lbl << "<null>";
            }
        }
        // Caso sin claves (defensivo)
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