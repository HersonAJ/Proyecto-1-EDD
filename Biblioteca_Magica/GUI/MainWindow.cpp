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
#include "../include/ExportadorDotB.h"
#include "Vistas/AVL/AVLViewer.h"
#include "Vistas/B/BViewer.h"
#include  <QLabel>
#include  <QTabWidget>

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

    // Crear tabs
    tabs = new QTabWidget(this);
    tabs->addTab(logWidget, "Log");

    // Crear viewer AVL y agregarlo como pestaña
    avlViewer = new AVLViewer(&arbol, this);
    tabs->addTab(avlViewer, "AVL");

    bViewer = new BViewer(&arbolB, this);
    tabs->addTab(bViewer, "Arbol B");

    //espacio para visualizar los otros arboles

    // Agregar tabs al layout central
    layout->addWidget(tabs);


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

    /*QAction* actionBuscarFecha = new QAction("Por año de publicacion rango", this);
    connect(actionBuscarFecha, &QAction::triggered, this, &MainWindow::onBuscarPorFecha);
    menuBuscar->addAction(actionBuscarFecha);*/
    menuBuscar->addAction(actionBuscarTitulo);
    menuBuscar->addAction("Por ISBN");
    menuBuscar->addAction("Por género");
    menuLibros->addMenu(menuBuscar);

    // ===== Menú Visualización =====
    QMenu *menuVisualizacion = new QMenu("Visualización", this);
    //menuVisualizacion->addAction("Ver AVL");
    QAction *actionVerAVL = new QAction("Ver AVL", this);
    connect(actionVerAVL, &QAction::triggered, this, [this]() {
        avlViewer->actualizarVista();            // refresca la imagen
        tabs->setCurrentWidget(avlViewer);       // cambia a la pestaña AVL
    });

    QAction *actionVerB = new QAction("Ver arbol B", this);
    connect(actionVerB, &QAction::triggered, this, [this]() {
        bViewer->actualizarVista();
        tabs->setCurrentWidget(bViewer);
    });

    menuVisualizacion->addAction(actionVerB);
    menuVisualizacion->addAction(actionVerAVL);

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

    LectorCSV lector(rutaArchivo, arbol, arbolB, indiceISBN);
    lector.setLogger([this](const std::string &msg) {
        // Detectar si es error o no
        if (msg.rfind("Error", 0) == 0) { // empieza con "Error"
            appendLog(msg, "error");
        } else {
            appendLog(msg, "ok");
        }
    });

    lector.procesarArchivo();

    // 🔥 NUEVO: Debug del Árbol B
    appendLog("=== ESTRUCTURA DEL ÁRBOL B (DEBUG) ===", "info");

    // Crear un string temporal para capturar la salida
    std::stringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());

    // Llamar a la función de impresión
    arbolB.imprimirParaPrueba();

    // Restaurar cout y obtener el resultado
    std::cout.rdbuf(old);
    std::string resultado = buffer.str();

    // Mostrar en el log línea por línea
    std::istringstream stream(resultado);
    std::string linea;
    while (std::getline(stream, linea)) {
        appendLog(linea, "debug");
    }

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
    std::remove(dotFile.c_str());
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

    // primera coincidencia
    Libro* primerEncontrado = arbol.buscarPorTitulo(titulo.toStdString());

    // lista completa de coincidencias
    ListaEcontados* lista = arbol.buscarTodosPorTitulo(titulo.toStdString());
    Nodo* actual = lista->getCabeza();

    if (primerEncontrado && actual) {
        QMessageBox::information(this, "Resultados", "Se encontraron coincidencias.");

        // salto de línea antes de mostrar resultados
        appendLog("\n--- Resultados de búsqueda ---", "ok");

        // primer resultado
        appendLog("Primer libro encontrado:\n " + primerEncontrado->toString(), "ok");

        // otras coincidencias
        int contador = 0;
        actual = lista->getCabeza();

        QString encabezado = "Otras coincidencias:";
        bool hayOtras = false;

        while (actual) {
            if (actual->libro != primerEncontrado) {
                if (!hayOtras) {
                    appendLog(encabezado.toStdString(), "ok");
                    hayOtras = true;
                }
                contador++;
                appendLog((QString::number(contador) + ". " +
                           QString::fromStdString(actual->libro->toString())).toStdString(), "ok");
            }
            actual = actual->siguiente;
        }

        if (!hayOtras) {
            appendLog("No hay otras coincidencias.", "info");
        }

    } else {
        appendLog("No se encontró el libro con el título: " + titulo.toStdString(), "error");
        QMessageBox::warning(this, "Sin resultados", "No se encontró el libro.");
    }

    delete lista;
}

void MainWindow::onEliminarLibro() {
    if (arbol.estaVacio()) {
        appendLog("El árbol está vacío. No hay libros para eliminar.", "error");
        return;
    }

    bool ok;
    QString isbn = QInputDialog::getText(this, "Eliminar libro",
                                         "ISBN del libro a eliminar:", QLineEdit::Normal,
                                         "", &ok);
    if (!ok || isbn.isEmpty()) return;

    std::string isbnStr = isbn.toStdString();

    // Eliminar directamente en el AVL
    arbol.eliminarPorISBN(isbnStr, indiceISBN);

    // Eliminar también en el Árbol B (recorriendo por ISBN)
    try {
        //arbolB.eliminarPorISBN(isbn.toStdString());  //
        appendLog("Árbol B después de eliminar.", "info");
//        debugMostrarArbolB();
    } catch (const std::exception& e) {
        appendLog("Error al eliminar en el Árbol B con ISBN: " + isbn.toStdString(), "error");
    }

    // (más adelante se hará lo mismo con el B+)
    appendLog("Libro eliminado con ISBN: " + isbn.toStdString(), "ok");
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

/*
void MainWindow::onBuscarPorFecha() {
   /* if (!arbolB.getRaiz()) {
        appendLog("El arbol B esta vacio. Cargue datos antes de buscar.", "error");
        return;
    }

    //dialogo para el año inicial
    bool ok1;
    int inicio = QInputDialog::getInt(this, "Rango de fechas - Inicio", "Año inicial: ", 2000, 0, 3000, 1, &ok1);
    if (!ok1) return;;

    //dialogo para el año final
    bool ok2;
    int fin = QInputDialog::getInt(this, "Rango de fechas - Fin", "Año final: ", 2020, inicio, 3000, 1, &ok2);
    if (!ok2) return;

    //validar rango
    if (inicio > fin) {
        appendLog("Error: El año inicial no puede ser mayor al año final,", "error");
        QMessageBox::warning(this, "Error", "El año inicial no puede ser mayor al año final.");
        return;
    }

    //realizar busqueda por rango
    ListaLibros resultados = arbolB.buscarPorRango(inicio, fin);

    if (resultados.getTamaño() > 0) {
        std::string mensaje = "Se encontraron " + std::to_string(resultados.getTamaño()) + " Libros entre " + std::to_string(inicio) + " y " +
            std::to_string(fin) + ":\n";

        //recorrer resultados
        Nodo* actual = resultados.getCabeza();
        while (actual != nullptr) {
            mensaje += "- " + actual->libro->toString() + "\n";
            actual = actual->siguiente;
        }

        appendLog(mensaje, "ok");

        //Mostrar resultado en QMessageBox
        QMessageBox::information(this, "Resultado de la busqueda ", QString::fromStdString("Encontrados: " + std::to_string(resultados.getTamaño()) +
            "libros\nUse el log para ver detalles"));
    } else {
        appendLog("No se encontraron libros entre " + std::to_string(inicio) + " y " + std::to_string(fin), "error");
        QMessageBox::information(this, "Sin resultados", "No se encontraron libros en ese rango de fechas.");
    }
}

void MainWindow::debugMostrarArbolB() {
    /*std::stringstream ss;
    ss << "contenido del arbol B inOrden;\n";
    std::streambuf* oldCoutBuffer = std::cout.rdbuf();
    std::cout.rdbuf(ss.rdbuf());

    //RecorridosB::inOrden(arbolB.getRaiz());
    //RecorridosB2::imprimirEstructura(arbolB.getRaiz());
    ArbolB3::imprimir(arbolB.getRaiz());

    std::cout.rdbuf(oldCoutBuffer);

    appendLog(ss.str(), "debug");
}*/