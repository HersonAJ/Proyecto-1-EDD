#include <iostream>
#include <ostream>
#include "ArbolBPlus.h"
#include <stdexcept>

bool ArbolBPlus::buscarPosicionGenero(const std::string& genero, NodoHoja** hojaResultado, int& posicionResultado) const {
    try {
        if (genero.empty()) throw std::invalid_argument("Género vacío en buscarPosicionGenero");
        if (!hojaResultado) throw std::invalid_argument("Puntero de hoja resultado nulo");

        *hojaResultado = buscarHoja(genero);
        if (!*hojaResultado) {
            throw std::runtime_error("Hoja nula retornada por buscarHoja");
        }

        for (int i = 0; i < (*hojaResultado)->numClaves; i++) {
            if ((*hojaResultado)->entradas[i].genero == genero) {
                posicionResultado = i;
                return true;
            }
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::buscarPosicionGenero: " << e.what() << std::endl;
        if (hojaResultado) *hojaResultado = nullptr;
        posicionResultado = -1;
        return false;
    }
}

void ArbolBPlus::eliminarPorISBN(const std::string& isbn, const std::string& genero) {
    try {
        // Ya no busca en indiceGeneral, recibe el género directamente
        if (genero.empty()) {
            std::cout << "Género vacío para ISBN: " << isbn << std::endl;
            return;
        }

        // Buscar la posición del género en el B+
        NodoHoja* hoja;
        int posicion;
        bool generoEncontrado = buscarPosicionGenero(genero, &hoja, posicion);

        if (!generoEncontrado) {
            std::cout << "Género '" << genero << "' no encontrado en el árbol B+." << std::endl;
            return;
        }

        // Eliminar el libro del AVL específico
        if (!hoja->entradas[posicion].indiceISBN) {
            throw std::runtime_error("IndiceISBN nulo en eliminarPorISBN");
        }

        hoja->entradas[posicion].indiceISBN->eliminar(isbn);

        // Verificar si el AVL quedó vacío después de eliminar
        if (hoja->entradas[posicion].indiceISBN->estaVacio()) {
            eliminarGeneroDeHoja(hoja, posicion);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::eliminarPorISBN: " << e.what() << std::endl;
    }
}

void ArbolBPlus::eliminarGeneroDeHoja(NodoHoja* hoja, int posicion) {
    try {
        if (!hoja) throw std::invalid_argument("Hoja nula en eliminarGeneroDeHoja");
        if (posicion < 0 || posicion >= hoja->numClaves) {
            throw std::out_of_range("Posición inválida en eliminarGeneroDeHoja");
        }

        // Eliminar la entrada desplazando las demás
        for (int i = posicion; i < hoja->numClaves - 1; i++) {
            hoja->entradas[i] = std::move(hoja->entradas[i + 1]);
        }
        hoja->numClaves--;

        // Verificar si la hoja queda con muy pocas entradas
        if (hoja->numClaves < T_BPLUS - 1) {
            balancearHoja(hoja);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::eliminarGeneroDeHoja: " << e.what() << std::endl;
        throw;
    }
}

void ArbolBPlus::balancearHoja(NodoHoja* hoja) {
    try {
        if (!hoja) throw std::invalid_argument("Hoja nula en balancearHoja");

        // Si es la raíz y tiene al menos 1 entrada, está bien
        if (hoja == raiz) {
            if (hoja->numClaves >= 1) return; // Raíz puede tener mínimo 1
            return;
        }

        NodoInterno* padre = buscarPadre(raiz, hoja);
        if (!padre) return;

        // Encontrar la posición de la hoja en el padre
        int posEnPadre = -1;
        for (int i = 0; i <= padre->numClaves; i++) {
            if (padre->hijos[i] == hoja) {
                posEnPadre = i;
                break;
            }
        }
        if (posEnPadre == -1) return;

        // Verificar hoja hermana izquierda (si existe)
        if (posEnPadre > 0) {
            NodoHoja* hermanaIzq = (NodoHoja*)padre->hijos[posEnPadre - 1];
            if (hermanaIzq && hermanaIzq->numClaves > T_BPLUS - 1) {
                redistribuirHojas(hermanaIzq, hoja, padre, posEnPadre - 1);
                return;
            }
        }

        // Verificar hoja hermana derecha (si existe)
        if (posEnPadre < padre->numClaves) {
            NodoHoja* hermanaDer = (NodoHoja*)padre->hijos[posEnPadre + 1];
            if (hermanaDer && hermanaDer->numClaves > 1) {
                redistribuirHojas(hoja, hermanaDer, padre, posEnPadre);
                return;
            }
        }

        // Si ninguna hermana puede prestar, hacer fusión
        if (posEnPadre > 0) {
            // Fusionar con hermana izquierda
            NodoHoja* hermanaIzq = (NodoHoja*)padre->hijos[posEnPadre - 1];
            if (hermanaIzq) {
                fusionarHojas(hermanaIzq, hoja, padre, posEnPadre - 1);
            }
        } else if (posEnPadre < padre->numClaves) {
            // Fusionar con hermana derecha
            NodoHoja* hermanaDer = (NodoHoja*)padre->hijos[posEnPadre + 1];
            if (hermanaDer) {
                fusionarHojas(hoja, hermanaDer, padre, posEnPadre);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::balancearHoja: " << e.what() << std::endl;
        throw;
    }
}

void ArbolBPlus::redistribuirHojas(NodoHoja* hojaIzq, NodoHoja* hojaDer, NodoInterno* padre, int posClavePadre) {
    try {
        if (!hojaIzq || !hojaDer || !padre) {
            throw std::invalid_argument("Parámetros nulos en redistribuirHojas");
        }
        if (posClavePadre < 0 || posClavePadre >= padre->numClaves) {
            throw std::out_of_range("Posición de clave padre inválida en redistribuirHojas");
        }

        // Calcular total de entradas
        int totalEntradas = hojaIzq->numClaves + hojaDer->numClaves;
        int mitad = totalEntradas / 2;

        // Mover entradas de derecha a izquierda si izquierda tiene menos
        if (hojaIzq->numClaves < mitad) {
            int necesarias = mitad - hojaIzq->numClaves;

            // Mover entradas de derecha a izquierda
            for (int i = 0; i < necesarias; i++) {
                hojaIzq->entradas[hojaIzq->numClaves] = std::move(hojaDer->entradas[i]);
                hojaIzq->numClaves++;
            }

            // Desplazar entradas en hoja derecha
            for (int i = 0; i < hojaDer->numClaves - necesarias; i++) {
                hojaDer->entradas[i] = std::move(hojaDer->entradas[i + necesarias]);
            }
            hojaDer->numClaves -= necesarias;

        } else { // Mover de izquierda a derecha
            int sobrantes = hojaIzq->numClaves - mitad;

            // Hacer espacio en hoja derecha
            for (int i = hojaDer->numClaves - 1; i >= 0; i--) {
                hojaDer->entradas[i + sobrantes] = std::move(hojaDer->entradas[i]);
            }

            // Mover entradas de izquierda a derecha
            for (int i = 0; i < sobrantes; i++) {
                hojaDer->entradas[i] = std::move(hojaIzq->entradas[hojaIzq->numClaves - sobrantes + i]);
            }
            hojaDer->numClaves += sobrantes;
            hojaIzq->numClaves -= sobrantes;
        }

        // Actualizar clave en el padre
        if (hojaDer->numClaves > 0) {
            padre->claves[posClavePadre] = hojaDer->entradas[0].genero;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::redistribuirHojas: " << e.what() << std::endl;
        throw;
    }
}

void ArbolBPlus::fusionarHojas(NodoHoja* hojaIzq, NodoHoja* hojaDer, NodoInterno* padre, int posClavePadre) {
    try {
        if (!hojaIzq || !hojaDer || !padre) {
            throw std::invalid_argument("Parámetros nulos en fusionarHojas");
        }
        if (posClavePadre < 0 || posClavePadre >= padre->numClaves) {
            throw std::out_of_range("Posición de clave padre inválida en fusionarHojas");
        }

        // Mover todas las entradas de hojaDer a hojaIzq (copia completa)
        for (int i = 0; i < hojaDer->numClaves; i++) {
            hojaIzq->entradas[hojaIzq->numClaves] = std::move(hojaDer->entradas[i]);
            hojaIzq->numClaves++;
        }

        // Actualizar enlaces entre hojas
        hojaIzq->siguiente = hojaDer->siguiente;
        if (hojaDer->siguiente) {
            hojaDer->siguiente->anterior = hojaIzq;
        }

        delete hojaDer; // destructor liberará entradas correctamente

        // Eliminar clave del padre y ajustar hijos
        for (int i = posClavePadre; i < padre->numClaves - 1; i++) {
            padre->claves[i] = padre->claves[i + 1];
            padre->hijos[i + 1] = padre->hijos[i + 2];
        }
        padre->numClaves--;

        // Evitar puntero colgante: limpiar la última referencia a hijo
        padre->hijos[padre->numClaves + 1] = nullptr;

        // Verificar si el padre necesita balanceo después de la fusión
        if (padre->numClaves < 1 && padre != raiz) {
            balancearInterno(padre);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::fusionarHojas: " << e.what() << std::endl;
        throw;
    }
}

void ArbolBPlus::balancearInterno(NodoInterno* interno) {
    try {
        if (!interno) throw std::invalid_argument("Nodo interno nulo en balancearInterno");

        // Si es la raíz y tiene al menos 1 clave, está bien
        if (interno == raiz) {
            if (interno->numClaves >= 1) return;

            // Si la raíz queda sin claves pero tiene un hijo, convertir ese hijo en raíz
            if (interno->numClaves == 0 && interno->hijos[0]) {
                NodoBPlus* nuevaRaiz = interno->hijos[0];
                delete interno;
                raiz = nuevaRaiz;

                // Si la nueva raíz es hoja, actualizar primeraHoja
                if (nuevaRaiz->esHoja) {
                    primeraHoja = (NodoHoja*)nuevaRaiz;
                }
            }
            return;
        }

        NodoInterno* padre = buscarPadre(raiz, interno);
        if (!padre) return;

        // Encontrar la posición del interno en el padre
        int posEnPadre = -1;
        for (int i = 0; i <= padre->numClaves; i++) {
            if (padre->hijos[i] == interno) {
                posEnPadre = i;
                break;
            }
        }
        if (posEnPadre == -1) return;

        // Verificar hermano izquierdo (si existe)
        if (posEnPadre > 0) {
            NodoInterno* hermanoIzq = (NodoInterno*)padre->hijos[posEnPadre - 1];
            if (hermanoIzq && hermanoIzq->numClaves > T_BPLUS - 1) {
                redistribuirInternos(hermanoIzq, interno, padre, posEnPadre - 1);
                return;
            }
        }

        // Verificar hermano derecho (si existe)
        if (posEnPadre < padre->numClaves) {
            NodoInterno* hermanoDer = (NodoInterno*)padre->hijos[posEnPadre + 1];
            if (hermanoDer && hermanoDer->numClaves > T_BPLUS - 1) {
                redistribuirInternos(interno, hermanoDer, padre, posEnPadre);
                return;
            }
        }

        // Si ningún hermano puede prestar, hacer fusión
        if (posEnPadre > 0) {
            NodoInterno* hermanoIzq = (NodoInterno*)padre->hijos[posEnPadre - 1];
            if (hermanoIzq) {
                fusionarInternos(hermanoIzq, interno, padre, posEnPadre - 1);
            }
        } else if (posEnPadre < padre->numClaves) {
            NodoInterno* hermanoDer = (NodoInterno*)padre->hijos[posEnPadre + 1];
            if (hermanoDer) {
                fusionarInternos(interno, hermanoDer, padre, posEnPadre);
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::balancearInterno: " << e.what() << std::endl;
        throw;
    }
}

void ArbolBPlus::redistribuirInternos(NodoInterno* internoIzq, NodoInterno* internoDer, NodoInterno* padre, int posClavePadre) {
    try {
        if (!internoIzq || !internoDer || !padre) {
            throw std::invalid_argument("Parámetros nulos en redistribuirInternos");
        }
        if (posClavePadre < 0 || posClavePadre >= padre->numClaves) {
            throw std::out_of_range("Posición de clave padre inválida en redistribuirInternos");
        }

        // Calcular total de claves
        int totalClaves = internoIzq->numClaves + internoDer->numClaves + 1; // +1 por la clave del padre
        int mitad = totalClaves / 2;

        if (internoIzq->numClaves < mitad) {
            // Mover de derecha a izquierda

            // Tomar clave del padre
            internoIzq->claves[internoIzq->numClaves] = padre->claves[posClavePadre];
            internoIzq->numClaves++;

            // Mover primera clave de derecho al padre
            padre->claves[posClavePadre] = internoDer->claves[0];

            // Mover primer hijo de derecho a izquierdo
            internoIzq->hijos[internoIzq->numClaves] = internoDer->hijos[0];

            // Desplazar claves e hijos en derecho
            for (int i = 0; i < internoDer->numClaves - 1; i++) {
                internoDer->claves[i] = internoDer->claves[i + 1];
                internoDer->hijos[i] = internoDer->hijos[i + 1];
            }
            internoDer->hijos[internoDer->numClaves - 1] = internoDer->hijos[internoDer->numClaves];
            internoDer->numClaves--;

        } else {
            // Mover de izquierda a derecha

            // Hacer espacio en derecho
            for (int i = internoDer->numClaves; i > 0; i--) {
                internoDer->claves[i] = internoDer->claves[i - 1];
                internoDer->hijos[i + 1] = internoDer->hijos[i];
            }
            internoDer->hijos[1] = internoDer->hijos[0];

            // Mover última clave de izquierdo al padre
            internoDer->claves[0] = padre->claves[posClavePadre];
            internoDer->numClaves++;

            // Mover última clave de izquierdo al padre
            padre->claves[posClavePadre] = internoIzq->claves[internoIzq->numClaves - 1];

            // Mover último hijo de izquierdo a derecho
            internoDer->hijos[0] = internoIzq->hijos[internoIzq->numClaves];

            internoIzq->numClaves--;
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::redistribuirInternos: " << e.what() << std::endl;
        throw;
    }
}

void ArbolBPlus::fusionarInternos(NodoInterno* internoIzq, NodoInterno* internoDer, NodoInterno* padre, int posClavePadre) {
    try {
        if (!internoIzq || !internoDer || !padre) {
            throw std::invalid_argument("Parámetros nulos en fusionarInternos");
        }
        if (posClavePadre < 0 || posClavePadre >= padre->numClaves) {
            throw std::out_of_range("Posición de clave padre inválida en fusionarInternos");
        }

        // Mover clave del padre al interno izquierdo
        internoIzq->claves[internoIzq->numClaves] = padre->claves[posClavePadre];
        internoIzq->numClaves++;

        // Mover todas las claves e hijos del derecho al izquierdo
        for (int i = 0; i < internoDer->numClaves; i++) {
            internoIzq->claves[internoIzq->numClaves] = internoDer->claves[i];
            internoIzq->hijos[internoIzq->numClaves] = internoDer->hijos[i];
            internoIzq->numClaves++;
        }
        internoIzq->hijos[internoIzq->numClaves] = internoDer->hijos[internoDer->numClaves];

        delete internoDer;

        // Eliminar clave del padre y ajustar hijos
        for (int i = posClavePadre; i < padre->numClaves - 1; i++) {
            padre->claves[i] = padre->claves[i + 1];
            padre->hijos[i + 1] = padre->hijos[i + 2];
        }
        padre->numClaves--;

        // Evitar puntero colgante
        padre->hijos[padre->numClaves + 1] = nullptr;

        // Verificar si el padre necesita balanceo después de la fusión
        if (padre->numClaves < T_BPLUS - 1 && padre != raiz) {
            balancearInterno(padre);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error en ArbolBPlus::fusionarInternos: " << e.what() << std::endl;
        throw;
    }
}