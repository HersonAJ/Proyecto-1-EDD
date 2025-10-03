#include "PruebaRendimiento.h"
#include <QHBoxLayout>
#include <QGroupBox>

PruebaRendimiento::PruebaRendimiento(ArbolAVL* arbolTitulos,
                                   IndiceISBN* indiceISBN,
                                   Catalogo* catalogo,
                                   QWidget* parent)
    : QWidget(parent),
      arbolTitulos(arbolTitulos),
      indiceISBN(indiceISBN),
      catalogo(catalogo)
{
    setupUI();
}

void PruebaRendimiento::setupUI() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Grupo de controles
    QGroupBox* groupControles = new QGroupBox("Configuración de Prueba");
    QVBoxLayout* layoutControles = new QVBoxLayout(groupControles);

    // Tipo de búsqueda
    QHBoxLayout* layoutTipo = new QHBoxLayout();
    layoutTipo->addWidget(new QLabel("Tipo de búsqueda:"));
    comboBusqueda = new QComboBox();
    comboBusqueda->addItem("Por Título");
    comboBusqueda->addItem("Por ISBN");
    connect(comboBusqueda, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &PruebaRendimiento::onTipoBusquedaChanged);
    layoutTipo->addWidget(comboBusqueda);
    layoutControles->addLayout(layoutTipo);

    // Campo de búsqueda
    QHBoxLayout* layoutBusqueda = new QHBoxLayout();
    labelInstruccion = new QLabel("Título a buscar:");
    layoutBusqueda->addWidget(labelInstruccion);
    editBusqueda = new QLineEdit();
    editBusqueda->setPlaceholderText("Ingrese el título...");
    layoutBusqueda->addWidget(editBusqueda);
    layoutControles->addLayout(layoutBusqueda);

    // Botón
    btnComparar = new QPushButton("Comparar Rendimiento");
    connect(btnComparar, &QPushButton::clicked, this, &PruebaRendimiento::onCompararClicked);
    layoutControles->addWidget(btnComparar);

    mainLayout->addWidget(groupControles);

    // Área de resultados
    QGroupBox* groupResultados = new QGroupBox("Resultados");
    QVBoxLayout* layoutResultados = new QVBoxLayout(groupResultados);
    textResultados = new QTextEdit();
    textResultados->setReadOnly(true);
    layoutResultados->addWidget(textResultados);

    mainLayout->addWidget(groupResultados);
}

void PruebaRendimiento::onTipoBusquedaChanged(int index) {
    if (index == 0) { // Título
        labelInstruccion->setText("Título a buscar:");
        editBusqueda->setPlaceholderText("Ingrese el título...");
    } else { // ISBN
        labelInstruccion->setText("ISBN a buscar:");
        editBusqueda->setPlaceholderText("Ingrese el ISBN...");
    }
}

void PruebaRendimiento::onCompararClicked() {
    QString texto = editBusqueda->text().trimmed();
    if (texto.isEmpty()) {
        appendResultado("❌ Error: Ingrese un valor para buscar", "red");
        return;
    }

    int tipo = comboBusqueda->currentIndex();
    std::string busqueda = texto.toStdString();

    textResultados->clear();
    appendResultado("🔍 INICIANDO COMPARACIÓN DE RENDIMIENTO", "blue");
    appendResultado("========================================", "blue");

    if (tipo == 0) {
        ejecutarComparacionTitulo(busqueda);
    } else {
        ejecutarComparacionISBN(busqueda);
    }
}

template<typename Func>
long long PruebaRendimiento::medirTiempo(Func funcion) {
    auto inicio = std::chrono::high_resolution_clock::now();
    funcion();
    auto fin = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(fin - inicio).count();
}

void PruebaRendimiento::ejecutarComparacionTitulo(const std::string& titulo) {
    appendResultado("📚 COMPARACIÓN: BÚSQUEDA POR TÍTULO", "darkblue");
    appendResultado("Buscando: \"" + QString::fromStdString(titulo) + "\"");
    appendResultado("");

    // Búsqueda secuencial
    Libro* resultadoSecuencial = nullptr;
    long long tiempoSecuencial = medirTiempo([&]() {
        resultadoSecuencial = catalogo->buscarTituloSecuencial(titulo);
    });

    // Búsqueda binaria (AVL)
    Libro* resultadoBinario = nullptr;
    long long tiempoBinario = medirTiempo([&]() {
        resultadoBinario = arbolTitulos->buscarPorTitulo(titulo);
    });

    // Mostrar resultados
    appendResultado("📊 RESULTADOS:", "darkgreen");
    appendResultado("• Búsqueda Secuencial (Lista): " + QString::number(tiempoSecuencial) + " μs",
                   resultadoSecuencial ? "green" : "red");
    appendResultado("• Búsqueda Binaria (AVL): " + QString::number(tiempoBinario) + " μs",
                   resultadoBinario ? "green" : "red");

    if (tiempoSecuencial > 0) {
        double mejora = ((double)tiempoSecuencial - tiempoBinario) / tiempoSecuencial * 100;
        QString mejoraStr = QString::number(std::abs(mejora), 'f', 2);
        appendResultado("• Mejora: " + mejoraStr + "% " +
                       (mejora > 0 ? "más rápido" : "más lento"), "orange");
    }
}

void PruebaRendimiento::ejecutarComparacionISBN(const std::string& isbn) {
    appendResultado("🔖 COMPARACIÓN: BÚSQUEDA POR ISBN", "darkblue");
    appendResultado("Buscando: \"" + QString::fromStdString(isbn) + "\"");
    appendResultado("");

    // Búsqueda secuencial
    Libro* resultadoSecuencial = nullptr;
    long long tiempoSecuencial = medirTiempo([&]() {
        resultadoSecuencial = catalogo->buscarISBNSecuencial(isbn);
    });

    // Búsqueda binaria (IndiceISBN)
    Libro* resultadoBinario = nullptr;
    long long tiempoBinario = medirTiempo([&]() {
        resultadoBinario = indiceISBN->buscar(isbn);
    });

    // Mostrar resultados
    appendResultado("📊 RESULTADOS:", "darkgreen");
    appendResultado("• Búsqueda Secuencial (Lista): " + QString::number(tiempoSecuencial) + " μs",
                   resultadoSecuencial ? "green" : "red");
    appendResultado("• Búsqueda Binaria (AVL ISBN): " + QString::number(tiempoBinario) + " μs",
                   resultadoBinario ? "green" : "red");

    if (tiempoSecuencial > 0) {
        double mejora = ((double)tiempoSecuencial - tiempoBinario) / tiempoSecuencial * 100;
        QString mejoraStr = QString::number(std::abs(mejora), 'f', 2);
        appendResultado("• Mejora: " + mejoraStr + "% " +
                       (mejora > 0 ? "más rápido" : "más lento"), "orange");
    }
}

void PruebaRendimiento::appendResultado(const QString& texto, const QString& color) {
    textResultados->append("<span style='color:" + color + ";'>" + texto + "</span>");
}