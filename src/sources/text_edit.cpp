#include "text_edit.h"

#include <QFile>
#include <QFileDialog>
#include <QSettings>

#pragma region Constructor

// Called when the text edit is created
TextEdit::TextEdit(QWidget *parent) : QTextEdit(parent) {
    setFont(QFont("Hack", 10));
    setLineWrapMode(QTextEdit::NoWrap);
    moveCursor(QTextCursor::End);
    
    QSettings settings("Deuce8", "2write");

    setPlainText(settings.value("text", "").toString());
    filePath = settings.value("filePath", "/home").toString();

    connect(this, &QTextEdit::selectionChanged, this, &TextEdit::highlightExtraSelection);
}

#pragma endregion Constructor
#pragma region Public Slots

// Find the next occurrence of 'toFind'
void TextEdit::findNext(){
    if (toFind.isEmpty())
        return;

    if (find(toFind))
        return;

    moveCursor(QTextCursor::Start);
    find(toFind);
}

// Find the previous occurrence of 'toFind'
void TextEdit::findPrev(){
    if (toFind.isEmpty())
        return;

    if (find(toFind, QTextDocument::FindBackward))
        return;

    moveCursor(QTextCursor::End);
    find(toFind, QTextDocument::FindBackward);
}

// Save the current file
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

// Save the file with a dialog
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

// Zoom in by 1 step    (This is a workaround for the fact that QTextEdit::zoomIn() requires a parameter, and we want to use it without parameters.)
void TextEdit::zoomIn(){
    QTextEdit::zoomIn(1);
    zoom += 1;
}

// Zoom out by 1 step   (This is a workaround for the fact that QTextEdit::zoomOut() requires a parameter, and we want to use it without parameters.)
void TextEdit::zoomOut(){
    QTextEdit::zoomOut(1);
    zoom -= 1;
}

// Load a file into the text edit
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
}

#pragma endregion Public Slots
#pragma region Private Slots

// Highlight all items matching the current selection, called when the selection changes
void TextEdit::highlightExtraSelection() {

    // Return if the selection is empty
    const QString current_selection = textCursor().selectedText();
    if (current_selection.isEmpty()){
        setExtraSelections({});
        return;
    }

    // Get the cursor, and set it to the start of the document
    QTextDocument *doc = document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::Start);

    // Create a format for the highlighted text
    QTextCharFormat format;
    format.setBackground(palette().highlight().color().lighter(64));
    format.setForeground(palette().highlightedText().color());

    // Create and populate a list of extra selections with all occurrences of the current selection
    QList<QTextEdit::ExtraSelection> selections;

    while (true) {
        if (cursor.isNull())
            break;

        cursor = doc->find(current_selection, cursor);
        selections.append(QTextEdit::ExtraSelection{cursor, format});
        cursor.setPosition(cursor.selectionEnd());
    }

    // Set the extra selections to the text edit
    setExtraSelections(selections);
}

#pragma endregion Private Slots