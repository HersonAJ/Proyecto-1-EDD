#ifndef BIBLIOTECA_MAGICA_EXPORTADORDOTB_H
#define BIBLIOTECA_MAGICA_EXPORTADORDOTB_H

#include <string>
#include <sstream>
#include <fstream>
#include "../ArbolB+/ArbolBPlus.h"
#include "../ArbolB+/NodoBPlus.h"
#include "../ArbolB+/NodoInterno.h"
#include "../ArbolB+/NodoHoja.h"
#include <vector>

class ExportarDotBPlus {
public:
    // Genera el contenido DOT como string.
    static std::string generar(const ArbolBPlus& arbol) {
        std::ostringstream out;
        out << "digraph arbol_bplus {\n";
        out << "  node [shape=box, style=filled, fontname=\"Helvetica\", fontsize=10];\n";
        out << "  edge [arrowhead=vee];\n\n";

        int idCounter = 0;
        NodoBPlus* raiz = arbol.getRaiz();
        if (raiz) {
            dibujarNodo(raiz, out, idCounter);

            // Dibujar enlaces entre hojas (azules)
            dibujarEnlacesHojas(arbol, out);
        } else {
            out << "  vacio [label=\"Árbol B+ vacío\", shape=plaintext];\n";
        }

        out << "}\n";
        return out.str();
    }

    // Genera el archivo DOT directamente.
    static bool generarArchivo(const ArbolBPlus& arbol, const std::string& ruta) {
        std::string dot = generar(arbol);
        std::ofstream f(ruta.c_str());
        if (!f.is_open()) return false;
        f << dot;
        f.close();
        return true;
    }

private:
    static std::string etiquetaNodoInterno(const NodoInterno* nodo) {
        std::ostringstream lbl;
        for (int i = 0; i < nodo->numClaves; ++i) {
            if (i > 0) lbl << " | ";
            lbl << nodo->claves[i];
        }
        return lbl.str();
    }

    static std::string etiquetaNodoHoja(const NodoHoja* nodo) {
        std::ostringstream lbl;
        for (int i = 0; i < nodo->numClaves; ++i) {
            if (i > 0) lbl << " | ";
            lbl << nodo->entradas[i].genero;
        }
        return lbl.str();
    }

    // Dibuja el subárbol
    static int dibujarNodo(NodoBPlus* nodo, std::ostringstream& out, int& idCounter) {
        if (!nodo) return -1;

        int myId = idCounter++;

        if (!nodo->esHoja) {
            NodoInterno* interno = static_cast<NodoInterno*>(nodo);
            out << "  n" << myId << " [label=\"";
            out << etiquetaNodoInterno(interno);
            out << "\", fillcolor=lightblue];\n";


            for (int i = 0; i <= interno->numClaves; ++i) {
                if (interno->hijos[i] != nullptr) {
                    int childId = dibujarNodo(interno->hijos[i], out, idCounter);
                    if (childId >= 0) {
                        out << "  n" << myId << " -> n" << childId << ";\n";
                    }
                }
            }
        } else {

            NodoHoja* hoja = static_cast<NodoHoja*>(nodo);
            out << "  n" << myId << " [label=\"";
            out << etiquetaNodoHoja(hoja);
            out << "\", fillcolor=lightgreen];\n";
        }
        return myId;
    }

    // Dibuja los enlaces entre hojas (para recorrido secuencial)
    static void dibujarEnlacesHojas(const ArbolBPlus& arbol, std::ostringstream& out) {
        out << "  // Enlaces entre hojas\n";
        out << "  edge [color=blue, constraint=false, style=dashed];\n";

        NodoHoja* actual = arbol.getPrimeraHoja();
        int prevId = -1;
        int currentId = 0;


        std::vector<int> idsHojas;
        NodoHoja* temp = actual;
        while (temp) {
            idsHojas.push_back(currentId++);
            temp = temp->siguiente;
        }
        
        actual = arbol.getPrimeraHoja();
        currentId = 0;
        while (actual && actual->siguiente) {
            out << "  n" << idsHojas[currentId] << " -> n" << idsHojas[currentId + 1] << ";\n";
            actual = actual->siguiente;
            currentId++;
        }

        out << "  edge [color=black, constraint=true, style=solid];\n";
    }
};

#endif //BIBLIOTECA_MAGICA_EXPORTADORDOTB_H