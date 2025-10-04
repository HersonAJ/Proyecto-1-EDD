#include "LectorCSV.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Modelos/Libro.h"
#include "../AVL/ArbolAVL.h"
#include "../include/Recorridos.h"
#include "../ArbolB/ListaLibros.h"
#include <regex>

std::vector<std::string> parseCSVLine(const std::string& linea) {
    std::vector<std::string> campos;
    std::string campo;
    bool dentroComillas = false;

    for (size_t i = 0; i < linea.size(); i++) {
        char c = linea[i];
        if (c == '"') {
            dentroComillas = !dentroComillas;
            campo.push_back(c);
        } else if (c == ',' && !dentroComillas) {
            campos.push_back(campo);
            campo.clear();
        } else {
            campo.push_back(c);
        }
    }

    campos.push_back(campo);
    return campos;
}



bool LectorCSV::tieneComillasValidas(const std::string& campoOriginal) {
    std::string campo = campoOriginal;

    // Quitar espacios iniciales y finales
    size_t start = campo.find_first_not_of(" \t\r\n");
    size_t end   = campo.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return false;
    campo = campo.substr(start, end - start + 1);

    // validar comillas
    if (campo.size() < 2) return false;
    if (campo.front() != '"' || campo.back() != '"') return false;

    // Contenido interno
    std::string interno = campo.substr(1, campo.size() - 2);
    start = interno.find_first_not_of(" \t\r\n");
    end   = interno.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return false;
    interno = interno.substr(start, end - start + 1);

    return !interno.empty();
}

std::string LectorCSV::limpiarCampo(const std::string& campoOriginal) {
    std::string campo = campoOriginal;

    // Quitar espacios iniciales y finales
    size_t start = campo.find_first_not_of(" \t\r\n");
    size_t end   = campo.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    campo = campo.substr(start, end - start + 1);

    // Quitar comillas si están al inicio y final
    if (campo.size() >= 2 && campo.front() == '"' && campo.back() == '"') {
        campo = campo.substr(1, campo.size() - 2);
    }

    // Quitar espacios otra vez
    start = campo.find_first_not_of(" \t\r\n");
    end   = campo.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    campo = campo.substr(start, end - start + 1);

    return campo;
}

// Constructor
LectorCSV::LectorCSV(ArbolAVL& arbolDestino, ArbolB& arbolB, IndiceISBN& indice, ArbolBPlus& arbolBPlus, Catalogo& catalogo)
    : rutaArchivo(""), arbol(arbolDestino), arbolB(arbolB), indiceISBN(indice), arbolBPlus(arbolBPlus), catalogoGlobal(catalogo) {}

//sobrecarga al constructor para que reciva la ruta
void LectorCSV::procesarArchivo(const std::string &ruta) {
    rutaArchivo = ruta;
    procesarArchivo();
}

void LectorCSV::procesarArchivo() {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        log("Error: no se pudo abrir el archivo: " + rutaArchivo);
        std::cerr << "Error: No se pudo abrir el archivo: " << rutaArchivo << std::endl;
        return;
    }
    log("Iniciando la lectura del archivo....");

    std::string linea;
    int numLinea = 0;
    ListaLibros librosTemporales;
    while (std::getline(archivo, linea)) {
        numLinea++;

        if (linea.empty()) continue;

        int cantidad = 0;
        auto campos = parseCSVLine(linea);

        if (campos.size() != 5) {
            log("Error en línea " + std::to_string(numLinea) + ": número incorrecto de campos. Se esperaban 5.");
            log("Contenido: " + linea);
            std::cerr << "Error en línea " << numLinea << ": número incorrecto de campos. Se esperaban 5." << std::endl;
            std::cerr << "Contenido: " << linea << std::endl;
            continue;
        }

        std::string titulo = campos[0];
        std::string isbn   = campos[1];
        std::string genero = campos[2];
        std::string fecha  = campos[3];
        std::string autor  = campos[4];

        // Validar comillas antes de limpiar
        if (!tieneComillasValidas(titulo) ||
            !tieneComillasValidas(isbn)   ||
            !tieneComillasValidas(genero) ||
            !tieneComillasValidas(fecha)  ||
            !tieneComillasValidas(autor)) {

            log("Error en línea " + std::to_string(numLinea) + ": formato inválido, faltan comillas o campo vacío.");
            log("Contenido: " + linea);

            std::cerr << "Error en línea " << numLinea << ": formato inválido, faltan comillas o campo vacío." << std::endl;
            std::cerr << "Contenido: " << linea << std::endl;
            continue;
        }

        // Limpiar campos
        titulo = limpiarCampo(titulo);
        isbn   = limpiarCampo(isbn);
        genero = limpiarCampo(genero);
        fecha  = limpiarCampo(fecha);
        autor  = limpiarCampo(autor);

        // OPCIÓN A: Validación ESTRICTA con formato ISBN específico (COMENTADA)

        if (!validarISBN(isbn)) {
            log("Error en línea " + std::to_string(numLinea) + ": ISBN con formato inválido: " + isbn);
            log("Formato esperado: xxx-xx-xxx-xxxx-x (13 dígitos)");
            continue;
        }

        /*
        // OPCIÓN B: Validación BÁSICA - solo verificar que no esté vacío (ACTIVA)
        if (isbn.empty()) {
            log("Error en línea " + std::to_string(numLinea) + ": ISBN vacío");
            continue;
        }*/

        // VALIDACIÓN DE FECHA (siempre activa)
        if (!validarFecha(fecha)) {
            log("Error en línea " + std::to_string(numLinea) + ": Fecha inválida: " + fecha);
            log("La fecha debe ser un número positivo");
            continue;
        }

        Libro* libroGlobal = new Libro(titulo, isbn, genero, fecha, autor);  // Para IndiceISBN global
        Libro* libroAVL = new Libro(titulo, isbn, genero, fecha, autor);     // Para Árbol AVL
        Libro* libroB = new Libro(titulo, isbn, genero, fecha, autor);       // Para Árbol B
        Libro* libroBPlus = new Libro(titulo, isbn, genero, fecha, autor);   // Para Árbol B+

        // Guardar la copia del B+ en la lista temporal
        librosTemporales.insertar(libroBPlus);

        std::cout << "Insertando en Árbol B - Fecha: '" << fecha << "' -> " << libroB->getFechaInt() << std::endl;
        arbolBPlus.insertarSoloGenero(genero);
        arbol.insertar(libroAVL);                    // Árbol AVL con su copia
        arbolB.insertar(libroB);                     // Árbol B con su copia
        indiceISBN.insertar(libroGlobal->getIsbn(), libroGlobal);  // Global con su copia
        catalogoGlobal.agregarLibro(new Libro(titulo, isbn, genero, fecha, autor));//lista para busqueda secuencial

        arbolB.verificarDuplicados();
        ArbolBPlus::recorrerEstructura(arbolBPlus.getRaiz());

        log("Línea " + std::to_string(numLinea) + " válida: " + titulo + ", " + isbn + ", " + genero + ", " + fecha + ", " + autor);
    }

    archivo.close();
    ListaLibros::Interador iter = librosTemporales.obtenerIterador();
    while (iter.tieneSiguiente()) {
        Libro* libroBPlus = iter.siguiente();
        arbolBPlus.insertarLibroEnGenero(libroBPlus);
    }
    log("Lectura finalizada");
}

// agregarLibroIndividual
bool LectorCSV::agregarLibroIndividual(const std::string& titulo, const std::string& isbn, const std::string& genero, const std::string& fecha, const std::string& autor) {

    // OPCIÓN A: Validación ESTRICTA con formato ISBN específico (COMENTADA)
    if (!validarISBN(isbn)) {
        log("Error: ISBN con formato inválido: " + isbn);
        log("Formato esperado: xxx-xx-xxx-xxxx-x (13 dígitos)");
        return false;
    }
    /*//OPCIÓN B: Validación BÁSICA - solo verificar que no esté vacío (ACTIVA)
    if (isbn.empty()) {
        log("Error: ISBN vacío");
        return false;
    }*/

    // VALIDACIÓN DE FECHA (siempre activa)
    if (!validarFecha(fecha)) {
        log("Error: Fecha inválida: " + fecha);
        log("La fecha debe ser un número positivo");
        return false;
    }

    // Validar que el ISBN no exista
    if (indiceISBN.buscar(isbn)) {
        log("Error: El ISBN '" + isbn + "' ya existe");
        return false;
    }

    // Validar campos no vacíos
    if (titulo.empty() || isbn.empty() || genero.empty() || fecha.empty() || autor.empty()) {
        log("Error: Todos los campos son obligatorios");
        return false;
    }

    try {
        // MISMAS 5 COPIAS que en procesarArchivo()
        Libro* libroGlobal = new Libro(titulo, isbn, genero, fecha, autor);
        Libro* libroAVL = new Libro(titulo, isbn, genero, fecha, autor);
        Libro* libroB = new Libro(titulo, isbn, genero, fecha, autor);
        Libro* libroBPlus = new Libro(titulo, isbn, genero, fecha, autor);
        Libro* libroCatalogo = new Libro(titulo, isbn, genero, fecha, autor);

        // MISMAS INSERCIONES que en procesarArchivo()
        arbol.insertar(libroAVL);
        arbolB.insertar(libroB);
        indiceISBN.insertar(isbn, libroGlobal);
        catalogoGlobal.agregarLibro(libroCatalogo);

        // MISMO PROCESO para Árbol B+
        arbolBPlus.insertarSoloGenero(genero);
        arbolBPlus.insertarLibroEnGenero(libroBPlus);

        log("Libro agregado manualmente: " + titulo + " - ISBN: " + isbn);
        return true;

    } catch (const std::exception& e) {
        log("Error excepción al agregar libro: " + std::string(e.what()));
        return false;
    }
}

bool LectorCSV::validarISBN(const std::string& isbn) {
    // Formato: xxx-xx-xxx-xxxx-x (ISBN-13)
    // Ejemplos válidos:
    // - 978-84-376-0494-7
    std::regex patronISBN(R"(^\d{3}-\d{1,5}-\d{1,7}-\d{1,7}-\d{1}$)");
    return std::regex_match(isbn, patronISBN);
}

bool LectorCSV::validarFecha(const std::string& fecha) {
    // Solo verificar que sea número positivo
    try {
        int año = std::stoi(fecha);
        return año >= 0;  //Cualquier número positivo
    } catch (const std::exception& e) {
        return false;
    }
}