#include "ArbolBPlus.h"
#include <iostream>
const int MAX_KEYS = 2 * T_BPLUS - 1;

// Constructor y destructor
ArbolBPlus::ArbolBPlus() {
    raiz = new NodoHoja();   // al inicio la raíz es una hoja
    primeraHoja = (NodoHoja*)raiz;
}

ArbolBPlus::~ArbolBPlus() {
    liberarRecursivo(raiz);
}

void ArbolBPlus::liberarRecursivo(NodoBPlus* nodo) {
    if (!nodo) return;

    if (!nodo->esHoja) {
        NodoInterno* interno = (NodoInterno*)nodo;
        for (int i = 0; i <= interno->numClaves; i++) {
            liberarRecursivo(interno->hijos[i]);
        }
        delete interno;
    } else {
        NodoHoja* hoja = (NodoHoja*)nodo;
        delete hoja;
    }
}

void ArbolBPlus::insertarSoloGenero(const std::string &genero) {
    if (genero.empty()) return;

    NodoHoja* hoja = buscarHoja(genero);
    if (!hoja) return;

    //buscar si el genero ya existe
    int pos = 0;
    while (pos < hoja->numClaves && genero > hoja->entradas[pos].genero) {
        pos++;
    }

    bool generoExiste = (pos < hoja->numClaves && hoja->entradas[pos].genero == genero);

    // Si no existe y la hoja está llena, partirla ANTES de insertar
    if (!generoExiste && hoja->numClaves >= MAX_KEYS) {
        dividirHoja(hoja);
        hoja = buscarHoja(genero); // reobtener la hoja correcta después de dividir
        // recalcular pos y existencia
        pos = 0;
        while (pos < hoja->numClaves && genero > hoja->entradas[pos].genero) pos++;
        generoExiste = (pos < hoja->numClaves && hoja->entradas[pos].genero == genero);
    }

    if (generoExiste) {
        // ya existe, nada más (los IndiceISBN quedan vacíos hasta que se carguen los libros)
        return;
    }

    // Insertar nueva entrada: crear EntradaGenero y moverla al hueco
    if (hoja->numClaves >= MAX_KEYS) {
        // defensivo: si aún está llena, abortamos para no corromper memoria
        std::cerr << "[ERROR] insertarSoloGenero: hoja aún llena tras split\n";
        return;
    }

    // Desplazar elementos a la derecha (desde numClaves-1 hasta pos)
    for (int j = hoja->numClaves - 1; j >= pos; --j) {
        hoja->entradas[j + 1] = std::move(hoja->entradas[j]); // mover, no copiar
    }

    // Construir la nueva entrada con genero y sin IndiceISBN (nullptr)
    NodoHoja::EntradaGenero nueva;
    nueva.genero = genero;
    // no se crean indiceISBN todavía (la idea de pre-registrar géneros)

    // Mover la nueva entrada al hueco
    hoja->entradas[pos] = std::move(nueva);
    hoja->numClaves++;
}


// Buscar hoja adecuada
NodoHoja* ArbolBPlus::buscarHoja(const std::string& genero) const {
    NodoBPlus* nodo = raiz;
    int nivel = 0;

    while (!nodo->esHoja) {
        NodoInterno* interno = (NodoInterno*)nodo;
        std::cout << "[DEBUG] buscarHoja - Nivel " << nivel << ", claves: ";
        for (int i = 0; i < interno->numClaves; i++) {
            std::cout << "[" << interno->claves[i] << "] ";
        }
        std::cout << std::endl;

        int i = 0;
        while (i < interno->numClaves && genero >= interno->claves[i]) {
            i++;
        }
        nodo = interno->hijos[i];
        nivel++;
    }

    NodoHoja* hoja = (NodoHoja*)nodo;
    std::cout << "[DEBUG] buscarHoja - Hoja final: ";
    for (int i = 0; i < hoja->numClaves; i++) {
        std::cout << "[" << hoja->entradas[i].genero << "] ";
    }
    std::cout << std::endl;

    return hoja;
}

// Insertar en hoja
void ArbolBPlus::insertarEnHoja(NodoHoja* hoja, Libro* libro) {
    if (!hoja || !libro) return;
    std::string genero = libro->getGenero();

    int i = 0;
    while (i < hoja->numClaves && genero > hoja->entradas[i].genero) {
        i++;
    }

    // Caso: género ya existe
    if (i < hoja->numClaves && hoja->entradas[i].genero == genero) {
        if (!hoja->entradas[i].indiceISBN) {
            hoja->entradas[i].indiceISBN = std::make_unique<IndiceISBN>();
        }
        hoja->entradas[i].indiceISBN->insertar(libro->getIsbn(), libro);
        return;
    }

    // Nuevo género: si la hoja está llena, partirla antes de insertar
    if (hoja->numClaves >= MAX_KEYS) {
        dividirHoja(hoja);
        hoja = buscarHoja(genero); // re-obtener la hoja correcta después de la division
        // recalcular posición
        i = 0;
        while (i < hoja->numClaves && genero > hoja->entradas[i].genero) i++;
        if (i < hoja->numClaves && hoja->entradas[i].genero == genero) {
            if (!hoja->entradas[i].indiceISBN) hoja->entradas[i].indiceISBN = std::make_unique<IndiceISBN>();
            hoja->entradas[i].indiceISBN->insertar(libro->getIsbn(), libro);
            return;
        }
    }

    // Desplazar con move (j desde numClaves-1 hasta i)
    for (int j = hoja->numClaves - 1; j >= i; --j) {
        hoja->entradas[j + 1] = std::move(hoja->entradas[j]);
    }

    // Crear la nueva entrada en i
    hoja->entradas[i].genero = genero;
    if (!hoja->entradas[i].indiceISBN) {
        hoja->entradas[i].indiceISBN = std::make_unique<IndiceISBN>();
    }
    hoja->entradas[i].indiceISBN->insertar(libro->getIsbn(), libro);
    hoja->numClaves++;
}

// Dividir hoja
void ArbolBPlus::dividirHoja(NodoHoja* hoja) {
    if (!hoja) return;
    int oldNum = hoja->numClaves;
    if (oldNum <= 0) return;

    // Elegir punto de corte:
    int mitad = oldNum / 2;
    if (mitad <= 0) mitad = 1;
    if (mitad >= oldNum) mitad = oldNum / 2;

    NodoHoja* nuevaHoja = new NodoHoja();
    nuevaHoja->numClaves = oldNum - mitad;

    // Mover la mitad superior hacia la nueva hoja
    for (int i = 0; i < nuevaHoja->numClaves; ++i) {
        int srcIdx = mitad + i;
        // seguridad contra índices
        if (srcIdx >= oldNum || i >= MAX_KEYS) break;
        nuevaHoja->entradas[i] = std::move(hoja->entradas[srcIdx]);
    }

    // Limpiar las entradas moved-from en la hoja original
    for (int i = mitad; i < oldNum; ++i) {
        hoja->entradas[i].genero.clear();
        hoja->entradas[i].indiceISBN.reset(); // liberar puntero si quedó
    }

    hoja->numClaves = mitad;

    // Ajustar enlaces entre hojas
    nuevaHoja->siguiente = hoja->siguiente;
    if (nuevaHoja->siguiente) {
        nuevaHoja->siguiente->anterior = nuevaHoja;
    }
    hoja->siguiente = nuevaHoja;
    nuevaHoja->anterior = hoja;

    // Subir clave al padre
    std::string claveSubir = (nuevaHoja->numClaves > 0) ? nuevaHoja->entradas[0].genero : std::string();

    if (hoja == raiz) {
        NodoInterno* nuevaRaiz = new NodoInterno();
        nuevaRaiz->claves[0] = claveSubir;
        nuevaRaiz->hijos[0] = hoja;
        nuevaRaiz->hijos[1] = nuevaHoja;
        nuevaRaiz->numClaves = 1;
        raiz = nuevaRaiz;
        return;
    }

    NodoInterno* padre = buscarPadre(raiz, hoja);
    if (!padre) {
        std::cerr << "Error: No se encontro padre de la hoja" << std::endl;
        return;
    }

    int i = 0;
    while (i < padre->numClaves && claveSubir > padre->claves[i]) i++;

    // Desplazar claves/hijos en padre (de forma segura)
    for (int j = padre->numClaves - 1; j >= i; --j) {
        padre->claves[j + 1] = padre->claves[j];
        padre->hijos[j + 2] = padre->hijos[j + 1];
    }

    padre->claves[i] = claveSubir;
    padre->hijos[i + 1] = nuevaHoja;
    padre->numClaves++;

    if (padre->numClaves >= MAX_KEYS) {
        dividirInterno(padre);
    }
}

// Dividir interno
void ArbolBPlus::dividirInterno(NodoInterno* interno) {
    int mitad = interno->numClaves / 2;

    NodoInterno* nuevoInterno = new NodoInterno();

    // Número de claves que van al nuevo nodo (lado derecho)
    int numDerecha = interno->numClaves - mitad - 1;
    if (numDerecha < 0) numDerecha = 0;
    if (numDerecha > (2 * T_BPLUS - 1)) numDerecha = 2 * T_BPLUS - 1; // seguridad extra
    nuevoInterno->numClaves = numDerecha;

    // Copiar claves e hijos al nuevo interno
    for (int i = 0; i < nuevoInterno->numClaves; i++) {
        nuevoInterno->claves[i] = interno->claves[mitad + 1 + i];
    }
    for (int i = 0; i <= nuevoInterno->numClaves; i++) {
        nuevoInterno->hijos[i] = interno->hijos[mitad + 1 + i];
    }

    // La clave que sube al padre
    std::string claveSubir = interno->claves[mitad];

    // Reducir el número de claves del interno original
    interno->numClaves = mitad;

    if (interno == raiz) {
        // Crear nueva raíz
        NodoInterno* nuevaRaiz = new NodoInterno();
        nuevaRaiz->claves[0] = claveSubir;
        nuevaRaiz->hijos[0] = interno;
        nuevaRaiz->hijos[1] = nuevoInterno;
        nuevaRaiz->numClaves = 1;
        raiz = nuevaRaiz;
    } else {
        NodoInterno* padre = buscarPadre(raiz, interno);

        if (!padre) {
            std::cout << "Error: No se encontró padre del nodo interno" << std::endl;
            return;
        }

        int i = 0;
        while (i < padre->numClaves && claveSubir > padre->claves[i]) {
            i++;
        }

        // Desplazar claves/hijos en el padre para hacer espacio
        for (int j = padre->numClaves - 1; j >= i; --j) {
            padre->claves[j + 1] = padre->claves[j];
            padre->hijos[j + 2] = padre->hijos[j + 1];
        }

        // Insertar la clave y el nuevo hijo
        padre->claves[i] = claveSubir;
        padre->hijos[i + 1] = nuevoInterno;
        padre->numClaves++;

        // Si el padre ahora está lleno, dividirlo
        if (padre->numClaves >= (2 * T_BPLUS - 1)) {
            dividirInterno(padre);
        }
    }
}

// Buscar padre (auxiliar)
NodoInterno* ArbolBPlus::buscarPadre(NodoBPlus* actual, NodoBPlus* hijo) const {
    if (!actual || actual->esHoja) return nullptr;

    NodoInterno* interno = (NodoInterno*)actual;
    for (int i = 0; i <= interno->numClaves; i++) {
        if (interno->hijos[i] == hijo) {
            return interno;
        }
        NodoInterno* posible = buscarPadre(interno->hijos[i], hijo);
        if (posible) return posible;
    }
    return nullptr;
}

void ArbolBPlus::insertarLibroEnGenero(Libro *libro) {
    if (!libro) return;
    std::string genero = libro->getGenero();
    NodoHoja* hoja = buscarHoja(genero);
    if (!hoja) return;

    // Mostrar todas las entradas de la hoja
    for (int i = 0; i < hoja->numClaves; i++) {
    }

    //buscar la entrada exacta al genero
    for (int i = 0; i < hoja->numClaves; i++) {
        if (hoja->entradas[i].genero == genero) {
            if (!hoja->entradas[i].indiceISBN) {
                hoja->entradas[i].indiceISBN = std::make_unique<IndiceISBN>();
            }
            hoja->entradas[i].indiceISBN->insertar(libro->getIsbn(), libro);
            return;
        }
    }
    insertarEnHoja(hoja, libro);
}