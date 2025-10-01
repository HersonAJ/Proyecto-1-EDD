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

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onCargarArchivo();
    void onExportarAVL();
    void onExportarB();
    void onBuscarPorTitulo();
    void onEliminarLibro();
    void onBuscarPorFecha();

private:
    void createMenu();
    QPlainTextEdit *logWidget;
   ArbolAVL arbol;
    ArbolB arbolB;
    IndiceISBN indiceISBN;
    ArbolBPlus arbolBPlus;
    void appendLog(const std::string &mensaje, const QString &tipo = "info");
    void debugMostrarArbolB();

    QTabWidget* tabs;
    AVLViewer* avlViewer;
    BViewer* bViewer;
    BPlusViewer* bPlusViewer;
};

#endif // MAINWINDOW_H
