#include <QApplication>
#include "GUI/MainWindow.h"

int main(int argc, char *argv[]) {

    qputenv("QT_QPA_PLATFORM", "xcb");
    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return QApplication::exec();
}