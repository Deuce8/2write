#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QCloseEvent>

#include "text_edit.h"
#include "find_toolbar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int argc = 1, char *argv[] = nullptr);

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    TextEdit *textEdit;
    FindToolbar *findToolbar;
};

#endif