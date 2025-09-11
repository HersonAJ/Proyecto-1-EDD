#ifndef BIBLIOTECA_MAGICA_LECTOCSV_H
#define BIBLIOTECA_MAGICA_LECTOCSV_H

#include  <string>

class LectorCSV {
private:
    std::string rutaArchivo;
    int contarCampos(const std::string& linea, char delim);
    bool tiene5Campos(const std::string& linea, char delim);

public:
    LectorCSV(const std::string& ruta);
    void procesarArchivo();
};

#endif //BIBLIOTECA_MAGICA_LECTOCSV_H