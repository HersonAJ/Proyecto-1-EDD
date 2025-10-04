#include "BPlusViewer.h"
#include <QPixmap>
#include <cstdlib>
#include <ctime>
#include "../../../include/ExportadorDotBPlus.h"

BPlusViewer::BPlusViewer(ArbolBPlus* arbol, QWidget* parent)
    : QWidget(parent), arbol(arbol)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    imagenLabel = new QLabel(this);
    imagenLabel->setAlignment(Qt::AlignCenter);
    imagenLabel->setScaledContents(true);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(imagenLabel);
    scrollArea->setWidgetResizable(true);
    layout->addWidget(scrollArea);

    setLayout(layout);
}

void BPlusViewer::actualizarVista() {
    std::string dotFile = "bplus_viewer.dot";
    std::string svgFile = "bplus_viewer.svg";

    if (!ExportarDotBPlus::generarArchivo(*arbol, dotFile)) {
        return;
    }

    std::string comando = "dot -Tsvg " + dotFile + " -o " + svgFile;
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        QPixmap pix(QString::fromStdString(svgFile));
        if (!pix.isNull()) {
            imagenOriginal = pix;
            imagenLabel->setPixmap(imagenOriginal.scaled(
                imagenOriginal.size() * escala,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation
            ));
        }
    }
    std::remove(dotFile.c_str());
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