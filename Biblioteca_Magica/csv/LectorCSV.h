#ifndef BIBLIOTECA_MAGICA_LECTOCSV_H
#define BIBLIOTECA_MAGICA_LECTOCSV_H

#include  <string>
#include "../AVL/ArbolAVL.h"

class LectorCSV {
private:
    std::string rutaArchivo;
    ArbolAVL& arbol;
    int contarCampos(const std::string& linea, char delim);
    bool tiene5Campos(const std::string& linea, char delim);
    static std::string limpiarCampo(const std::string& campo);
    static bool tieneComillasValidas(const std::string& campo);
public:
    LectorCSV(const std::string& ruta, ArbolAVL& arbolDestino);
    void procesarArchivo();
};

#endif //BIBLIOTECA_MAGICA_LECTOCSV_H