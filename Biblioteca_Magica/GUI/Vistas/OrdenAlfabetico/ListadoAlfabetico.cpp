#include "ListadoAlfabetico.h"
#include <QHeaderView>
#include <QScrollBar>

ListadoAlfabetico::ListadoAlfabetico(ArbolAVL* arbolTitulos, QWidget* parent)
    : QWidget(parent), arbolTitulos(arbolTitulos) {
    setupUI();
}

void ListadoAlfabetico::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Título
    QLabel* titulo = new QLabel("📚 LISTADO DE LIBROS EN ORDEN ALFABÉTICO");
    titulo->setStyleSheet("font-size: 16px; font-weight: bold; margin: 10px;");
    mainLayout->addWidget(titulo);

    // Tabla
    tablaLibros = new QTableWidget();
    configurarTabla();
    mainLayout->addWidget(tablaLibros);
}

void ListadoAlfabetico::configurarTabla() {
    tablaLibros->setColumnCount(7);
    QStringList headers;
    headers << "No." << "Título" << "Autor" << "ISBN" << "Género" << "Fecha" << "Ejemplares";
    tablaLibros->setHorizontalHeaderLabels(headers);

    // Configurar propiedades de la tabla
    tablaLibros->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaLibros->setSelectionMode(QAbstractItemView::SingleSelection);
    tablaLibros->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaLibros->setAlternatingRowColors(true);
    tablaLibros->setSortingEnabled(true);

    // Ajustar columnas
    tablaLibros->horizontalHeader()->setStretchLastSection(true);
    tablaLibros->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tablaLibros->verticalHeader()->setVisible(false);

    // Estilos
    tablaLibros->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #d0d0d0;"
        "    background-color: white;"
        "    alternate-background-color: #f8f8f8;"
        "}"
        "QHeaderView::section {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "    border: 1px solid #3d8b40;"
        "}"
    );
}

void ListadoAlfabetico::cargarDatosEnTabla() {
    if (!arbolTitulos || arbolTitulos->estaVacio()) {
        tablaLibros->setRowCount(1);
        tablaLibros->setItem(0, 0, new QTableWidgetItem("No hay libros en el catálogo"));
        tablaLibros->setSpan(0, 0, 1, 7);
        return;
    }

    // Obtener libros ordenados alfabéticamente
    ListaLibros* librosOrdenados = arbolTitulos->obtenerLibrosEnOrdenAlfabetico();

    // Configurar número de filas
    tablaLibros->setRowCount(librosOrdenados->getTamaño());

    // Llenar la tabla usando el iterador
    int fila = 0;
    auto iterador = librosOrdenados->obtenerIterador();

    while (iterador.tieneSiguiente()) {
        Libro* libro = iterador.siguiente();

        // Crear items para cada columna
        tablaLibros->setItem(fila, 0, new QTableWidgetItem(QString::number(fila + 1)));
        tablaLibros->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(libro->getTitulo())));
        tablaLibros->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(libro->getAutor())));
        tablaLibros->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(libro->getIsbn())));
        tablaLibros->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(libro->getGenero())));
        tablaLibros->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(libro->getFecha())));
        tablaLibros->setItem(fila, 6, new QTableWidgetItem(QString::number(libro->getCantidad())));

        fila++;
    }

    // Ajustar el tamaño de las columnas al contenido
    tablaLibros->resizeColumnsToContents();

    // Hacer la columna "No." más estrecha ya que son solo números
    tablaLibros->setColumnWidth(0, 50);

    // Liberar memoria
    delete librosOrdenados;
}