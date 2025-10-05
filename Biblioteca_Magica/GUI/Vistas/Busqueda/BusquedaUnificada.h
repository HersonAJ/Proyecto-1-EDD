#ifndef BIBLIOTECA_MAGICA_BUSQUEDAUNIFICADA_H
#define BIBLIOTECA_MAGICA_BUSQUEDAUNIFICADA_H


#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include "../../../AVL/ArbolAVL.h"
#include "../../../AVL_Auxiliar/IndiceISBN.h"
#include "../../../ArbolB/ArbolB.h"
#include "../../../ArbolB+/ArbolBPlus.h"
#include "../../../Modelos/Catalogo.h"
#include "../../../ArbolB/ListaLibros.h"
#include "../../../AVL/ListaEncontrados.h"

class BusquedaUnificada : public QWidget {
    Q_OBJECT

public:
    explicit BusquedaUnificada(ArbolAVL* arbolTitulos,
                                  IndiceISBN* indiceISBN,
                                  ArbolB* arbolB,
                                  ArbolBPlus* arbolBPlus,
                                  std::function<void(const std::string&, const QString&)> logCallBack,
                                  QWidget* parent = nullptr);

private slots:
    void onTipoBusquedaCambiado(int index);
    void onBuscarClicked();

private:
    // Estructuras de datos
    ArbolAVL* arbolTitulos;
    IndiceISBN* indiceISBN;
    ArbolB* arbolB;
    ArbolBPlus* arbolBPlus;

    //agreando mensajes al log
    std::function<void(const std::string&, const QString&)> appendLog;

    // Elementos UI
    QComboBox* comboTipoBusqueda;
    QLineEdit* editBusquedaSimple;
    QSpinBox* spinFechaDesde;
    QSpinBox* spinFechaHasta;
    QPushButton* btnBuscar;
    QTableWidget* tablaResultados;
    QLabel* labelCampo;

    void setupUI();
    void configurarTabla();
    void mostrarCamposPorTipo(int tipo);
    void buscarPorTitulo(const std::string& titulo);
    void buscarPorISBN(const std::string& isbn);
    void buscarPorGenero(const std::string& genero);
    void buscarPorFecha(int desde, int hasta);
    void limpiarTabla();
    void llenarTablaDesdeListaEncontados(ListaEncontados* resultados);
    void llenarTablaDesdeListaLibros(ListaLibros* resultados);
    void llenarTablaDesdeLibroUnico(Libro* libro);
    void mostrarMensajeNoResultados();
};

#endif //BIBLIOTECA_MAGICA_BUSQUEDAUNIFICADA_H