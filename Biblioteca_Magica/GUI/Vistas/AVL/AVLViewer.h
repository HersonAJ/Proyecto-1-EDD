#ifndef BIBLIOTECA_MAGICA_AVLVIEWER_H
#define BIBLIOTECA_MAGICA_AVLVIEWER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "../../../AVL/ArbolAVL.h"

class AVLViewer: public QWidget {
    Q_OBJECT

    public:
    explicit AVLViewer(ArbolAVL* arbol, QWidget* parent = nullptr);

    void actualizarVista();

    private:
    ArbolAVL* arbol;
    QLabel* imagenLabel;
    double escala = 1.0;

    protected:
    void wheelEvent(QWheelEvent* event) override;
};


#endif //BIBLIOTECA_MAGICA_AVLVIEWER_H