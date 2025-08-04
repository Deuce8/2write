#include "find_toolbar.h"

#include <QHBoxLayout>
#include <QShortcut>
#include <QKeySequence>
#include <QAction>

#pragma region Constructor

FindToolbar::FindToolbar(QWidget *parent, TextEdit *textEdit) : QToolBar(parent) {
    //Window setup
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    setOrientation(Qt::Horizontal);
    setStyleSheet("QToolBar{spacing:0px;}");

    //Content Setup
    lineEdit = new QLineEdit(this);
    lineEdit->setFont(QFont("Hack", 10));
    lineEdit->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    lineEdit->setPlaceholderText("Find");
    lineEdit->setContentsMargins(0, 0, 0, 0);
    connect(lineEdit, &QLineEdit::textChanged, textEdit, &TextEdit::setFind);

    QAction *nextSelection = new QAction(this);
    nextSelection->setIcon(QIcon::fromTheme("go-down"));
    nextSelection->setShortcut(QKeySequence(Qt::Key_F3));
    connect(nextSelection, &QAction::triggered, textEdit, &TextEdit::findNext);
    connect(nextSelection, &QAction::triggered, textEdit, static_cast<void (QWidget::*)()>(&QWidget::setFocus));

    QAction *prevSelection = new QAction(this);
    prevSelection->setIcon(QIcon::fromTheme("go-up"));
    prevSelection->setShortcut(QKeySequence(Qt::Key_F3 | Qt::SHIFT));
    connect(prevSelection, &QAction::triggered, textEdit, &TextEdit::findPrev);
    connect(prevSelection, &QAction::triggered, textEdit, static_cast<void (QWidget::*)()>(&QWidget::setFocus));

    addWidget(lineEdit);
    addSeparator();
    addAction(prevSelection);
    addAction(nextSelection);
}

#pragma endregion Constructor
#pragma region Protected

void FindToolbar::showEvent(QShowEvent *event) {
    lineEdit->setFocus();
}

#pragma endregion Protected