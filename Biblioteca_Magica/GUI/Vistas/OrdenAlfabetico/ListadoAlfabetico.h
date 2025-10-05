#ifndef BIBLIOTECA_MAGICA_LISTADOALFABETICO_H
#define BIBLIOTECA_MAGICA_LISTADOALFABETICO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include "../../../AVL/ArbolAVL.h"
#include "../../../ArbolB/ListaLibros.h"

class ListadoAlfabetico : public QWidget {
    Q_OBJECT

    public:
    explicit ListadoAlfabetico(ArbolAVL* arbolTitulos, QWidget* parent = nullptr);
    void cargarDatosEnTabla();

    private:
    ArbolAVL* arbolTitulos;
    QTableWidget* tablaLibros;

    void setupUI();
    void configurarTabla();
};

#endif //BIBLIOTECA_MAGICA_LISTADOALFABETICO_H