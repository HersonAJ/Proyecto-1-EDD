#include "LectorCSV.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include "../Modelos/Libro.h"
#include "../AVL/ArbolAVL.h"
#include "../include/Recorridos.h"

std::string* parseCSVLine(const std::string& linea, int& cantidad) {
    std::string* campos = new std::string[10];
    cantidad = 0;
    std::string campo;
    bool dentroComillas = false;

    for (size_t i = 0; i <linea.size(); i++) {
        char c = linea[i];

        if (c == '"') {
            dentroComillas = !dentroComillas;
            campo.push_back(c);
        } else if (c == ',' && !dentroComillas) {
            campos[cantidad++] = campo;
            campo.clear();
        } else {
            campo.push_back(c);
        }
    }

    campos[cantidad++] = campo;
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
LectorCSV::LectorCSV(const std::string& ruta, ArbolAVL& arbolDestino, ArbolB& arbolB)
    : rutaArchivo(ruta), arbol(arbolDestino), arbolB(arbolB) {}


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

    while (std::getline(archivo, linea)) {
        numLinea++;

        if (linea.empty()) continue;

        int cantidad = 0;
        std::string* campos = parseCSVLine(linea, cantidad);

        if (cantidad != 5) {
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

        // Crear libro y guardar en ambos árboles
        Libro* libro = new Libro(titulo, isbn, genero, fecha, autor);
        arbol.insertar(*libro);
        arbolB.insertar(libro);


        log("Línea " + std::to_string(numLinea) + " válida: " + titulo + ", " + isbn + ", " + genero + ", " + fecha + ", " + autor);
    }

    archivo.close();
    log("Lectura finalizada");
}
