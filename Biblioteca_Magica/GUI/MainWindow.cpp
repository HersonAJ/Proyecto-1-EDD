#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
//#include "../csv/LectorCSV.h"
//#include "../include/Recorridos.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Configuración básica de la ventana
    setWindowTitle("Biblioteca Mágica");
    resize(800, 600);

    // Crear el menú
    createMenu();
}

void MainWindow::createMenu() {
    // Crear barra de menú
    QMenuBar *menuBar = new QMenuBar(this);

    // Crear menú "Archivo"
    QMenu *menuArchivo = new QMenu("Archivo", this);

    // Acción "Cargar archivo"
    QAction *actionCargar = new QAction("Cargar archivo", this);

    connect(actionCargar, &QAction::triggered, this,&MainWindow::onCargarArchivo);

    menuArchivo->addAction(actionCargar);
    // Agregar el menú a la barra
    menuBar->addMenu(menuArchivo);
    // Establecer la barra de menú en la ventana
    setMenuBar(menuBar);
}

void MainWindow::onCargarArchivo() {

    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo CSV",
        "",
        "Archivo CSV (*.csv);;Todos los archivos(*)");

    if (ruta.isEmpty()) {
        return;
    }

    std::string rutaArchivo = ruta.toStdString();

   // LectorCSV lector(rutaArchivo, arbol);
    //lector.procesarArchivo();

    QMessageBox::information(this, "Exito", "Archivo cargado y procesado correctamente, ");
    //prueba de lectura
   // std::cout << "\n Recorrido inOrden del arbol AVL: \n" ;
   // Recorridos<NodoAVL>::inOrden(arbol.getRaiz());
}
