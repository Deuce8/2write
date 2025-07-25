#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H

#include <QTextEdit>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QString>

class TextEdit : public QTextEdit {
    Q_OBJECT

public:
    TextEdit(QWidget *parent = nullptr);

    void loadFile(const QString &path);

    QString getFilePath() const;

public slots:
    void findText();
    void importFile();
    void saveFile();
    void saveFileAs();
    void zoomIn();
    void zoomOut();

private slots:
    void highlightExtraSelection();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

private:
    int zoom;
    QString filePath;
};

#endif