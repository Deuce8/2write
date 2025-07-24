#include "editor.h"

#include <QFile>
#include <QMimeData>
#include <QPalette>
#include <QShortcut>
#include <QKeySequence>
#include <QFileDialog>
#include <Qt>
#include <QFont>

#pragma region Constructor

Editor::Editor(QWidget *parent) : QTextEdit(parent) {
    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);
    setAcceptDrops(true);
    setWindowTitle(tr("2Write"));
    setStyleSheet(QString("background-color: %1").arg(qApp->palette().color(QPalette::Base).name()));
    setViewportMargins(12, 12, 12, 12);
    
    setFont(QFont("Hack", 10));
    setLineWrapMode(QTextEdit::NoWrap);

    connect(new QShortcut(QKeySequence(QKeySequence::Open), this), &QShortcut::activated, this, [this]() { this->loadFile(getLoadFile()); });
    connect(new QShortcut(QKeySequence(QKeySequence::Save), this), &QShortcut::activated, this, [this]() { this->saveFile(); });
    connect(new QShortcut(QKeySequence(QKeySequence::SaveAs), this), &QShortcut::activated, this, [this]() { this->saveFileAs(getSaveFile()); });
    connect(new QShortcut(QKeySequence(QKeySequence::Find), this), &QShortcut::activated, this, [this]() { this->findText(); });

    connect(this, &QTextEdit::selectionChanged, this, [this]() { this->highlightExtraSelection(); });
}

#pragma endregion Constructor
#pragma region Input + Output

QString Editor::getLoadFile(){
    return QFileDialog::getOpenFileName(this, tr("Open Text File"), "/home", tr("Text Files (*.txt);;All Files (*)"));
}

QString Editor::getSaveFile(){
    return QFileDialog::getSaveFileName(this, tr("Save Text File"));
}

bool Editor::loadFile(const QString &path){
    if (path.isEmpty())
        return false;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextDocument* doc = new QTextDocument(this);
    doc->setPlainText(QTextStream(&file).readAll());
    setDocument(doc);

    moveCursor(QTextCursor::End);
    
    file_path = path;
    file.close();
    return true;
}

bool Editor::saveFile(){
    if (file_path.isEmpty())
        return saveFileAs(getSaveFile());

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream(&file) << toPlainText();
    file.close();
    return true;
}

bool Editor::saveFileAs(const QString &path){
    if (path.isEmpty())
        return false;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream(&file) << toPlainText();
    file_path = path;
    file.close();
    return true;
}

void Editor::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void Editor::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    
    loadFile(urls.first().toLocalFile());
}

#pragma endregion Input
#pragma region Shortcuts

bool Editor::findText(){
    QString selected = textCursor().selectedText();
    if (selected.isEmpty())
        return false;

    if (!find(selected)){
        moveCursor(QTextCursor::Start);
        return find(selected);
    }
    
    return true;
}

#pragma endregion Shortcuts
#pragma region Background Processes

void Editor::highlightExtraSelection() {
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

#pragma endregion Background Processes