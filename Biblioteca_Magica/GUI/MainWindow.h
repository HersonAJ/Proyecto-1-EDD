#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include "../include/AVL/ArbolAVL.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void onCargarArchivo();

private:
    void createMenu();
   // ArbolAVL arbol;
};

#endif // MAINWINDOW_H
