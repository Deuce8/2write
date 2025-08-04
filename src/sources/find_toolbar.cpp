#include "find_toolbar.h"

#include <QHBoxLayout>
#include <QShortcut>
#include <QKeySequence>

#pragma region Constructor

// Called when the find toolbar is created
FindToolbar::FindToolbar(QWidget *parent, TextEdit *textEdit) : QToolBar(parent) {
    // Window setup
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setOrientation(Qt::Horizontal);
    setStyleSheet("QToolBar{spacing:0px;}");

    // Content Setup
    lineEdit = new QLineEdit(this);
    lineEdit->setFont(QFont("Hack", 10));
    lineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineEdit->setPlaceholderText("Find");
    lineEdit->setContentsMargins(0, 0, 0, 0);
    connect(lineEdit, &QLineEdit::textChanged, textEdit, &TextEdit::setFind);
    addWidget(lineEdit);

    addSeparator();

    prevSelection = new QAction(this);
    prevSelection->setIcon(QIcon::fromTheme("go-up"));
    prevSelection->setShortcut(QKeySequence(Qt::Key_F3 | Qt::SHIFT));
    connect(prevSelection, &QAction::triggered, textEdit, &TextEdit::findPrev);
    connect(prevSelection, &QAction::triggered, textEdit, static_cast<void (QWidget::*)()>(&QWidget::setFocus));
    addAction(prevSelection);

    nextSelection = new QAction(this);
    nextSelection->setIcon(QIcon::fromTheme("go-down"));
    nextSelection->setShortcut(QKeySequence(Qt::Key_F3));
    connect(nextSelection, &QAction::triggered, textEdit, &TextEdit::findNext);
    connect(nextSelection, &QAction::triggered, textEdit, static_cast<void (QWidget::*)()>(&QWidget::setFocus));
    addAction(nextSelection);

    // The layout will end up being: [ lineEdit | prevSelection  nextSelection ]
}

#pragma endregion Constructor
#pragma region Protected

// Called when the find toolbar is shown (This happens via Ctrl+F in main_window.cpp)
void FindToolbar::showEvent(QShowEvent *event) {
    lineEdit->setFocus();
}

#pragma endregion Protected