#ifndef BIBLIOTECA_MAGICA_LECTOCSV_H
#define BIBLIOTECA_MAGICA_LECTOCSV_H

#include <functional>
#include  <string>
#include "../AVL/ArbolAVL.h"
#include  "../ArbolB/ArbolB.h"
#include "../include/Recorridos.h"
#include "../AVL_Auxiliar/IndiceISBN.h"
#include "../ArbolB+/ArbolBPlus.h"
#include "../Modelos/Catalogo.h"

class LectorCSV {
private:
    std::string rutaArchivo;
    ArbolAVL& arbol;
    ArbolB& arbolB;
    IndiceISBN& indiceISBN;
    ArbolBPlus& arbolBPlus;
    Catalogo& catalogoGlobal;
    std::function<void(const std::string &)> logger;
    int contarCampos(const std::string& linea, char delim);
    bool tiene5Campos(const std::string& linea, char delim);
    static std::string limpiarCampo(const std::string& campo);
    static bool tieneComillasValidas(const std::string& campo);
    void log(const std::string &msg) {
        if (logger) logger(msg);
    }
public:
    LectorCSV(ArbolAVL& arbolDestino, ArbolB& arbolB, IndiceISBN& indice, ArbolBPlus& arbolBPlus, Catalogo& catalogo);
    void setRutaArchivo(const std::string& ruta) { rutaArchivo = ruta; }
    bool agregarLibroIndividual(const std::string& titulo, const std::string& isbn, const std::string& genero, const std::string& fecha, const std::string& autor);
    bool validarISBN(const std::string &isbn);
    bool validarFecha(const std::string &fecha);
    void procesarArchivo();
    void procesarArchivo(const std::string& ruta);//sobrecarga
    void setLogger(std::function<void(const std::string&)> logFunc) {
        logger = logFunc;
    }
};

#endif //BIBLIOTECA_MAGICA_LECTOCSV_H