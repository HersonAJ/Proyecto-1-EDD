#include "LectorCSV.h"
#include <iostream>
#include <fstream>
#include <sstream>

LectorCSV::LectorCSV(const std::string& ruta) : rutaArchivo(ruta) {}

int LectorCSV::contarCampos(const std::string& linea, char delim) {
    int count = 0;
    std::stringstream ss(linea);
    std::string token;
    while (std::getline(ss, token, delim)) {
        count++;
    }
    return count;
}

bool LectorCSV::tiene5Campos(const std::string& linea, char delim) {
    return contarCampos(linea, delim) == 5;
}

void LectorCSV::procesarArchivo() {
    std::ifstream archivo(rutaArchivo);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo: " << rutaArchivo << std::endl;
        return;
    }

    std::string linea;
    int numLinea = 0;

    while (std::getline(archivo, linea)) {
        numLinea++;

        // Saltar líneas vacías
        if (linea.empty()) continue;

        if (!tiene5Campos(linea, ',')) {
            std::cerr << "Error en línea " << numLinea << ": número incorrecto de campos. Se esperaban 5." << std::endl;
            std::cerr << "Contenido: " << linea << std::endl;
            continue;
        }

        std::stringstream ss(linea);
        std::string titulo, isbn, genero, fecha, autor;
        std::getline(ss, titulo, ',');
        std::getline(ss, isbn, ',');
        std::getline(ss, genero, ',');
        std::getline(ss, fecha, ',');
        std::getline(ss, autor, ',');

        //crear el libro e insertar en el arbol


        // Si llegamos aquí, la línea es válida
        std::cout << "Línea " << numLinea << " válida: " << linea << std::endl;

    }

    archivo.close();
}