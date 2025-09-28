#ifndef BIBLIOTECA_MAGICA_LECTOCSV_H
#define BIBLIOTECA_MAGICA_LECTOCSV_H

#include <functional>
#include  <string>
#include "../AVL/ArbolAVL.h"
#include  "../ArbolB/ArbolB.h"
#include "../include/Recorridos.h"
#include "../AVL_Auxiliar/IndiceISBN.h"

class LectorCSV {
private:
    std::string rutaArchivo;
    ArbolAVL& arbol;
    ArbolB& arbolB;
    IndiceISBN& indiceISBN;
    std::function<void(const std::string &)> logger;
    int contarCampos(const std::string& linea, char delim);
    bool tiene5Campos(const std::string& linea, char delim);
    static std::string limpiarCampo(const std::string& campo);
    static bool tieneComillasValidas(const std::string& campo);
    void log(const std::string &msg) {
        if (logger) logger(msg);
    }
public:
    LectorCSV(const std::string& ruta, ArbolAVL& arbolDestino, ArbolB& arbolB, IndiceISBN& indice);
    void procesarArchivo();
    void setLogger(std::function<void(const std::string&)> logFunc) {
        logger = logFunc;
    }
};

#endif //BIBLIOTECA_MAGICA_LECTOCSV_H