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
#include "../include/ExportadorDotBPlus.h"
#include "Vistas/AVL/AVLViewer.h"
#include "Vistas/B/BViewer.h"
#include "Vistas/B+/BPlusViewer.h"
#include  <QLabel>
#include  <QTabWidget>
#include <QFormLayout>
#include <QDialogButtonBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    lectorCSV(arbol, arbolB, indiceISBN, arbolBPlus, catalogoGlobal)
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

    listadoAlfabetico = new ListadoAlfabetico(&arbol, this);
    tabs->addTab(listadoAlfabetico, "Listado Alfabetico");

    // Crear viewer AVL y agregarlo como pestaña
    avlViewer = new AVLViewer(&arbol, this);
    tabs->addTab(avlViewer, "AVL");

    bViewer = new BViewer(&arbolB, this);
    tabs->addTab(bViewer, "Arbol B");

    bPlusViewer = new BPlusViewer(&arbolBPlus, this);
    tabs->addTab(bPlusViewer, "BPlus");

    busquedaUnificada = new BusquedaUnificada(&arbol, &indiceISBN, &arbolB, &arbolBPlus,[this](const std::string& mensaje, const QString& tipo) {
        this->appendLog(mensaje, tipo);},this);
    tabs->addTab(busquedaUnificada, "Busqueda");

    rendimiento = new PruebaRendimiento(&arbol, &indiceISBN, &catalogoGlobal, this);
    tabs->addTab(rendimiento, "Rendimiento");

    // Agregar tabs al layout central
    layout->addWidget(tabs);
    central->setLayout(layout);
    setCentralWidget(central);
}

void MainWindow::createMenu() {
    QMenuBar *menuBar = new QMenuBar(this);

    //Menu Archivo
    QMenu *menuArchivo = new QMenu("Archivo", this);

    QAction *actionCargar = new QAction("Cargar archivo CSV", this);
    connect(actionCargar, &QAction::triggered, this, &MainWindow::onCargarArchivo);
    menuArchivo->addAction(actionCargar);

    QAction *actionExportar = new QAction("Exportar AVL", this);
    connect(actionExportar, &QAction::triggered, this, &MainWindow::onExportarAVL);
    menuArchivo->addAction(actionExportar);

    QAction *actionExportarB = new QAction("Exportar B", this);
    connect(actionExportarB, &QAction::triggered, this, &MainWindow::onExportarB);
    menuArchivo->addAction(actionExportarB);

    QAction *actionExportarBPlus = new QAction("Exportar B+", this);
    connect(actionExportarBPlus, &QAction::triggered, this, &MainWindow::onExportarBPlus);
    menuArchivo->addAction(actionExportarBPlus);

    menuArchivo->addSeparator();

    QAction *actionSalir = new QAction("Salir", this);
    connect(actionSalir, &QAction::triggered, this, &QMainWindow::close);
    menuArchivo->addAction(actionSalir);

    //Menu Libros
    QMenu *menuLibros = new QMenu("Libros", this);

    QAction *actionAgregar = new QAction("Agregar libro", this);
    connect(actionAgregar, &QAction::triggered, this, &MainWindow::onAgregarLibro);
    menuLibros->addAction(actionAgregar);

    QAction *actionEliminar = new QAction("Eliminar libro", this);
    connect(actionEliminar, &QAction::triggered, this, &MainWindow::onEliminarLibro);
    menuLibros->addAction(actionEliminar);

    QAction *actionBuscarUnificado = new QAction("Buscar un libro", this);
    connect(actionBuscarUnificado, &QAction::triggered, this, [this]() {
       tabs->setCurrentWidget(busquedaUnificada);
    });

    menuLibros->addAction(actionBuscarUnificado);

    //Menu Visualizacion
    QMenu *menuVisualizacion = new QMenu("Visualización", this);

    QAction *actionVerListado = new QAction("Ver Listado Alfabético", this);
    connect(actionVerListado, &QAction::triggered, this, [this]() {
        listadoAlfabetico->cargarDatosEnTabla();
        tabs->setCurrentWidget(listadoAlfabetico);
    });
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

    QAction *actionVerBPlus = new QAction("Ver arbol B+", this);
    connect(actionVerBPlus, &QAction::triggered, this, [this]() {
        bPlusViewer->actualizarVista();
        tabs->setCurrentWidget(bPlusViewer);
    });

    QAction *actionCompararRendimiento = new QAction("Comparar rendimiento", this);
    connect(actionCompararRendimiento, &QAction::triggered, this, [this]() {
        tabs->setCurrentWidget(rendimiento);
    });

    menuVisualizacion->addAction(actionVerListado);
    menuVisualizacion->addAction(actionVerB);
    menuVisualizacion->addAction(actionVerAVL);
    menuVisualizacion->addAction(actionVerBPlus);
    menuVisualizacion->addSeparator();

    // Agregar menús a la barra
    menuBar->addMenu(menuArchivo);
    menuBar->addMenu(menuLibros);
    menuBar->addMenu(menuVisualizacion);

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

    //usar la sobrecarga con ruta específica
    lectorCSV.setLogger([this](const std::string &msg) {
        if (msg.rfind("Error", 0) == 0) {
            appendLog(msg, "error");
        } else {
            appendLog(msg, "ok");
        }
    });

    lectorCSV.procesarArchivo(rutaArchivo);  //usar la sobrecarga

    appendLog("Archivo procesado correctamente.");
    QMessageBox::information(this, "Éxito", "Archivo cargado y procesado correctamente.");
    actualizarTodasLasVistas();
}

void MainWindow::onExportarAVL() {
    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar imagen del árbol AVL",
        "",
        "Imagen SVG (*.svg);;Imagen PNG (*.png)"
    );

    if (ruta.isEmpty()) return;

    //Nombre único con timestamp para evitar solapamientos
    std::string timestamp = std::to_string(std::time(nullptr));
    std::string dotFile = "arbol_export_" + timestamp + ".dot";

    //Determinar formato basado en extensión del archivo
    std::string formato = (ruta.endsWith(".svg", Qt::CaseInsensitive)) ? "svg" : "png";

    arbol.guardarComoDOT(dotFile);

    std::string comando = "dot -T" + formato + " " + dotFile + " -o \"" + ruta.toStdString() + "\"";
    int resultado = system(comando.c_str());

    // Limpiar archivo temporal DOT
    std::remove(dotFile.c_str());

    if (resultado == 0) {
        appendLog("Árbol AVL exportado: " + ruta.toStdString() + " (" + formato + ")", "ok");
        QMessageBox::information(this, "Éxito", "Árbol AVL exportado correctamente.");
    } else {
        appendLog("Error al generar la imagen del árbol AVL.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen. Verifica que Graphviz esté instalado.");
    }
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

    //búsqueda en el índice global
    Libro* libro = indiceISBN.buscar(isbnStr);
    if (!libro) {
        appendLog("Libro con ISBN '" + isbnStr + "' no encontrado.", "error");
        return;
    }

    //Obtener todos los datos necesarios
    std::string titulo = libro->getTitulo();
    std::string fecha = libro->getFecha();
    std::string genero = libro->getGenero();

    //Eliminar de todas las estructuras
    arbol.eliminarPorISBN(isbnStr, titulo);      // AVL general
    arbolB.eliminarPorISBN(isbnStr, fecha);      // Árbol B
    arbolBPlus.eliminarPorISBN(isbnStr, genero); // Árbol B+
    bool eliminadoDelCatalogo = catalogoGlobal.eliminarLibroPorISBN(isbnStr);

    // eliminar del índice global
    indiceISBN.eliminar(isbnStr);

    appendLog("Libro eliminado con ISBN: " + isbnStr, "ok");
    QMessageBox::information(this, "Eliminado", "El libro ha sido eliminado correctamente.");
    actualizarTodasLasVistas();
}

void MainWindow::onExportarB() {
    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar imagen del Árbol B",
        "",
        "Imagen SVG (*.svg);;Imagen PNG (*.png)"
    );

    if (ruta.isEmpty()) return;

    //Nombre único con timestamp para evitar solapamientos
    std::string timestamp = std::to_string(std::time(nullptr));
    std::string dotFile = "arbolB_export_" + timestamp + ".dot";

    // Determinar formato basado en extensión del archivo
    std::string formato = (ruta.endsWith(".svg", Qt::CaseInsensitive)) ? "svg" : "png";

    if (!ExportarDotB::generarArchivo(arbolB, dotFile)) {
        appendLog("Error al generar archivo DOT del Árbol B.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar el archivo DOT.");
        return;
    }

    std::string comando = "dot -T" + formato + " " + dotFile + " -o \"" + ruta.toStdString() + "\"";
    int resultado = system(comando.c_str());

    //Limpiar archivo temporal DOT
    std::remove(dotFile.c_str());

    if (resultado == 0) {
        appendLog("Árbol B exportado: " + ruta.toStdString() + " (" + formato + ")", "ok");
        QMessageBox::information(this, "Éxito", "Árbol B exportado correctamente.");
    } else {
        appendLog("Error al generar la imagen del Árbol B.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen. Verifica que Graphviz esté instalado.");
    }
}

void MainWindow::onExportarBPlus() {
    if (arbolBPlus.getRaiz() == nullptr) {
        appendLog("El árbol B+ está vacío. No hay nada que exportar.", "error");
        return;
    }

    QString ruta = QFileDialog::getSaveFileName(
        this,
        "Guardar imagen del Árbol B+",
        "",
        "Imagen SVG (*.svg);;Imagen PNG (*.png)"
    );

    if (ruta.isEmpty()) return;

    //Nombre único con timestamp para evitar solapamientos
    std::string timestamp = std::to_string(std::time(nullptr));
    std::string dotFile = "arbolBPlus_export_" + timestamp + ".dot";

    //Determinar formato basado en extensión del archivo
    std::string formato = (ruta.endsWith(".svg", Qt::CaseInsensitive)) ? "svg" : "png";

    if (!ExportarDotBPlus::generarArchivo(arbolBPlus, dotFile)) {
        appendLog("Error al generar archivo DOT del Árbol B+.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar el archivo DOT.");
        return;
    }

    std::string comando = "dot -T" + formato + " " + dotFile + " -o \"" + ruta.toStdString() + "\"";
    int resultado = system(comando.c_str());

    //Limpiar archivo temporal DOT
    std::remove(dotFile.c_str());

    if (resultado == 0) {
        appendLog("Árbol B+ exportado: " + ruta.toStdString() + " (" + formato + ")", "ok");
        QMessageBox::information(this, "Éxito", "Árbol B+ exportado correctamente.");
    } else {
        appendLog("Error al generar la imagen del Árbol B+.", "error");
        QMessageBox::warning(this, "Error", "No se pudo generar la imagen. Verifica que Graphviz esté instalado.");
    }
}

void MainWindow::onAgregarLibro() {
    // Diálogo para ingresar datos del libro
    QDialog dialog(this);
    dialog.setWindowTitle("Agregar Nuevo Libro");
    dialog.setFixedSize(400, 300);

    QFormLayout form(&dialog);

    // Campos de entrada
    QLineEdit *editTitulo = new QLineEdit(&dialog);
    QLineEdit *editISBN = new QLineEdit(&dialog);
    QLineEdit *editGenero = new QLineEdit(&dialog);
    QLineEdit *editFecha = new QLineEdit(&dialog);
    QLineEdit *editAutor = new QLineEdit(&dialog);

    // Agregar campos al formulario
    form.addRow("Título:", editTitulo);
    form.addRow("ISBN:", editISBN);
    form.addRow("Género:", editGenero);
    form.addRow("Fecha (año):", editFecha);
    form.addRow("Autor:", editAutor);

    // Botones
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
                               Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Mostrar diálogo
    if (dialog.exec() != QDialog::Accepted) {
        return; // Usuario canceló
    }

    // Obtener datos
    QString titulo = editTitulo->text().trimmed();
    QString isbn = editISBN->text().trimmed();
    QString genero = editGenero->text().trimmed();
    QString fecha = editFecha->text().trimmed();
    QString autor = editAutor->text().trimmed();

    // Validaciones
    if (titulo.isEmpty() || isbn.isEmpty() || genero.isEmpty() || fecha.isEmpty() || autor.isEmpty()) {
        appendLog("Error: Todos los campos son obligatorios", "error");
        QMessageBox::warning(this, "Error", "Todos los campos son obligatorios.");
        return;
    }

    // Convertir a std::string
    std::string tituloStr = titulo.toStdString();
    std::string isbnStr = isbn.toStdString();
    std::string generoStr = genero.toStdString();
    std::string fechaStr = fecha.toStdString();
    std::string autorStr = autor.toStdString();

    //Usar la MISMA instancia persistente de LectorCSV
    bool exito = lectorCSV.agregarLibroIndividual(tituloStr, isbnStr, generoStr, fechaStr, autorStr);

    if (exito) {
        // Actualizar vistas
        actualizarTodasLasVistas();

        appendLog("Libro agregado manualmente: " + tituloStr + " - ISBN: " + isbnStr, "ok");
        QMessageBox::information(this, "Éxito", "Libro agregado correctamente al sistema.");
    } else {
        appendLog("Error al agregar libro manualmente: " + tituloStr, "error");
        QMessageBox::warning(this, "Error", "No se pudo agregar el libro. Verifique que el ISBN no exista y los datos sean válidos.");
    }
}
void MainWindow::actualizarTodasLasVistas() {
    //arboles
    avlViewer->actualizarVista();
    bViewer->actualizarVista();
    bPlusViewer->actualizarVista();
    listadoAlfabetico->cargarDatosEnTabla();
}