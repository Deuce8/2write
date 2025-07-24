#ifndef EDITOR_H
#define EDITOR_H

#include <QTextEdit>
#include <QDragEnterEvent>
#include <QString>

class Editor : public QTextEdit {
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);

    QString getLoadFile();
    QString getSaveFile();
    bool loadFile(const QString &path);
    bool saveFile();
    bool saveFileAs(const QString &path);
    bool findText();

    void highlightExtraSelection();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    QString file_path;
};

#endif