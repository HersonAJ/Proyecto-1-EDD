#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QFileDialog>
#include <QMessageBox>
#include "../csv/LectorCSV.h"
#include "../include/Recorridos.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Biblioteca Mágica");
    resize(800, 600);
    createMenu();
}

void MainWindow::createMenu() {
    QMenuBar *menuBar = new QMenuBar(this);

    QMenu *menuArchivo = new QMenu("Archivo", this);

    QAction *actionCargar = new QAction("Cargar archivo", this);
    connect(actionCargar, &QAction::triggered, this, &MainWindow::onCargarArchivo);
    menuArchivo->addAction(actionCargar);

    QAction *actionExportar = new QAction("Exportar AVL como imagen", this);
    connect(actionExportar, &QAction::triggered, this, &MainWindow::onExportarAVL);
    menuArchivo->addAction(actionExportar);

    menuBar->addMenu(menuArchivo);
    setMenuBar(menuBar);
}

void MainWindow::onCargarArchivo() {
    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo CSV",
        "",
        "Archivo CSV (*.csv);;Todos los archivos(*)");

    if (ruta.isEmpty()) return;

    std::string rutaArchivo = ruta.toStdString();

    LectorCSV lector(rutaArchivo, arbol);
    lector.procesarArchivo();

    QMessageBox::information(this, "Éxito", "Archivo cargado y procesado correctamente.");

    std::cout << "\nRecorrido inOrden del árbol AVL:\n";
    Recorridos<NodoAVL>::inOrden(arbol.getRaiz());
}

void MainWindow::onExportarAVL() {
    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar imagen del árbol",
        "",
        "Imagen PNG (*.png)"
    );

    if (ruta.isEmpty()) return;

    std::string dotFile = "arbol.dot";
    arbol.guardarComoDOT(dotFile);

    std::string comando = "dot -Tpng " + dotFile + " -o \"" + ruta.toStdString() + "\"";
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        QMessageBox::information(this, "Éxito", "Árbol exportado correctamente.");
    } else {
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen. Verifica que Graphviz esté instalado.");
    }
}
