#include "BPlusViewer.h"
#include <QPixmap>
#include <cstdlib>
#include "../../../include/ExportadorDotB+.h"

BPlusViewer::BPlusViewer(ArbolBPlus* arbol, QWidget* parent)
    : QWidget(parent), arbol(arbol)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    imagenLabel = new QLabel(this);
    imagenLabel->setAlignment(Qt::AlignCenter);
    imagenLabel->setScaledContents(true);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imagenLabel);
    scrollArea->setWidgetResizable(true); // la imagen se adapta al área
    layout->addWidget(scrollArea);

    setLayout(layout);
}

void BPlusViewer::actualizarVista() {
    std::string dotFile = "arbolBPlus.dot";
    std::string pngFile = "arbolBPlus.png";

    if (!ExportarDotBPlus::generarArchivo(*arbol, dotFile)) {
        return;
    }

    std::string comando = "dot -Tpng " + dotFile + " -o " + pngFile;
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        QPixmap pix(QString::fromStdString(pngFile));
        if (!pix.isNull()) {
            imagenOriginal = pix; // guarda la imagen original
            imagenLabel->setPixmap(imagenOriginal.scaled(
                imagenOriginal.size() * escala,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
        }
    }
}

void BPlusViewer::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            escala *= 1.1; // zoom in
        } else {
            escala /= 1.1; // zoom out
        }

        if (!imagenOriginal.isNull()) {
            imagenLabel->setPixmap(imagenOriginal.scaled(
                imagenOriginal.size() * escala,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
        }

        event->accept();
    } else {
        QWidget::wheelEvent(event);
    }
}