#include "ArbolB.h"
#include <iostream>
#include <functional>
#include <unordered_set>
#include <queue>
#include <stdexcept>

ArbolB::~ArbolB() {
    try {
        destruirRecursivo(raiz);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::~ArbolB: " << e.what() << std::endl;
    }
}

void ArbolB::destruirRecursivo(NodoB* nodo) {
    if (nodo) {
        try {
            // Primero destruir todas las entradas de este nodo
            for (int i = 0; i < nodo->numClaves; i++) {
                if (nodo->claves[i]) {
                    delete nodo->claves[i];
                    nodo->claves[i] = nullptr;
                }
            }

            // Luego destruir recursivamente los hijos
            if (!nodo->esHoja) {
                for (int i = 0; i <= nodo->numClaves; i++) {
                    destruirRecursivo(nodo->hijos[i]);
                }
            }

            delete nodo;
        } catch (const std::exception& e) {
            std::cerr << "Error en ArbolB::destruirRecursivo: " << e.what() << std::endl;
            throw;
        }
    }
}

// Buscar una fecha en TODO el árbol (incluyendo nodos internos)
EntradaFecha* ArbolB::buscarFechaGlobal(int fecha) {
    try {
        return buscarFechaEnNodo(raiz, fecha);
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::buscarFechaGlobal: " << e.what() << std::endl;
        return nullptr;
    }
}

EntradaFecha* ArbolB::buscarFechaEnNodo(NodoB* nodo, int fecha) {
    if (!nodo) return nullptr;

    try {
        //BÚSQUEDA BINARIA dentro del nodo - O(log T)
        int izquierda = 0;
        int derecha = nodo->numClaves - 1;

        while (izquierda <= derecha) {
            int mid = izquierda + (derecha - izquierda) / 2;

            if (!nodo->claves[mid]) {
                throw std::runtime_error("Clave nula encontrada durante búsqueda binaria");
            }

            int fechaActual = nodo->claves[mid]->fecha;

            if (fechaActual == fecha) {
                return nodo->claves[mid];  //Encontrado
            } else if (fechaActual < fecha) {
                izquierda = mid + 1;
            } else {
                derecha = mid - 1;
            }
        }

        // Si no es hoja, buscar recursivamente en el hijo adecuado
        if (!nodo->esHoja) {
            // izquierda ahora indica la posición del hijo donde debería estar
            return buscarFechaEnNodo(nodo->hijos[izquierda], fecha);
        }

        return nullptr;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::buscarFechaEnNodo: " << e.what() << std::endl;
        return nullptr;
    }
}

void ArbolB::insertar(Libro* libro) {
    try {
        if (!libro) throw std::invalid_argument("Libro nulo en ArbolB::insertar");

        int fechaInt = libro->getFechaInt();

        // PRIMERO buscar si la fecha ya existe en TODO el árbol
        EntradaFecha* entradaExistente = buscarFechaGlobal(fechaInt);

        if (entradaExistente) {
            // Fecha existe -> solo agregar ISBN
            entradaExistente->indiceISBN.insertar(libro->getIsbn(), libro);
            return;  // NO continuar con la inserción normal
        }

        // Fecha NO existe -> proceder con inserción normal
        if (raiz == nullptr) {
            raiz = new NodoB(true);
            raiz->claves[0] = new EntradaFecha(fechaInt);
            raiz->claves[0]->indiceISBN.insertar(libro->getIsbn(), libro);
            raiz->numClaves = 1;
        } else {
            if (raiz->numClaves == 2*T - 1) {
                NodoB* nuevaRaiz = new NodoB(false);
                nuevaRaiz->hijos[0] = raiz;
                dividirHijo(nuevaRaiz, 0);
                raiz = nuevaRaiz;

                int i = 0;
                if (fechaInt > nuevaRaiz->claves[0]->fecha) {
                    i++;
                }
                insertarNoLleno(nuevaRaiz->hijos[i], fechaInt, libro);
            } else {
                insertarNoLleno(raiz, fechaInt, libro);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::insertar: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::dividirHijo(NodoB* padre, int indice) {
    try {
        NodoB* hijo = padre->hijos[indice];
        if (!hijo) throw std::runtime_error("Hijo nulo en dividirHijo");

        NodoB* nuevoHijo = new NodoB(hijo->esHoja);
        nuevoHijo->numClaves = T - 1;

        // TRANSFERIR claves al nuevo hijo
        for (int j = 0; j < T - 1; j++) {
            nuevoHijo->claves[j] = hijo->claves[j + T];
            hijo->claves[j + T] = nullptr;
        }

        if (!hijo->esHoja) {
            for (int j = 0; j < T; j++) {
                nuevoHijo->hijos[j] = hijo->hijos[j + T];
                hijo->hijos[j + T] = nullptr;
            }
        }

        // Guardar la clave promovida y LIMPIARLA del hijo
        EntradaFecha* clavePromovida = hijo->claves[T - 1];
        if (!clavePromovida) throw std::runtime_error("Clave promovida nula en dividirHijo");

        hijo->claves[T - 1] = nullptr;
        hijo->numClaves = T - 1;

        // Reorganizar hijos del padre
        for (int j = padre->numClaves; j >= indice + 1; j--) {
            padre->hijos[j + 1] = padre->hijos[j];
        }
        padre->hijos[indice + 1] = nuevoHijo;

        // Reorganizar claves del padre
        for (int j = padre->numClaves - 1; j >= indice; j--) {
            padre->claves[j + 1] = padre->claves[j];
        }

        padre->claves[indice] = clavePromovida;
        padre->numClaves++;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::dividirHijo: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::insertarNoLleno(NodoB* nodo, int fecha, Libro* libro) {
    try {
        if (!nodo) throw std::invalid_argument("Nodo nulo en insertarNoLleno");
        if (!libro) throw std::invalid_argument("Libro nulo en insertarNoLleno");

        int i = nodo->numClaves - 1;

        if (nodo->esHoja) {
            // Este método SOLO se llama para FECHAS NUEVAS
            int pos = 0;
            while (pos < nodo->numClaves && fecha > nodo->claves[pos]->fecha) {
                pos++;
            }

            // Desplazar y crear nueva entrada
            for (int j = nodo->numClaves; j > pos; j--) {
                nodo->claves[j] = nodo->claves[j - 1];
            }

            nodo->claves[pos] = new EntradaFecha(fecha);
            nodo->claves[pos]->indiceISBN.insertar(libro->getIsbn(), libro);
            nodo->numClaves++;

        } else {
            // Buscar hijo adecuado
            while (i >= 0 && fecha < nodo->claves[i]->fecha) {
                i--;
            }
            i++;

            if (nodo->hijos[i]->numClaves == 2 * T - 1) {
                dividirHijo(nodo, i);
                if (fecha > nodo->claves[i]->fecha) {
                    i++;
                }
            }
            insertarNoLleno(nodo->hijos[i], fecha, libro);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::insertarNoLleno: " << e.what() << std::endl;
        throw;
    }
}

void ArbolB::verificarDuplicados() {
    try {
        std::unordered_set<int> fechasVistas;
        std::function<void(NodoB*)> verificarNodo = [&](NodoB* nodo) {
            if (!nodo) return;

            for (int i = 0; i < nodo->numClaves; i++) {
                if (!nodo->claves[i]) continue;

                int fecha = nodo->claves[i]->fecha;
                if (fechasVistas.count(fecha)) {
                    std::cout << "DUPLICADO ENCONTRADO: " << fecha << std::endl;
                } else {
                    fechasVistas.insert(fecha);
                }
            }

            if (!nodo->esHoja) {
                for (int i = 0; i <= nodo->numClaves; i++) {
                    verificarNodo(nodo->hijos[i]);
                }
            }
        };

        verificarNodo(raiz);
        std::cout << "Verificación completada. " << fechasVistas.size() << " fechas únicas." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolB::verificarDuplicados: " << e.what() << std::endl;
    }
}