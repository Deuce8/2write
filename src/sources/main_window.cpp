#include "main_window.h"

#include <QVBoxLayout>
#include <QShortcut>
#include <QMimeData>
#include <QKeySequence>
#include <QFileInfo>
#include <QSettings>
#include <QFileDialog>

#pragma region Constructor

MainWindow::MainWindow(QWidget *parent, int argc, char *argv[]) : QMainWindow(parent) {
    //Window setup
    setFocusPolicy(Qt::ClickFocus);
    setWindowFlags(Qt::Window);
    setAcceptDrops(true);
    setWindowTitle(tr("2Write"));
    
    //Window contents setup
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *centralLayout = new QVBoxLayout(centralWidget);
    centralLayout->setContentsMargins(12, 12, 12, 12);

    textEdit = new TextEdit(centralWidget);

    findToolbar = new FindToolbar(centralWidget, textEdit);
    findToolbar->hide();

    centralLayout->addWidget(textEdit);
    centralLayout->addWidget(findToolbar);

    setCentralWidget(centralWidget);

    //Signal & Slot binding
    connect( new QShortcut(QKeySequence::Find, this), &QShortcut::activated, this, [this] {findToolbar->setVisible(!findToolbar->isVisible()); });
    
    connect( new QShortcut(QKeySequence::Open, this), &QShortcut::activated, this, &MainWindow::importFile );
    connect( new QShortcut(QKeySequence::Save, this), &QShortcut::activated, textEdit, &TextEdit::saveFile );
    connect( new QShortcut(QKeySequence::SaveAs, this), &QShortcut::activated, textEdit, &TextEdit::saveFileAs );
    connect( new QShortcut(QKeySequence::ZoomIn, this), &QShortcut::activated, textEdit, &TextEdit::zoomIn );
    connect( new QShortcut(QKeySequence::ZoomOut, this), &QShortcut::activated, textEdit, &TextEdit::zoomOut );

    connect( this, &MainWindow::fileLoaded, textEdit, &TextEdit::loadFile);

    //Argument parsing, and loading provided files
    if (argc > 1) {
        QString filePath = QString::fromLocal8Bit(argv[1]);
        if (QFileInfo::exists(filePath))
            emit fileLoaded(filePath);
    }
}

#pragma endregion Constructor
#pragma region Private Slots

void MainWindow::importFile(){
    emit fileLoaded(QFileDialog::getOpenFileName(this, tr("Open Text File"), textEdit->getFilePath(), tr("Text Files (*.txt);;All Files (*)")));
}

#pragma endregion Private Slots
#pragma region Protected

void MainWindow::closeEvent(QCloseEvent *event){
    QSettings settings("Deuce8", "2write");

    const bool save = settings.value("saveSession", true).toBool();
    const QString plainText = !save ? "" : textEdit->toPlainText();
    const QString filePath = plainText.isEmpty() || !save ? "" : textEdit->getFilePath();

    settings.setValue("text", plainText);
    settings.setValue("filePath", filePath);
    settings.setValue("saveSession", save);

    event->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QList<QUrl> urls = event->mimeData()->urls();
    if(urls.isEmpty())
        return;
    
    emit fileLoaded(urls.first().toLocalFile());
}

#pragma endregion Protected