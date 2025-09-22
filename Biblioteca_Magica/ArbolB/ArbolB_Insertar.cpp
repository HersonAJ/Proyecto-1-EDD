#include  "ArbolB.h"
#include  <iostream>

void ArbolB::insertar(Libro *libro) {
    // Validación de entrada
    if (libro == nullptr) {
        std::cerr << "Error: Intento de insertar libro nulo" << std::endl;
        return;
    }

    if (raiz == nullptr) {
        // Si el árbol está vacío, se crea la raíz
        raiz = new NodoB(true);
        if (raiz == nullptr) {
            std::cerr << "Error: No se pudo asignar memoria para la raíz" << std::endl;
            return;
        }
        raiz->claves[0] = libro;
        raiz->numClaves = 1;
    } else {
        // Si la raíz está llena se tiene que dividir
        if (raiz->numClaves == 2 * T - 1) {
            NodoB* nuevaRaiz = new NodoB(false);
            if (nuevaRaiz == nullptr) {
                std::cerr << "Error: No se pudo asignar memoria para nueva raíz" << std::endl;
                return;
            }

            nuevaRaiz->hijos[0] = raiz;
            // Dividir la raíz
            dividirHijo(nuevaRaiz, 0, raiz);

            // Decidir en qué hijo insertar
            int i = 0;
            if (libro->compararPorFecha(*nuevaRaiz->claves[0]) > 0) {
                i++;
            }
            insertarNoLleno(nuevaRaiz->hijos[i], libro);
            raiz = nuevaRaiz;
        } else {
            insertarNoLleno(raiz, libro);
        }
    }
}

//metodo para insertar en un nodo que no esta lleno
void ArbolB::insertarNoLleno(NodoB *nodo, Libro *libro) {
    // Validación de entrada
    if (nodo == nullptr || libro == nullptr) {
        std::cerr << "Error: Nodo o libro nulo en insertarNoLleno" << std::endl;
        return;
    }

    int i = nodo->numClaves - 1;

    if (nodo->esHoja) {
        // Mover claves hacia la derecha hasta encontrar la posición correcta
        while (i >= 0 && libro->compararPorFecha(*nodo->claves[i]) < 0) {
            nodo->claves[i + 1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i + 1] = libro;
        nodo->numClaves++;
    } else {
        // Buscar el hijo adecuado
        while (i >= 0 && libro->compararPorFecha(*nodo->claves[i]) < 0) {
            i--;
        }
        i++;

        // Si el hijo está lleno se divide
        if (nodo->hijos[i] != nullptr && nodo->hijos[i]->numClaves == 2 * T - 1) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            // Después de dividir, revisar si debemos ir al hijo derecho
            if (libro->compararPorFecha(*nodo->claves[i]) > 0) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], libro);
    }
}


//dividir un hijo lleno en dos nodos
/*void ArbolB::dividirHijo(NodoB *padre, int i, NodoB *hijo) {
    // Validaciones de entrada
    if (padre == nullptr || hijo == nullptr) {
        std::cerr << "Error: Padre o hijo nulo en dividirHijo" << std::endl;
        return;
    }

    if (i < 0 || i > padre->numClaves) {
        std::cerr << "Error: Índice inválido en dividirHijo" << std::endl;
        return;
    }

    NodoB* nuevo = new NodoB(hijo->esHoja);
    if (nuevo == nullptr) {
        std::cerr << "Error: No se pudo asignar memoria para nuevo nodo" << std::endl;
        return;
    }

    nuevo->numClaves = T - 1;

    // Copiar las últimas T-1 claves al nuevo nodo
    for (int j = 0; j < T - 1; j++) {
        nuevo->claves[j] = hijo->claves[j + T];
        hijo->claves[j + T] = nullptr; // Limpiar referencia en el hijo
    }

    // Copiar los hijos si no es hoja
    if (!hijo->esHoja) {
        for (int j = 0; j < T; j++) {
            nuevo->hijos[j] = hijo->hijos[j + T];
            hijo->hijos[j + T] = nullptr; // Limpiar referencia en el hijo
        }
    }

    hijo->numClaves = T - 1;

    // Mover los hijos del padre para hacer espacio
    for (int j = padre->numClaves; j >= i + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[i + 1] = nuevo;

    // Mover las claves del padre
    for (int j = padre->numClaves - 1; j >= i; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }

    // La clave media sube al padre
    padre->claves[i] = hijo->claves[T - 1];
    hijo->claves[T - 1] = nullptr; // Limpiar la clave que subió
    padre->numClaves++;
}

*/
// Dividir un hijo lleno en dos nodos
void ArbolB::dividirHijo(NodoB *padre, int i, NodoB *hijo) {
    // Validaciones de entrada
    if (padre == nullptr || hijo == nullptr) {
        std::cerr << "Error: Padre o hijo nulo en dividirHijo" << std::endl;
        return;
    }

    if (i < 0 || i > padre->numClaves) {
        std::cerr << "Error: Índice inválido en dividirHijo" << std::endl;
        return;
    }

    NodoB* nuevo = new NodoB(hijo->esHoja);
    if (nuevo == nullptr) {
        std::cerr << "Error: No se pudo asignar memoria para nuevo nodo" << std::endl;
        return;
    }

    // Copiar las últimas T-1 claves de hijo → nuevo
    nuevo->numClaves = T - 1;
    for (int j = 0; j < T - 1; j++) {
        nuevo->claves[j] = hijo->claves[j + T];
        hijo->claves[j + T] = nullptr; // limpiar referencia
    }

    // Copiar los hijos si no es hoja
    if (!hijo->esHoja) {
        for (int j = 0; j < T; j++) {
            nuevo->hijos[j] = hijo->hijos[j + T];
            hijo->hijos[j + T] = nullptr; // limpiar referencia
        }
    }

    // Reducir el número de claves en hijo (se queda con T-1)
    hijo->numClaves = T - 1;

    // Desplazar los hijos del padre a la derecha para insertar el nuevo
    for (int j = padre->numClaves; j >= i + 1; j--) {
        padre->hijos[j + 1] = padre->hijos[j];
    }
    padre->hijos[i + 1] = nuevo;

    // Desplazar las claves del padre a la derecha para insertar la mediana
    for (int j = padre->numClaves - 1; j >= i; j--) {
        padre->claves[j + 1] = padre->claves[j];
    }

    // Subir la clave mediana desde hijo al padre
    padre->claves[i] = hijo->claves[T - 1];
    hijo->claves[T - 1] = nullptr; // limpiar referencia de la mediana
    padre->numClaves++;
}
