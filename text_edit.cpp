#include "text_edit.h"

#include <QFile>
#include <QMimeData>
#include <QPalette>
#include <QFileDialog>
#include <Qt>
#include <QFont>
#include <QSettings>

#pragma region Constructor

TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent) {
    setStyleSheet(QString("background-color: %1").arg(qApp->palette().color(QPalette::Base).name()));

    setFont(QFont("Hack", 10));
    setLineWrapMode(QTextEdit::NoWrap);
    moveCursor(QTextCursor::End);

    QSettings settings = QSettings("2write");

    setPlainText(settings.value("text", "").toString());
    filePath = settings.value("filePath", "/home").toString();

    connect(this, &QTextEdit::selectionChanged, this, &TextEdit::highlightExtraSelection );
}

#pragma endregion Constructor
#pragma region Public Functions

void TextEdit::loadFile(const QString &path){
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextDocument* doc = new QTextDocument(this);
    doc->setPlainText(QTextStream(&file).readAll());
    setDocument(doc);

    moveCursor(QTextCursor::End);
    
    filePath = path;
    file.close();
}

QString TextEdit::getFilePath() const {
    return filePath;
}

#pragma endregion Public Functions
#pragma region Qt Events

void TextEdit::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void TextEdit::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    
    loadFile(urls.first().toLocalFile());
}

#pragma endregion Qt Events
#pragma region Public Slots

void TextEdit::findText(){
    QString selected = textCursor().selectedText();
    if (selected.isEmpty())
        return;

    if (!find(selected))
        return;

    moveCursor(QTextCursor::Start);
    find(selected);
}

void TextEdit::importFile(){
    loadFile(QFileDialog::getOpenFileName(this, tr("Open Text File"), filePath, tr("Text Files (*.txt);;All Files (*)")));
}

void TextEdit::saveFile(){
    if (filePath.isEmpty()){
        saveFileAs();
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream(&file) << toPlainText();
    file.close();
}

void TextEdit::saveFileAs(){
    const QString path = QFileDialog::getSaveFileName(this, tr("Save Text File"), filePath);

    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream(&file) << toPlainText();
    filePath = path;
    file.close();
}

void TextEdit::zoomIn(){
    QTextEdit::zoomIn(1);
    zoom += 1;
}

void TextEdit::zoomOut(){
    QTextEdit::zoomOut(1);
    zoom -= 1;
}

#pragma endregion Public Slots
#pragma region Private Slots

void TextEdit::highlightExtraSelection() {
    const QString current_selection = textCursor().selectedText();
    if (current_selection.isEmpty()){
        setExtraSelections({});
        return;
    }

    QTextDocument *doc = document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);

    QTextCharFormat format;
    format.setBackground(qApp->palette().highlight().color().lighter(64));
    format.setForeground(qApp->palette().highlightedText().color());

    QList<QTextEdit::ExtraSelection> selections;

    while (true) {
        if (cursor.isNull())
            break;

        cursor = doc->find(current_selection, cursor);
        selections.append(QTextEdit::ExtraSelection{cursor, format});
        cursor.setPosition(cursor.selectionEnd());
    }

    setExtraSelections(selections);
}

#pragma endregion Private Slots