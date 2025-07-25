#include "main_window.h"

#include <QVBoxLayout>
#include <QShortcut>
#include <QKeySequence>
#include <QFileInfo>
#include <QSettings>

#pragma region Constructor

MainWindow::MainWindow(QWidget *parent, int argc, char *argv[]) : QMainWindow(parent) {
    //Window setup
    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);
    setAcceptDrops(true);
    setWindowTitle(tr("2Write"));
    setStyleSheet(QString("background-color: %1").arg(qApp->palette().color(QPalette::Base).name()));

    //Window contents setup
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(12, 12, 12, 12);

    textEdit = new TextEdit(centralWidget);
    centralLayout->addWidget(textEdit);

    setCentralWidget(centralWidget);

    //Shortcut binding
    connect( new QShortcut(QKeySequence::Find, this), &QShortcut::activated, textEdit, &TextEdit::findText );
    connect( new QShortcut(QKeySequence::Open, this), &QShortcut::activated, textEdit, &TextEdit::importFile );
    connect( new QShortcut(QKeySequence::Save, this), &QShortcut::activated, textEdit, &TextEdit::saveFile );
    connect( new QShortcut(QKeySequence::SaveAs, this), &QShortcut::activated, textEdit, &TextEdit::saveFileAs );
    connect( new QShortcut(QKeySequence::ZoomIn, this), &QShortcut::activated, textEdit, &TextEdit::zoomIn );
    connect( new QShortcut(QKeySequence::ZoomOut, this), &QShortcut::activated, textEdit, &TextEdit::zoomOut );

    //Argument parsing, and loading provided files
    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        if (QFileInfo::exists(filePath))
            textEdit->loadFile(filePath);
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    QSettings settings("2write");

    const QString plainText = textEdit->toPlainText();
    const QString filePath = plainText.isEmpty() ? "" : textEdit->getFilePath();

    settings.setValue("text", plainText);
    settings.setValue("filePath", filePath);

    event->accept();
}

#pragma endregion Constructor
