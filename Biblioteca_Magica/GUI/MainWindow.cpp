#include "MainWindow.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include "../csv/LectorCSV.h"
#include <QTextCursor>
#include <QTextCharFormat>
#include <QColor>
#include <QInputDialog>
#include <QLineEdit>
#include "../AVL/EliminacionAVL.h"
#include "../include/ExportadorDotB.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Biblioteca Mágica");
    resize(1000, 700);

    // Widget central con layout vertical
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(central);

    // Crear menú
    createMenu();

    // Crear panel de log
    logWidget = new QPlainTextEdit(this);
    logWidget->setReadOnly(true);
    logWidget->setPlaceholderText("Aquí se mostrarán los mensajes del sistema...");
    layout->addWidget(logWidget);

    central->setLayout(layout);
    setCentralWidget(central);
}

void MainWindow::createMenu() {
    QMenuBar *menuBar = new QMenuBar(this);

    // ===== Menú Archivo =====
    QMenu *menuArchivo = new QMenu("Archivo", this);

    QAction *actionCargar = new QAction("Cargar archivo CSV", this);
    connect(actionCargar, &QAction::triggered, this, &MainWindow::onCargarArchivo);
    menuArchivo->addAction(actionCargar);

    QAction *actionExportar = new QAction("Exportar AVL como imagen", this);
    connect(actionExportar, &QAction::triggered, this, &MainWindow::onExportarAVL);
    menuArchivo->addAction(actionExportar);

    QAction *actionExportarB = new QAction("Exportar B", this);
    connect(actionExportarB, &QAction::triggered, this, &MainWindow::onExportarB);
    menuArchivo->addAction(actionExportarB);

    menuArchivo->addSeparator();

    QAction *actionSalir = new QAction("Salir", this);
    connect(actionSalir, &QAction::triggered, this, &QMainWindow::close);
    menuArchivo->addAction(actionSalir);

    // ===== Menú Libros =====
    QMenu *menuLibros = new QMenu("Libros", this);

    QAction *actionAgregar = new QAction("Agregar libro", this);
    menuLibros->addAction(actionAgregar);

    QAction *actionEliminar = new QAction("Eliminar libro", this);
    connect(actionEliminar, &QAction::triggered, this, &MainWindow::onEliminarLibro);
    menuLibros->addAction(actionEliminar);

    QMenu *menuBuscar = new QMenu("Buscar libro", this);
    QAction *actionBuscarTitulo = new QAction("Por título", this);
    connect(actionBuscarTitulo, &QAction::triggered, this, &MainWindow::onBuscarPorTitulo);
    menuBuscar->addAction(actionBuscarTitulo);
    menuBuscar->addAction("Por ISBN");
    menuBuscar->addAction("Por género");
    menuBuscar->addAction("Por rango de fechas");
    menuLibros->addMenu(menuBuscar);

    // ===== Menú Visualización =====
    QMenu *menuVisualizacion = new QMenu("Visualización", this);
    menuVisualizacion->addAction("Ver AVL");
    menuVisualizacion->addAction("Ver Árbol B");
    menuVisualizacion->addAction("Ver Árbol B+");
    menuVisualizacion->addSeparator();
    menuVisualizacion->addAction("Comparar rendimiento");

    // ===== Menú Herramientas =====
    QMenu *menuHerramientas = new QMenu("Herramientas", this);
    menuHerramientas->addAction("Mostrar estadísticas");
    menuHerramientas->addAction("Mostrar log");

    // ===== Menú Ayuda =====
    QMenu *menuAyuda = new QMenu("Ayuda", this);
    menuAyuda->addAction("Acerca de");

    // Agregar menús a la barra
    menuBar->addMenu(menuArchivo);
    menuBar->addMenu(menuLibros);
    menuBar->addMenu(menuVisualizacion);
    menuBar->addMenu(menuHerramientas);
    menuBar->addMenu(menuAyuda);

    setMenuBar(menuBar);
}

void MainWindow::appendLog(const std::string &mensaje, const QString &tipo) {
    QTextCharFormat formato;
    if (tipo == "ok") {
        formato.setForeground(QBrush(QColor("green")));
    } else if (tipo == "error") {
        formato.setForeground(QBrush(QColor("red")));
    } else {
        formato.setForeground(QBrush(QColor("black")));
    }

    QTextCursor cursor = logWidget->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.insertText(QString::fromStdString(mensaje) + "\n", formato);
    logWidget->setTextCursor(cursor);
    logWidget->ensureCursorVisible();
}

void MainWindow::onCargarArchivo() {
    QString ruta = QFileDialog::getOpenFileName(
        this,
        "Seleccionar archivo CSV",
        "",
        "Archivo CSV (*.csv);;Todos los archivos(*)");

    if (ruta.isEmpty()) return;

    std::string rutaArchivo = ruta.toStdString();

    appendLog("Cargando archivo: " + rutaArchivo, "info");

    LectorCSV lector(rutaArchivo, arbol, arbolB);
    lector.setLogger([this](const std::string &msg) {
        // Detectar si es error o no
        if (msg.rfind("Error", 0) == 0) { // empieza con "Error"
            appendLog(msg, "error");
        } else {
            appendLog(msg, "ok");
        }
    });

    lector.procesarArchivo();

    appendLog("Archivo procesado correctamente.");
    QMessageBox::information(this, "Éxito", "Archivo cargado y procesado correctamente.");
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
        appendLog("Imagen exportada: " + ruta.toStdString(), "Ok");
        QMessageBox::information(this, "Éxito", "Árbol exportado correctamente.");
    } else {
        appendLog("Error al generar la imagen.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen. Verifica que Graphviz esté instalado.");
    }
}

void MainWindow::onBuscarPorTitulo() {
    if (arbol.estaVacio()) {
        appendLog("El árbol está vacío. Cargue datos antes de buscar.", "error");
        return;
    }

    bool ok;
    QString titulo = QInputDialog::getText(this, "Buscar por título",
                                           "Título del libro:", QLineEdit::Normal,
                                           "", &ok);
    if (!ok || titulo.isEmpty()) return;

    Libro libroBuscado(titulo.toStdString(), "", "", "", "");
    NodoAVL* encontrado = arbol.buscar(libroBuscado, arbol.getRaiz());

    if (encontrado) {
        appendLog("Libro encontrado: " + encontrado->dato.toString(), "ok");
        QMessageBox::information(this, "Resultado",
            QString::fromStdString(encontrado->dato.toString()));
    } else {
        appendLog("No se encontró el libro con título: " + titulo.toStdString(), "error");
        QMessageBox::warning(this, "Sin resultados", "No se encontró el libro.");
    }
}

void MainWindow::onEliminarLibro() {
    if (arbol.estaVacio()) {
        appendLog("El árbol está vacío. No hay libros para eliminar.", "error");
        return;
    }

    bool ok;
    QString titulo = QInputDialog::getText(this, "Eliminar libro",
                                           "Título del libro a eliminar:", QLineEdit::Normal,
                                           "", &ok);
    if (!ok || titulo.isEmpty()) return;

    Libro libroAEliminar(titulo.toStdString(), "", "", "", "");

    NodoAVL* encontrado = arbol.buscar(libroAEliminar, arbol.getRaiz());
    if (!encontrado) {
        appendLog("No se encontró el libro con título: " + titulo.toStdString(), "error");
        QMessageBox::warning(this, "Sin resultados", "No se encontró el libro.");
        return;
    }

    // 🔹 Usar la versión recursiva que actualiza la raíz
    EliminacionAVL::eliminar(arbol, libroAEliminar);

    appendLog("Libro eliminado: " + titulo.toStdString(), "ok");
    QMessageBox::information(this, "Eliminado", "El libro ha sido eliminado correctamente.");
}

void MainWindow::onExportarB() {
    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar imagen del Árbol B",
        "",
        "Imagen PNG (*.png)"
    );

    if (ruta.isEmpty()) return;

    std::string dotFile = "arbolB.dot";
    if (!ExportarDotB::generarArchivo(arbolB, dotFile)) {
        appendLog("Error al generar archivo DOT del Árbol B.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar el archivo DOT.");
        return;
    }

    std::string comando = "dot -Tpng " + dotFile + " -o \"" + ruta.toStdString() + "\"";
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        appendLog("Imagen del Árbol B exportada: " + ruta.toStdString(), "ok");
        QMessageBox::information(this, "Éxito", "Árbol B exportado correctamente.");
    } else {
        appendLog("Error al generar la imagen del Árbol B.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen. Verifica que Graphviz esté instalado.");
    }
}
