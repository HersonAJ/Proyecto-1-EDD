#include "BusquedaUnificada.h"
#include <QFormLayout>
#include <QGroupBox>

BusquedaUnificada::BusquedaUnificada(ArbolAVL* arbolTitulos,IndiceISBN* indiceISBN,ArbolB* arbolB,ArbolBPlus* arbolBPlus,std::function<void(const std::string&, const QString&)> logCallBack,QWidget* parent)
    : QWidget(parent),
      arbolTitulos(arbolTitulos),
      indiceISBN(indiceISBN),
      arbolB(arbolB),
      arbolBPlus(arbolBPlus),
      //salida para el appendlog
      appendLog(logCallBack) {
    setupUI();
}

void BusquedaUnificada::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Grupo de controles de búsqueda
    QGroupBox* groupBusqueda = new QGroupBox("Configurar Búsqueda");
    QFormLayout* layoutBusqueda = new QFormLayout(groupBusqueda);

    // Tipo de búsqueda
    comboTipoBusqueda = new QComboBox();
    comboTipoBusqueda->addItem("Por Título");
    comboTipoBusqueda->addItem("Por ISBN");
    comboTipoBusqueda->addItem("Por Género");
    comboTipoBusqueda->addItem("Por Fecha");
    connect(comboTipoBusqueda, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BusquedaUnificada::onTipoBusquedaCambiado);
    layoutBusqueda->addRow("Tipo de búsqueda:", comboTipoBusqueda);

    // Campos dinámicos
    QHBoxLayout* layoutCampos = new QHBoxLayout();

    // Campo simple (para título, ISBN, género)
    editBusquedaSimple = new QLineEdit();
    editBusquedaSimple->setPlaceholderText("Ingrese término de búsqueda...");
    layoutCampos->addWidget(editBusquedaSimple);

    // Campos de fecha (ocultos inicialmente)
    spinFechaDesde = new QSpinBox();
    spinFechaDesde->setRange(0, 3000);
    spinFechaDesde->setValue(1900);
    spinFechaDesde->setVisible(false);
    spinFechaDesde->setPrefix("Desde: ");

    spinFechaHasta = new QSpinBox();
    spinFechaHasta->setRange(0, 3000);
    spinFechaHasta->setValue(2024);
    spinFechaHasta->setVisible(false);
    spinFechaHasta->setPrefix("Hasta: ");

    layoutCampos->addWidget(spinFechaDesde);
    layoutCampos->addWidget(spinFechaHasta);

    labelCampo = new QLabel("Término:");
    layoutBusqueda->addRow(labelCampo, layoutCampos);

    // Botón buscar
    btnBuscar = new QPushButton(" Buscar");
    btnBuscar->setStyleSheet("QPushButton { font-weight: bold; padding: 5px; }");
    connect(btnBuscar, &QPushButton::clicked, this, &BusquedaUnificada::onBuscarClicked);
    layoutBusqueda->addRow(btnBuscar);

    mainLayout->addWidget(groupBusqueda);

    // Tabla de resultados
    QGroupBox* groupResultados = new QGroupBox("Resultados");
    QVBoxLayout* layoutResultados = new QVBoxLayout(groupResultados);
    tablaResultados = new QTableWidget();
    configurarTabla();
    layoutResultados->addWidget(tablaResultados);

    mainLayout->addWidget(groupResultados);

    // Mostrar campos iniciales
    mostrarCamposPorTipo(0);
}

void BusquedaUnificada::configurarTabla() {
    tablaResultados->setColumnCount(7);
    QStringList headers;
    headers << "No." << "Título" << "Autor" << "ISBN" << "Género" << "Fecha" << "Ejemplares";
    tablaResultados->setHorizontalHeaderLabels(headers);

    // Configurar propiedades de la tabla
    tablaResultados->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaResultados->setSelectionMode(QAbstractItemView::SingleSelection);
    tablaResultados->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaResultados->setAlternatingRowColors(true);
    tablaResultados->setSortingEnabled(false);

    // Ajustar columnas
    tablaResultados->horizontalHeader()->setStretchLastSection(true);
    tablaResultados->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tablaResultados->verticalHeader()->setVisible(false);

    // Estilos
    tablaResultados->setStyleSheet(
        "QTableWidget {"
        "    gridline-color: #d0d0d0;"
        "    background-color: white;"
        "    alternate-background-color: #f8f8f8;"
        "}"
        "QHeaderView::section {"
        "    background-color: #2196F3;"
        "    color: white;"
        "    font-weight: bold;"
        "    padding: 5px;"
        "    border: 1px solid #1976D2;"
        "}"
    );

    // Mostrar tabla vacía inicialmente
    limpiarTabla();
}

void BusquedaUnificada::onTipoBusquedaCambiado(int index) {
    mostrarCamposPorTipo(index);
}

void BusquedaUnificada::mostrarCamposPorTipo(int tipo) {
    // Ocultar todos primero
    editBusquedaSimple->setVisible(false);
    spinFechaDesde->setVisible(false);
    spinFechaHasta->setVisible(false);

    // Mostrar los apropiados según el tipo
    switch(tipo) {
        case 0: // Título
            labelCampo->setText("Título:");
            editBusquedaSimple->setPlaceholderText("Ingrese título del libro...");
            editBusquedaSimple->setVisible(true);
            break;
        case 1: // ISBN
            labelCampo->setText("ISBN:");
            editBusquedaSimple->setPlaceholderText("Ingrese ISBN del libro...");
            editBusquedaSimple->setVisible(true);
            break;
        case 2: // Género
            labelCampo->setText("Género:");
            editBusquedaSimple->setPlaceholderText("Ingrese género literario...");
            editBusquedaSimple->setVisible(true);
            break;
        case 3: // Fecha
            labelCampo->setText("Rango de años:");
            spinFechaDesde->setVisible(true);
            spinFechaHasta->setVisible(true);
            break;
    }
}

void BusquedaUnificada::onBuscarClicked() {
    int tipo = comboTipoBusqueda->currentIndex();

    // Validaciones básicas
    switch(tipo) {
        case 0: case 1: case 2: // Título, ISBN, Género
            if (editBusquedaSimple->text().trimmed().isEmpty()) {
                QMessageBox::warning(this, "Campo vacío", "Ingrese un término de búsqueda.");
                return;
            }
            break;
        case 3: // Fecha
            if (spinFechaDesde->value() > spinFechaHasta->value()) {
                QMessageBox::warning(this, "Rango inválido", "El año 'desde' no puede ser mayor al año 'hasta'.");
                return;
            }
            break;
    }

    // Ejecutar búsqueda según tipo
    switch(tipo) {
        case 0: // Título
            buscarPorTitulo(editBusquedaSimple->text().trimmed().toStdString());
            break;
        case 1: // ISBN
            buscarPorISBN(editBusquedaSimple->text().trimmed().toStdString());
            break;
        case 2: // Género
            buscarPorGenero(editBusquedaSimple->text().trimmed().toStdString());
            break;
        case 3: // Fecha
            buscarPorFecha(spinFechaDesde->value(), spinFechaHasta->value());
            break;
    }
}

void BusquedaUnificada::buscarPorTitulo(const std::string& titulo) {
    if (!arbolTitulos || arbolTitulos->estaVacio()) {
        appendLog("El árbol de títulos está vacío. Cargue datos antes de buscar.", "error");
        QMessageBox::information(this, "Árbol vacío", "El árbol de títulos está vacío.");
        return;
    }

    appendLog("\n--- Búsqueda por Título: '" + titulo + "' ---", "ok");

    // primera coincidencia
    Libro* primerEncontrado = arbolTitulos->buscarPorTitulo(titulo);

    // lista completa de coincidencias
    ListaEncontados* resultados = arbolTitulos->buscarTodosPorTitulo(titulo);

    if (primerEncontrado && resultados->getCabeza()) {
        appendLog("Primer libro encontrado:\n " + primerEncontrado->toString(), "ok");

        // Otras coincidencias
        Nodo* actual = resultados->getCabeza();
        int contador = 0;
        bool hayOtras = false;

        while (actual) {
            if (actual->libro != primerEncontrado) {
                if (!hayOtras) {
                    appendLog("Otras coincidencias:", "ok");
                    hayOtras = true;
                }
                contador++;
                appendLog(std::to_string(contador) + ". " + actual->libro->toString(), "ok");
            }
            actual = actual->siguiente;
        }

        if (!hayOtras) {
            appendLog("No hay otras coincidencias.", "info");
        }

        QMessageBox::information(this, "Resultados", "Se encontraron coincidencias.");
    } else {
        appendLog("No se encontró el libro con el título: " + titulo, "error");
        QMessageBox::warning(this, "Sin resultados", "No se encontró el libro.");
    }

    llenarTablaDesdeListaEncontados(resultados);
    delete resultados;
}

void BusquedaUnificada::buscarPorISBN(const std::string& isbn) {
    if (!indiceISBN || indiceISBN->estaVacio()) {
        appendLog("El índice ISBN está vacío. Cargue datos antes de buscar.", "error");
        QMessageBox::information(this, "Índice vacío", "El índice ISBN está vacío.");
        return;
    }

    appendLog("\n--- Búsqueda por ISBN: '" + isbn + "' ---", "ok");

    Libro* resultado = indiceISBN->buscar(isbn);

    if (resultado) {
        appendLog("Libro encontrado:\n" + resultado->toString(), "ok");
        QMessageBox::information(this, "Libro encontrado", "Se encontró el libro con ISBN: " + QString::fromStdString(isbn));
    } else {
        appendLog("No se encontró ningún libro con ISBN: " + isbn, "error");
        QMessageBox::warning(this, "Sin resultados", "No se encontró ningún libro con ese ISBN.");
    }

    llenarTablaDesdeLibroUnico(resultado);
}

void BusquedaUnificada::buscarPorGenero(const std::string& genero) {
    if (!arbolBPlus || arbolBPlus->getRaiz() == nullptr) {
        appendLog("El árbol B+ está vacío. Cargue datos antes de buscar.", "error");
        QMessageBox::information(this, "Árbol vacío", "El árbol B+ está vacío.");
        return;
    }

    appendLog("\n--- Búsqueda por Género: '" + genero + "' ---", "ok");

    ListaLibros* resultados = arbolBPlus->buscarPorGenero(genero);

    if (resultados->getTamaño() > 0) {
        std::string encabezado = "Se encontraron " + std::to_string(resultados->getTamaño()) +
                                 " libros en el género '" + genero + "':";
        appendLog(encabezado, "warning");

        // Recorrer resultados para el log
        std::string mensaje;
        auto iterador = resultados->obtenerIterador();
        while (iterador.tieneSiguiente()) {
            Libro* libro = iterador.siguiente();
            mensaje += "- " + libro->toString() + "\n";
        }
        appendLog(mensaje, "ok");

        QMessageBox::information(this, "Resultado de la búsqueda",
                                 QString::fromStdString("Encontrados: " +
                                 std::to_string(resultados->getTamaño()) +
                                 " libros"));
    } else {
        appendLog("No se encontraron libros en el género '" + genero + "'", "error");
        QMessageBox::information(this, "Sin resultados",
                                 QString::fromStdString("No se encontraron libros en el género '" + genero + "'."));
    }

    llenarTablaDesdeListaLibros(resultados);
    delete resultados;
}

void BusquedaUnificada::buscarPorFecha(int desde, int hasta) {
    if (!arbolB || !arbolB->getRaiz()) {
        appendLog("El árbol B está vacío. Cargue datos antes de buscar.", "error");
        QMessageBox::information(this, "Árbol vacío", "El árbol B está vacío.");
        return;
    }

    appendLog("\n--- Búsqueda por Fecha: " + std::to_string(desde) + " - " + std::to_string(hasta) + " ---", "ok");

    ListaLibros* resultados = arbolB->buscarPorRango(desde, hasta);

    if (resultados->getTamaño() > 0) {
        std::string encabezado = "Se encontraron " + std::to_string(resultados->getTamaño()) +
                                 " libros entre " + std::to_string(desde) + " y " + std::to_string(hasta) + ":";
        appendLog(encabezado, "warning");

        // Recorrer resultados para el log
        std::string mensaje;
        auto iterador = resultados->obtenerIterador();
        while (iterador.tieneSiguiente()) {
            Libro* libro = iterador.siguiente();
            mensaje += "- " + libro->toString() + "\n";
        }
        appendLog(mensaje, "ok");

        QMessageBox::information(this, "Resultado de la búsqueda",
                                QString::fromStdString("Encontrados: " + std::to_string(resultados->getTamaño()) +
                                " libros\nUse el log para ver detalles"));
    } else {
        appendLog("No se encontraron libros entre " + std::to_string(desde) + " y " + std::to_string(hasta), "error");
        QMessageBox::information(this, "Sin resultados", "No se encontraron libros en ese rango de fechas.");
    }

    llenarTablaDesdeListaLibros(resultados);
    delete resultados;
}

void BusquedaUnificada::limpiarTabla() {
    tablaResultados->setRowCount(0);
}

void BusquedaUnificada::llenarTablaDesdeListaEncontados(ListaEncontados* resultados) {
    limpiarTabla();

    if (!resultados || !resultados->getCabeza()) {
        mostrarMensajeNoResultados();
        return;
    }

    Nodo* actual = resultados->getCabeza();
    int fila = 0;

    while (actual) {
        tablaResultados->insertRow(fila);

        Libro* libro = actual->libro;
        tablaResultados->setItem(fila, 0, new QTableWidgetItem(QString::number(fila + 1)));
        tablaResultados->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(libro->getTitulo())));
        tablaResultados->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(libro->getAutor())));
        tablaResultados->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(libro->getIsbn())));
        tablaResultados->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(libro->getGenero())));
        tablaResultados->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(libro->getFecha())));
        tablaResultados->setItem(fila, 6, new QTableWidgetItem(QString::number(libro->getCantidad())));

        actual = actual->siguiente;
        fila++;
    }

    tablaResultados->resizeColumnsToContents();
    tablaResultados->setColumnWidth(0, 50);
}

void BusquedaUnificada::llenarTablaDesdeListaLibros(ListaLibros* resultados) {
    limpiarTabla();

    if (!resultados || resultados->getTamaño() == 0) {
        mostrarMensajeNoResultados();
        return;
    }

    tablaResultados->setRowCount(resultados->getTamaño());

    int fila = 0;
    auto iterador = resultados->obtenerIterador();

    while (iterador.tieneSiguiente()) {
        Libro* libro = iterador.siguiente();

        tablaResultados->setItem(fila, 0, new QTableWidgetItem(QString::number(fila + 1)));
        tablaResultados->setItem(fila, 1, new QTableWidgetItem(QString::fromStdString(libro->getTitulo())));
        tablaResultados->setItem(fila, 2, new QTableWidgetItem(QString::fromStdString(libro->getAutor())));
        tablaResultados->setItem(fila, 3, new QTableWidgetItem(QString::fromStdString(libro->getIsbn())));
        tablaResultados->setItem(fila, 4, new QTableWidgetItem(QString::fromStdString(libro->getGenero())));
        tablaResultados->setItem(fila, 5, new QTableWidgetItem(QString::fromStdString(libro->getFecha())));
        tablaResultados->setItem(fila, 6, new QTableWidgetItem(QString::number(libro->getCantidad())));

        fila++;
    }

    tablaResultados->resizeColumnsToContents();
    tablaResultados->setColumnWidth(0, 50);
}

void BusquedaUnificada::llenarTablaDesdeLibroUnico(Libro* libro) {
    limpiarTabla();

    if (!libro) {
        mostrarMensajeNoResultados();
        return;
    }

    tablaResultados->setRowCount(1);

    tablaResultados->setItem(0, 0, new QTableWidgetItem("1"));
    tablaResultados->setItem(0, 1, new QTableWidgetItem(QString::fromStdString(libro->getTitulo())));
    tablaResultados->setItem(0, 2, new QTableWidgetItem(QString::fromStdString(libro->getAutor())));
    tablaResultados->setItem(0, 3, new QTableWidgetItem(QString::fromStdString(libro->getIsbn())));
    tablaResultados->setItem(0, 4, new QTableWidgetItem(QString::fromStdString(libro->getGenero())));
    tablaResultados->setItem(0, 5, new QTableWidgetItem(QString::fromStdString(libro->getFecha())));
    tablaResultados->setItem(0, 6, new QTableWidgetItem(QString::number(libro->getCantidad())));

    tablaResultados->resizeColumnsToContents();
    tablaResultados->setColumnWidth(0, 50);
}

void BusquedaUnificada::mostrarMensajeNoResultados() {
    tablaResultados->setRowCount(1);
    tablaResultados->setItem(0, 0, new QTableWidgetItem("No se encontraron resultados"));
    tablaResultados->setSpan(0, 0, 1, 7);
}