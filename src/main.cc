#include <iostream>
#include <QApplication>

#include "gui.h"

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);

    yurzi::MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
