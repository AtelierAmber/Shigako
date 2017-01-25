#include <QLabel>

#include "MainWindow.h"
#include "ScribbleArea.h"
#include "Engine.h"

MainWindow::MainWindow(){
    m_engine = new Engine;

    setCentralWidget(m_engine);

    createActions();
    createMenus();

    setWindowTitle(QString::fromStdWString(L"Shigako \u30B7\u30ac\u30B3"));
    resize(500, 500);
}

void MainWindow::closeEvent(QCloseEvent *event){
    if (maybeSave()) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::open(){
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
            m_engine->openImage(fileName);
    }
}

void MainWindow::newI(){
    if (maybeSave()){
        NewImageWizard* wizard = new NewImageWizard(this);
        wizard->show();
    }
}

void MainWindow::save(){
    QAction *action = qobject_cast<QAction *>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MainWindow::penColor(){
    QColor newColor = QColorDialog::getColor(m_engine->penColor(), this, QString("Pen Color"), QColorDialog::ColorDialogOption::ShowAlphaChannel);
    if (newColor.isValid())
        m_engine->setPenColor(newColor);
}

void MainWindow::penWidth(){
     bool ok;
     int newWidth = QInputDialog::getInt(this, tr("Scribble"),
         tr("Select pen width:"),
         m_engine->penWidth(),
         1, 50, 1, &ok);
     if (ok)
         m_engine->setPenWidth(newWidth);
}

void MainWindow::about(){
    QMessageBox::about(this, tr("About Scribble"),
        tr("<p>The <b>Scribble</b> example shows how to use QMainWindow as the "
        "base widget for an application, and how to reimplement some of "
        "QWidget's event handlers to receive the events generated for "
        "the application's widgets:</p><p> We reimplement the mouse event "
        "handlers to facilitate drawing, the paint event handler to "
        "update the application and the resize event handler to optimize "
        "the application's appearance. In addition we reimplement the "
        "close event handler to intercept the close events before "
        "terminating the application.</p><p> The example also demonstrates "
        "how to use QPainter to draw an image in real time, as well as "
        "to repaint widgets.</p>"));
}

bool MainWindow::clear(){
    bool ok = maybeSave();
    if (ok){
        m_engine->clearImage();
    }
    return ok;
}

void MainWindow::engineNewImage(const QSize& size, const QColor& color){
    m_engine->newImage(size, color);
}

void MainWindow::createActions(){
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    foreach(QByteArray format, QImageWriter::supportedImageFormats()) {
        QString text = tr("%1...").arg(QString(format).toUpper());

        QAction *action = new QAction(text, this);
        action->setData(format);
        connect(action, SIGNAL(triggered()), this, SLOT(save()));
        saveAsActs.append(action);
    }

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    newAct = new QAction(tr("&New"), this);
    connect(newAct, SIGNAL(triggered()), this, SLOT(newI()));

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen"), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()),
        this, SLOT(clear()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void MainWindow::createMenus(){
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction *action, saveAsActs)
        saveAsMenu->addAction(action);

    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);
    fileMenu->addAction(newAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MainWindow::maybeSave(){
    if (m_engine->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Shigako"),
            tr("The image has been modified.\n"
            "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard
            | QMessageBox::Cancel);
        if (ret == QMessageBox::Save) {
            return saveFile("png");
        }
        else if (ret == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat){
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
        initialPath,
        tr("%1 Files (*.%2);;All Files (*)")
        .arg(QString::fromLatin1(fileFormat.toUpper()))
        .arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty()) {
        return false;
    }
    else {
        return m_engine->saveImage(fileName, fileFormat.constData());
        return true;
    }
}

ImagePage::ImagePage(QWidget* parent):
    QWizardPage(parent){
    setTitle("New Image");
    resize(251, 221);

    m_gridLayout = new QGridLayout(this);
    m_gridLayout->setContentsMargins(0, 0, 0, 0);
    m_gridLayout->setGeometry(QRect(10, 10, 226, 201));

    m_sizeLabel = new QLabel(this);
    m_gridLayout->addWidget(m_sizeLabel, 0, 0, 1, 3);

    m_widthLabel = new QLabel(this);
    m_gridLayout->addWidget(m_widthLabel, 1, 0, 1, 1);
    m_width = new QLineEdit(this);
    m_gridLayout->addWidget(m_width, 1, 1, 1, 2);

    m_heightLabel = new QLabel(this);
    m_gridLayout->addWidget(m_heightLabel, 2, 0, 1, 1);
    m_height = new QLineEdit(this);
    m_gridLayout->addWidget(m_height, 2, 1, 1, 2);

    m_backgroundColorLabel = new QLabel(this);
    m_gridLayout->addWidget(m_backgroundColorLabel, 4, 0, 1, 3);
    m_backgroundColorBox = new QGroupBox(this);
    m_horizontalLayout = new QHBoxLayout(m_backgroundColorBox);
    m_transparent = new QRadioButton(m_backgroundColorBox);
    m_horizontalLayout->addWidget(m_transparent);

    m_white = new QRadioButton(m_backgroundColorBox);
    m_horizontalLayout->addWidget(m_white);

    m_black = new QRadioButton(m_backgroundColorBox);
    m_horizontalLayout->addWidget(m_black);

    m_gridLayout->addWidget(m_backgroundColorBox, 5, 0, 1, 3);

    m_sizeLabel->setText(QString("--Size:------------------------------------------------"));
    m_widthLabel->setText(QString("Width"));
    m_width->setText(QString());
    m_width->setToolTip(QString("pixels"));

    m_heightLabel->setText(QString(" Height:"));
    m_height->setText(QString());
    m_height->setToolTip(QString("pixels"));

    m_backgroundColorLabel->setText(QString("--Background Color:--------------------------------"));
    m_backgroundColorBox->setTitle(QString());
    m_transparent->setText(QString("Transparent"));
    m_white->setText(QString("White"));
    m_black->setText(QString("Black"));

    registerField("iWidth", m_width);
    registerField("iHeight", m_height);
    registerField("iBackTrans", m_transparent);
    registerField("iBackWhite", m_white);
    registerField("iBackBlack", m_black);

    setLayout(m_gridLayout);
}

NewImageWizard::NewImageWizard(MainWindow* mainWindow){
    setWindowTitle("New Image");
    setOptions(QWizard::CancelButtonOnLeft);
    m_page = new ImagePage(this);
    addPage(m_page);
    m_parentWindow = mainWindow;
}

void NewImageWizard::accept(){
    iW = field("iWidth").toInt();
    iH = field("iHeight").toInt();
    if (field("iBackTrans").toBool()){
        iBackg = qRgba(0, 0, 0, 0);
    }
    else if (field("iBackWhite").toBool()){
        iBackg = qRgba(255, 255, 255, 255);
    }
    else if (field("iBackBlack").toBool()){
        iBackg = qRgba(0, 0, 0, 255);
    }
    isDone = true;
    m_parentWindow->engineNewImage(QSize(iW, iH), iBackg);
    QDialog::accept();
    delete this;
}