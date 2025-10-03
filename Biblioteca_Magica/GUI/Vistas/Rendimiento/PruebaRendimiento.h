#ifndef BIBLIOTECA_MAGICA_PRUEBARENDIMIENTO_H
#define BIBLIOTECA_MAGICA_PRUEBARENDIMIENTO_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QLabel>
#include <chrono>
#include "../../../Modelos/Libro.h"
#include "../../../AVL/ArbolAVL.h"
#include "../../../AVL_Auxiliar/IndiceISBN.h"
#include "../../../Modelos/Catalogo.h"

class PruebaRendimiento : public QWidget {
    Q_OBJECT

public:
    explicit PruebaRendimiento(ArbolAVL* arbolTitulos,
                              IndiceISBN* indiceISBN,
                              Catalogo* catalogo,
                              QWidget* parent = nullptr);

    void ejecutarComparacionTitulo(const std::string& titulo);
    void ejecutarComparacionISBN(const std::string& isbn);

private slots:
    void onCompararClicked();
    void onTipoBusquedaChanged(int index);

private:
    ArbolAVL* arbolTitulos;
    IndiceISBN* indiceISBN;
    Catalogo* catalogo;

    // UI Elementos
    QComboBox* comboBusqueda;
    QLineEdit* editBusqueda;
    QPushButton* btnComparar;
    QTextEdit* textResultados;
    QLabel* labelInstruccion;

    void setupUI();
    void appendResultado(const QString& texto, const QString& color = "black");

    // Mediciones de tiempo
    template<typename Func>
    long long medirTiempo(Func funcion);
};


#endif //BIBLIOTECA_MAGICA_PRUEBARENDIMIENTO_H