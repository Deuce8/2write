#include "find_toolbar.h"

#include <QHBoxLayout>
#include <QShortcut>
#include <QKeySequence>
#include <QAction>
#include <QLineEdit>

#pragma region Constructor

FindToolbar::FindToolbar(QWidget *parent, int argc, char *argv[]) : QToolBar(parent) {
    //Window setup
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setOrientation(Qt::Horizontal);
    setStyleSheet("QToolBar{spacing:0px;}");

    //Content Setup
    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setFont(QFont("Hack", 10));
    lineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineEdit->setPlaceholderText("Find");
    lineEdit->setContentsMargins(0, 0, 0, 0);

    QAction *nextSelection = new QAction(this);
    nextSelection->setIcon(QIcon::fromTheme("go-down"));
    nextSelection->setShortcut(QKeySequence::InsertParagraphSeparator);

    QAction *prevSelection = new QAction(this);
    prevSelection->setIcon(QIcon::fromTheme("go-up"));
    prevSelection->setShortcut(QKeySequence::InsertLineSeparator);
    //connect(prevSelection, QAction::triggered)

    addWidget(lineEdit);
    addSeparator();
    addAction(prevSelection);
    addAction(nextSelection);
}

#pragma endregion Constructor