#include "editor.h"

#include <QFile>
#include <QMimeData>
#include <QPalette>
#include <QShortcut>
#include <QKeySequence>
#include <QFileDialog>
#include <Qt>
#include <QFont>
#include <QSettings>

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
    setPlainText(QSettings("Brian Bohn II", "2write").value("text", "").toString());
    moveCursor(QTextCursor::End);

    connect(new QShortcut(QKeySequence(QKeySequence::Open), this), &QShortcut::activated, this, &Editor::importFile );
    connect(new QShortcut(QKeySequence(QKeySequence::Save), this), &QShortcut::activated, this, &Editor::saveFile );
    connect(new QShortcut(QKeySequence(QKeySequence::SaveAs), this), &QShortcut::activated, this, &Editor::saveFileAs );
    connect(new QShortcut(QKeySequence(QKeySequence::Find), this), &QShortcut::activated, this, &Editor::findText );
    connect(new QShortcut(QKeySequence(QKeySequence::ZoomIn), this), &QShortcut::activated, this, [this]() { this->zoomIn(1); });
    connect(new QShortcut(QKeySequence(QKeySequence::ZoomOut), this), &QShortcut::activated, this, [this]() { this->zoomOut(1); });
    connect(this, &QTextEdit::selectionChanged, this, &Editor::highlightExtraSelection );
}

#pragma endregion Constructor
#pragma region Public Functions

void Editor::loadFile(const QString &path){
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextDocument* doc = new QTextDocument(this);
    doc->setPlainText(QTextStream(&file).readAll());
    setDocument(doc);

    moveCursor(QTextCursor::End);
    
    file_path = path;
    file.close();
}

#pragma endregion Public Functions
#pragma region Qt Events

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

void Editor::closeEvent(QCloseEvent *event){
    QSettings("Brian Bohn II", "2write").setValue("text", toPlainText());

    event->accept();
}

#pragma endregion Qt Events
#pragma region Private Slots

void Editor::findText(){
    QString selected = textCursor().selectedText();
    if (selected.isEmpty())
        return;

    if (!find(selected))
        return;

    moveCursor(QTextCursor::Start);
    find(selected);
}

void Editor::importFile(){
    loadFile(QFileDialog::getOpenFileName(this, tr("Open Text File"), "/home", tr("Text Files (*.txt);;All Files (*)")));
}

void Editor::saveFile(){
    if (file_path.isEmpty()){
        saveFileAs();
        return;
    }

    QFile file(file_path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream(&file) << toPlainText();
    file.close();
}

void Editor::saveFileAs(){
    const QString path = QFileDialog::getSaveFileName(this, tr("Save Text File"));

    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream(&file) << toPlainText();
    file_path = path;
    file.close();
}

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

#pragma endregion Private Slots