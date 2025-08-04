#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <QTextEdit>

class TextEdit : public QTextEdit {
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);

    QString getFilePath() const { return filePath; };

public slots:
    void findNext();
    void findPrev();

    void saveFile();
    void saveFileAs();
    void zoomIn();
    void zoomOut();
    void setFind(const QString &find);

    void loadFile(const QString &path);

private slots:
    void highlightExtraSelection();

private:
    int zoom;
    QString toFind;
    QString filePath;
};

#endif