#ifndef BIBLIOTECA_MAGICA_BPLUSVIEWER_H
#define BIBLIOTECA_MAGICA_BPLUSVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWheelEvent>
#include "../../../ArbolB+/ArbolBPlus.h"

class BPlusViewer : public QWidget {
    Q_OBJECT

public:
    explicit BPlusViewer(ArbolBPlus* arbol, QWidget* parent = nullptr);
    void actualizarVista();

private:
    ArbolBPlus* arbol;
    QLabel* imagenLabel;
    QScrollArea* scrollArea;
    QPixmap imagenOriginal;
    double escala = 1.0;

protected:
    void wheelEvent(QWheelEvent *event) override;
};


#endif //BIBLIOTECA_MAGICA_BPLUSVIEWER_H