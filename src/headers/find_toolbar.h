#ifndef FIND_TOOLBAR_H
#define FIND_TOOLBAR_H

#include <QToolBar>

class FindToolbar : public QToolBar {
    Q_OBJECT

public:
    FindToolbar(QWidget *parent = nullptr, int argc = 1, char *argv[] = nullptr);

};

#endif