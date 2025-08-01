#ifndef FIND_TOOLBAR_H
#define FIND_TOOLBAR_H

#include <QToolBar>

#include "text_edit.h"

class FindToolbar : public QToolBar {
    Q_OBJECT

public:
    FindToolbar(QWidget *parent = nullptr, TextEdit *textEdit = nullptr);

};

#endif