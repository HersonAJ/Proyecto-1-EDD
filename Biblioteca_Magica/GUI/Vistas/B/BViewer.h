#ifndef BIBLIOTECA_MAGICA_BVIEWER_H
#define BIBLIOTECA_MAGICA_BVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWheelEvent>
#include "../../../ArbolB/ArbolB.h"

class BViewer : public QWidget {
    Q_OBJECT

public:
    explicit BViewer(ArbolB* arbol, QWidget* parent = nullptr);
    void actualizarVista();

private:
    ArbolB* arbol;
    QLabel* imagenLabel;
    QScrollArea* scrollArea;
    QPixmap imagenOriginal;
    double escala = 1.0;

protected:
    void wheelEvent(QWheelEvent *event) override;
};

#endif // BIBLIOTECA_MAGICA_BVIEWER_H
