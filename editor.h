#ifndef EDITOR_H
#define EDITOR_H

#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QCloseEvent>
#include <QString>

class Editor : public QTextEdit {
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);

    void loadFile(const QString &path);

private slots:
    void findText();
    void importFile();
    void saveFile();
    void saveFileAs();

    void highlightExtraSelection();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private:
    QString file_path;
};

#endif