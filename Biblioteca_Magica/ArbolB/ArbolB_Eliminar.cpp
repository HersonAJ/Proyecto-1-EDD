#include <iostream>
#include <ostream>
#include "ArbolB.h"
#include <stdexcept>

void ArbolB::eliminarPorISBN(const std::string& isbn, const std::string& fecha) {
    try {
        if (fecha.empty()) {
            std::cout << "Fecha vacía para ISBN: " << isbn << std::endl;
            return;
        }

        // Convertir fecha a entero
        int fechaInt;
        try {
            fechaInt = std::stoi(fecha);
        } catch (const std::exception& e) {
            std::cout << "Error convirtiendo fecha: '" << fecha << "' para ISBN: " << isbn << std::endl;
            return;
        }

        // Buscar la clave de esa fecha en el árbol
        NodoB* nodo = raiz;
        while (nodo) {
            int i = 0;
            while (i < nodo->numClaves && fechaInt > nodo->claves[i]->fecha) {
                i++;
            }

            if (i < nodo->numClaves && nodo->claves[i]->fecha == fechaInt) {
                // Se encontró la fecha
                nodo->claves[i]->indiceISBN.eliminar(isbn);

                // Si el índice ISBN local se queda vacío se elimina la clave completamente en el B
                if (nodo->claves[i]->indiceISBN.estaVacio()) {
                    std::string fechaStr = std::to_string(fechaInt);
                    eliminar(fechaStr);
                }
                return;
            }

            if (nodo->esHoja) break;
            nodo = nodo->hijos[i];
        }

        std::cout << "No se encontró la fecha asociada al ISBN en el árbol B: " << isbn << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::eliminarPorISBN: " << e.what() << std::endl;
    }
}

void ArbolB::eliminar(const std::string& fechaStr) {
    try {
        if (!raiz) return;

        int fechaInt = std::stoi(fechaStr);
        eliminarRecursivo(raiz, fechaInt);

        // Si la raíz queda sin claves y no es hoja, subir el primer hijo
        if (raiz->numClaves == 0 && !raiz->esHoja) {
            NodoB* viejaRaiz = raiz;
            raiz = raiz->hijos[0];
            delete viejaRaiz;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::eliminar: " << e.what() << std::endl;
    }
}

EntradaFecha* ArbolB::obtenerMaximo(NodoB* nodo) {
    try {
        if (!nodo) throw std::invalid_argument("Nodo nulo en obtenerMaximo");

        NodoB* actual = nodo;
        while (!actual->esHoja) {
            if (!actual->hijos[actual->numClaves]) {
                throw std::runtime_error("Hijo nulo en obtenerMaximo");
            }
            actual = actual->hijos[actual->numClaves]; // último hijo
        }

        if (actual->numClaves == 0 || !actual->claves[actual->numClaves - 1]) {
            throw std::runtime_error("No hay claves válidas en obtenerMaximo");
        }

        return actual->claves[actual->numClaves - 1];
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::obtenerMaximo: " << e.what() << std::endl;
        return nullptr;
    }
}

EntradaFecha* ArbolB::obtenerMinimo(NodoB* nodo) {
    try {
        if (!nodo) throw std::invalid_argument("Nodo nulo en obtenerMinimo");

        NodoB* actual = nodo;
        while (!actual->esHoja) {
            if (!actual->hijos[0]) {
                throw std::runtime_error("Hijo nulo en obtenerMinimo");
            }
            actual = actual->hijos[0];
        }

        if (actual->numClaves == 0 || !actual->claves[0]) {
            throw std::runtime_error("No hay claves válidas en obtenerMinimo");
        }

        return actual->claves[0];
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::obtenerMinimo: " << e.what() << std::endl;
        return nullptr;
    }
}

void ArbolB::fusionar(NodoB* nodo, int idx) {
    try {
        if (!nodo) throw std::invalid_argument("Nodo nulo en fusionar");
        if (idx < 0 || idx >= nodo->numClaves) throw std::out_of_range("Índice inválido en fusionar");

        NodoB* hijoIzq = nodo->hijos[idx];
        NodoB* hijoDer = nodo->hijos[idx + 1];

        if (!hijoIzq || !hijoDer) throw std::runtime_error("Hijos nulos en fusionar");

        // mover la clave del padre hacia hijoIzq (movemos el puntero)
        hijoIzq->claves[hijoIzq->numClaves] = nodo->claves[idx];

        // copiar claves de hijoDer a hijoIzq
        for (int i = 0; i < hijoDer->numClaves; ++i) {
            hijoIzq->claves[hijoIzq->numClaves + 1 + i] = hijoDer->claves[i];
        }

        // copiar hijos si no son hojas
        if (!hijoIzq->esHoja) {
            for (int i = 0; i <= hijoDer->numClaves; ++i) {
                hijoIzq->hijos[hijoIzq->numClaves + 1 + i] = hijoDer->hijos[i];
            }
        }

        // actualizar cuenta de claves en hijoIzq
        hijoIzq->numClaves += 1 + hijoDer->numClaves;

        // mover claves e hijos del nodo padre a la izquierda para "eliminar" clave idx
        for (int i = idx; i < nodo->numClaves - 1; ++i) {
            nodo->claves[i] = nodo->claves[i + 1];
            nodo->hijos[i + 1] = nodo->hijos[i + 2];
        }

        // limpiar referencias sobrantes
        nodo->claves[nodo->numClaves - 1] = nullptr;
        nodo->hijos[nodo->numClaves] = nullptr;
        nodo->numClaves--;

        // liberar el nodo derecho (sus punteros ya fueron movidos a hijoIzq)
        delete hijoDer;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::fusionar: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::prestarDeIzquierda(NodoB* nodo, int idx) {
    try {
        if (!nodo) throw std::invalid_argument("Nodo nulo en prestarDeIzquierda");
        if (idx <= 0 || idx > nodo->numClaves) throw std::out_of_range("Índice inválido en prestarDeIzquierda");

        NodoB* hijo = nodo->hijos[idx];
        NodoB* hermano = nodo->hijos[idx - 1];

        if (!hijo || !hermano) throw std::runtime_error("Hijos nulos en prestarDeIzquierda");

        // desplazar claves del hijo a la derecha para abrir el puesto 0
        for (int i = hijo->numClaves - 1; i >= 0; --i) {
            hijo->claves[i + 1] = hijo->claves[i];
        }
        if (!hijo->esHoja) {
            for (int i = hijo->numClaves; i >= 0; --i) {
                hijo->hijos[i + 1] = hijo->hijos[i];
            }
        }

        // bajar la clave del padre al hijo (movemos puntero)
        hijo->claves[0] = nodo->claves[idx - 1];

        // si el hermano no es hoja, mover su último hijo como hijo->hijos[0]
        if (!hermano->esHoja) {
            hijo->hijos[0] = hermano->hijos[hermano->numClaves];
            hermano->hijos[hermano->numClaves] = nullptr;
        }

        // subir la última clave del hermano al padre
        nodo->claves[idx - 1] = hermano->claves[hermano->numClaves - 1];
        hermano->claves[hermano->numClaves - 1] = nullptr;

        hijo->numClaves++;
        hermano->numClaves--;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::prestarDeIzquierda: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::prestarDeDerecha(NodoB* nodo, int idx) {
    try {
        if (!nodo) throw std::invalid_argument("Nodo nulo en prestarDeDerecha");
        if (idx < 0 || idx >= nodo->numClaves) throw std::out_of_range("Índice inválido en prestarDeDerecha");

        NodoB* hijo = nodo->hijos[idx];
        NodoB* hermano = nodo->hijos[idx + 1];

        if (!hijo || !hermano) throw std::runtime_error("Hijos nulos en prestarDeDerecha");

        // bajar la clave del padre al final de hijo
        hijo->claves[hijo->numClaves] = nodo->claves[idx];

        // si no es hoja, mover primer hijo del hermano como hijo
        if (!hijo->esHoja) {
            hijo->hijos[hijo->numClaves + 1] = hermano->hijos[0];
        }

        // subir la primera clave del hermano al padre
        nodo->claves[idx] = hermano->claves[0];

        // desplazar claves del hermano a la izquierda
        for (int i = 1; i < hermano->numClaves; ++i) {
            hermano->claves[i - 1] = hermano->claves[i];
        }
        if (!hermano->esHoja) {
            for (int i = 1; i <= hermano->numClaves; ++i) {
                hermano->hijos[i - 1] = hermano->hijos[i];
            }
        }

        // limpiar última referencia
        hermano->claves[hermano->numClaves - 1] = nullptr;
        hermano->hijos[hermano->numClaves] = nullptr;

        hijo->numClaves++;
        hermano->numClaves--;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::prestarDeDerecha: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::eliminarRecursivo(NodoB* nodo, int fecha) {
    try {
        if (!nodo) return;

        int idx = 0;
        while (idx < nodo->numClaves && fecha > nodo->claves[idx]->fecha) {
            idx++;
        }

        // 1) clave encontrada en este nodo
        if (idx < nodo->numClaves && nodo->claves[idx]->fecha == fecha) {
            if (nodo->esHoja) {
                // Caso 1: nodo hoja -> eliminar entrada (liberar memoria)
                delete nodo->claves[idx];
                for (int i = idx; i < nodo->numClaves - 1; ++i) {
                    nodo->claves[i] = nodo->claves[i + 1];
                }
                nodo->claves[nodo->numClaves - 1] = nullptr;
                nodo->numClaves--;
                return;
            } else {
                // Caso 2: clave en nodo interno
                if (nodo->hijos[idx]->numClaves >= T) {
                    // 2a: tomar predecesor
                    EntradaFecha* pred = obtenerMaximo(nodo->hijos[idx]);
                    if (!pred) throw std::runtime_error("Predecesor nulo en eliminación");
                    // copiar contenido del predecesor al lugar de la clave (no mover puntero)
                    *(nodo->claves[idx]) = *pred;
                    // eliminar el predecesor recursivamente
                    eliminarRecursivo(nodo->hijos[idx], pred->fecha);
                } else if (nodo->hijos[idx + 1]->numClaves >= T) {
                    // 2b: tomar sucesor
                    EntradaFecha* succ = obtenerMinimo(nodo->hijos[idx + 1]);
                    if (!succ) throw std::runtime_error("Sucesor nulo en eliminación");
                    *(nodo->claves[idx]) = *succ;
                    eliminarRecursivo(nodo->hijos[idx + 1], succ->fecha);
                } else {
                    // 2c: ambos hijos tienen T-1 -> fusionar y eliminar en el hijo fusionado
                    fusionar(nodo, idx);
                    eliminarRecursivo(nodo->hijos[idx], fecha);
                }
            }
        } else {
            // 2) clave NO está en este nodo
            if (nodo->esHoja) {
                // no existe la clave
                return;
            }

            bool flagUltima = (idx == nodo->numClaves);

            if (nodo->hijos[idx]->numClaves < T) {
                if (idx > 0 && nodo->hijos[idx - 1]->numClaves >= T) {
                    // pedir prestado del hermano izquierdo
                    prestarDeIzquierda(nodo, idx);
                } else if (idx < nodo->numClaves && nodo->hijos[idx + 1]->numClaves >= T) {
                    // pedir prestado del hermano derecho
                    prestarDeDerecha(nodo, idx);
                } else {
                    // fusionar con un hermano
                    if (idx < nodo->numClaves) {
                        fusionar(nodo, idx);
                    } else {
                        fusionar(nodo, idx - 1);
                        idx--; // ahora descendemos al hijo fusionado
                    }
                }
            }

            // decidir en qué hijo bajar: si originalmente era el último y después de fill
            if (flagUltima && idx > nodo->numClaves) {
                eliminarRecursivo(nodo->hijos[idx - 1], fecha);
            } else {
                eliminarRecursivo(nodo->hijos[idx], fecha);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::eliminarRecursivo: " << e.what() << std::endl;
        throw;
    }
}