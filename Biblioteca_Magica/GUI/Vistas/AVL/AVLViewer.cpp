#include "AVLViewer.h"
#include  <QPixmap>
#include <cstdlib>

AVLViewer::AVLViewer(ArbolAVL* arbol, QWidget* parent) : QWidget(parent), arbol(arbol) {
    QVBoxLayout* layout = new QVBoxLayout(this);
    imagenLabel = new QLabel(this);
    imagenLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imagenLabel);
    setLayout(layout);
}

void AVLViewer::actualizarVista() {
    std::string dotFile = "arbol.dot";
    std::string pngFile = "arbol.png";

    arbol->guardarComoDOT(dotFile);
    std::string comando = "dot -Tpng " + dotFile + " -o " + pngFile;
    int resultado = system(comando.c_str());

    if (resultado == 0) {
        QPixmap pix(QString::fromStdString(pngFile));
        if (!pix.isNull()) {
            // aplicar zoom con el factor escala
            imagenLabel->setPixmap(pix.scaled(pix.size() * escala,
                                              Qt::KeepAspectRatio,
                                              Qt::SmoothTransformation));
        }
    }
}

#include <QWheelEvent>

void AVLViewer::wheelEvent(QWheelEvent* event) {
    if (event->modifiers() & Qt::ControlModifier) {
        if (event->angleDelta().y() > 0) {
            escala *= 1.1; // zoom in
        } else {
            escala /= 1.1; // zoom out
        }
        actualizarVista();
        event->accept();
    } else {
        QWidget::wheelEvent(event); // comportamiento normal
    }
}
