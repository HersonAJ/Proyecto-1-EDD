#include "AVLViewer.h"
#include <QPixmap>
#include <cstdlib>

AVLViewer::AVLViewer(ArbolAVL* arbol, QWidget* parent)
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

void AVLViewer::actualizarVista() {
    std::string dotFile = "avl_viewer.dot";
    std::string svgFile = "avl_viewer.svg";

    arbol->guardarComoDOT(dotFile);
    std::string comando = "dot -Tsvg " + dotFile + " -o " + svgFile;
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        QPixmap pix(QString::fromStdString(svgFile));
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

void AVLViewer::wheelEvent(QWheelEvent* event) {
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
