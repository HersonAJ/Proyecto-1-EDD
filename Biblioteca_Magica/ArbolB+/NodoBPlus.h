#ifndef BIBLIOTECA_MAGICA_NODOBPLUS_H
#define BIBLIOTECA_MAGICA_NODOBPLUS_H

#include <string>

const int T_BPLUS = 3;

class NodoBPlus {
    public:
    int numClaves;
    bool esHoja;

    NodoBPlus(bool esHoja);
    virtual ~NodoBPlus() = default;

    //metodos que comparten ambos nodos
    virtual bool estaLeno() const = 0;
    virtual bool estaVacio() const = 0;
    virtual int encontrarPosicion(const std::string& genero) const = 0;
};

#endif //BIBLIOTECA_MAGICA_NODOBPLUS_H