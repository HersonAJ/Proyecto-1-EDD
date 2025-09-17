#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPlainTextEdit>

#include "../ArbolB/ArbolB.h"
#include "../AVL/ArbolAVL.h"

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

private:
    void createMenu();
    QPlainTextEdit *logWidget;
   ArbolAVL arbol;
    ArbolB arbolB;
    void appendLog(const std::string &mensaje, const QString &tipo = "info");
};

#endif // MAINWINDOW_H
