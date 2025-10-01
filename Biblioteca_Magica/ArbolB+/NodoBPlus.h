#ifndef BIBLIOTECA_MAGICA_NODOBPLUS_H
#define BIBLIOTECA_MAGICA_NODOBPLUS_H

const int T_BPLUS = 3;

// Clase base para nodos B+ (abstracta, interfaz mínima)
class NodoBPlus {
public:
    bool esHoja;     // true si es hoja
    int numClaves;   // número actual de claves

    explicit NodoBPlus(bool hoja) : esHoja(hoja), numClaves(0) {}
    virtual ~NodoBPlus() = default;

};

#endif // BIBLIOTECA_MAGICA_NODOBPLUS_H