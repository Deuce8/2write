#include <QApplication>
#include <Qt>
#include <QFileInfo>

#include "editor.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Editor editor;

    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        if (QFileInfo::exists(filePath))
            editor.loadFile(filePath);
    }

    editor.show();
    return app.exec();
}