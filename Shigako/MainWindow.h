#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QtWidgets>
#include <QMainWindow>
#include <QWizard>

class Engine;

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    void engineNewImage(const QSize& size, const QColor& color);

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    private slots:
    void open();
    void newI();
    void save();
    void penColor();
    void penWidth();
    void about();
    bool clear();

private:
    void createActions();
    void createMenus();
    bool maybeSave();
    bool saveFile(const QByteArray &fileFormat);

    Engine* m_engine;

    QMenu* saveAsMenu;
    QMenu* fileMenu;
    QMenu* optionMenu;
    QMenu* helpMenu;

    QAction* openAct;
    QAction* newAct;
    QList<QAction* > saveAsActs;
    QAction* exitAct;
    QAction* penColorAct;
    QAction* penWidthAct;
    QAction* clearScreenAct;
    QAction* aboutAct;
    QAction* aboutQtAct;
};

class ImagePage :
    public QWizardPage{
    Q_OBJECT
public:
    ImagePage(QWidget* parent = 0);

private:
    QGridLayout* m_gridLayout;
    QLabel* m_sizeLabel;
    QLabel* m_widthLabel;
    QLineEdit* m_width;
    QLabel* m_heightLabel;
    QLineEdit* m_height;
    QLabel* m_backgroundColorLabel;
    QGroupBox* m_backgroundColorBox;
    QHBoxLayout* m_horizontalLayout;
    QRadioButton* m_transparent;
    QRadioButton* m_white;
    QRadioButton* m_black;
};

class NewImageWizard :
    public QWizard{
    Q_OBJECT
public:
    NewImageWizard(MainWindow* mainWindow);

    void accept() override;

    int iW;
    int iH;
    QColor iBackg;
    bool isDone = false;

private:
    MainWindow* m_parentWindow;

    ImagePage* m_page;
};

#endif