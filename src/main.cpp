#include <QApplication>

#include "main_window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow(nullptr, argc, argv);
    mainWindow.show();
    
    return app.exec();
}