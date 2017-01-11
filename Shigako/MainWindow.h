#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QMainWindow>

class Engine;

QT_BEGIN_NAMESPACE
class QLabel;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

    private slots:
    void open();
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
    QList<QAction* > saveAsActs;
    QAction* exitAct;
    QAction* penColorAct;
    QAction* penWidthAct;
    QAction* clearScreenAct;
    QAction* aboutAct;
    QAction* aboutQtAct;
};

#endif