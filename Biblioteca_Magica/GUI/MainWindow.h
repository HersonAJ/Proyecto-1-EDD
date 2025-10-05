#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPlainTextEdit>
#include <sstream>

#include "../ArbolB/ArbolB.h"
#include "../AVL/ArbolAVL.h"
#include "Vistas/AVL/AVLViewer.h"
#include "Vistas/B/BViewer.h"
#include "../AVL_Auxiliar/IndiceISBN.h"
#include "../ArbolB+/ArbolBPlus.h"
#include "Vistas/B+/BPlusViewer.h"
#include "../Modelos/Catalogo.h"
#include "Vistas/Rendimiento/PruebaRendimiento.h"
#include "../csv/LectorCSV.h"
#include "Vistas/OrdenAlfabetico/ListadoAlfabetico.h"
#include "Vistas/Busqueda/BusquedaUnificada.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onCargarArchivo();
    void onExportarAVL();
    void onExportarB();
    void onExportarBPlus();
    void onEliminarLibro();
    void onAgregarLibro();
    void actualizarTodasLasVistas();

private:
    void createMenu();
    QPlainTextEdit *logWidget;
   ArbolAVL arbol;
    ArbolB arbolB;
    IndiceISBN indiceISBN;
    ArbolBPlus arbolBPlus;
    Catalogo catalogoGlobal;
    LectorCSV lectorCSV;
    void appendLog(const std::string &mensaje, const QString &tipo = "info");
    void debugMostrarArbolB();

    QTabWidget* tabs;
    AVLViewer* avlViewer;
    BViewer* bViewer;
    BPlusViewer* bPlusViewer;
    PruebaRendimiento* rendimiento;
    ListadoAlfabetico* listadoAlfabetico;
    BusquedaUnificada* busquedaUnificada;
};

#endif // MAINWINDOW_H
