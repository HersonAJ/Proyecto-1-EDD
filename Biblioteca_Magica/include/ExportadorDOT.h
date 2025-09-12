#ifndef BIBLIOTECA_MAGICA_EXPORTADORDOT_H
#define BIBLIOTECA_MAGICA_EXPORTADORDOT_H

#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

template <typename Nodo>
class ExportadorDOT {
public:
    static void exportar(const Nodo* raiz, const std::string& rutaArchivo) {
        std::ofstream file(rutaArchivo);
        if (!file.is_open()) {
            std::cerr << "Error: no se pudo crear el archivo DOT en " << rutaArchivo << std::endl;
            return;
        }

        file << "digraph Arbol {\n";
        file << "    rankdir=TB;\n"; // orientación de arriba hacia abajo
        file << "    node [shape=record, style=filled, fillcolor=lightyellow, fontname=\"Arial\", fontsize=10];\n";
        recorrer(raiz, file);
        file << "}\n";

        file.close();
    }

private:
    static std::string escapeLabel(const std::string& s) {
        std::string out;
        out.reserve(s.size());
        for (char c : s) {
            if (c == '"' || c == '\\') out.push_back('\\');
            out.push_back(c);
        }
        return out;
    }

    static std::string nodeId(const Nodo* nodo) {
        std::ostringstream oss;
        oss << "n" << reinterpret_cast<uintptr_t>(nodo);
        return oss.str();
    }

    // Nodo con tres secciones: izquierda | título | derecha
    static void declararNodo(const Nodo* nodo, std::ofstream& out) {
        out << "    " << nodeId(nodo)
            << " [label=\"<L> | " << escapeLabel(nodo->dato.getTitulo()) << " | <R>\"];\n";
    }

    static void recorrer(const Nodo* nodo, std::ofstream& out) {
        if (!nodo) return;

        declararNodo(nodo, out);

        if (nodo->izquierdo) {
            declararNodo(nodo->izquierdo, out);
            out << "    " << nodeId(nodo) << ":L -> " << nodeId(nodo->izquierdo) << ";\n";
            recorrer(nodo->izquierdo, out);
        }
        if (nodo->derecho) {
            declararNodo(nodo->derecho, out);
            out << "    " << nodeId(nodo) << ":R -> " << nodeId(nodo->derecho) << ";\n";
            recorrer(nodo->derecho, out);
        }
    }
};

#endif //BIBLIOTECA_MAGICA_EXPORTADORDOT_H