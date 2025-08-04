#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QDragEnterEvent>
#include <QDropEvent>

#include "text_edit.h"
#include "find_toolbar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int argc = 1, char *argv[] = nullptr);

private slots:
    void importFile();

signals:
    void fileLoaded(const QString &path);

protected:
    void closeEvent(QCloseEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    TextEdit *textEdit;
    FindToolbar *findToolbar;
};

#endif