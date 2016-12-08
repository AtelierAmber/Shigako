#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDir>
#include <QMainWindow>
#include <QStringList>

#include "Engine.h"

QT_BEGIN_NAMESPACE
class QAction;
class QActionGroup;
class QMenu;
class QScrollArea;
QT_END_NAMESPACE
class PaintArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

    private slots:
    void about();
    void open();
    bool saveAs();
    /*void brushColor();
    void brushWidth();
    void changeBrush();
    void insertShape();
    void applyFilter();
    
    void aboutPlugins();*/

private:

    void createActions();
    void createMenus();
    /*void loadPlugins();
    void populateMenus(QObject *plugin);
    void addToMenu(QObject *plugin, const QStringList &texts, QMenu *menu,
        const char *member, QActionGroup *actionGroup = 0);*/

    //PaintArea *paintArea;
    //QScrollArea *scrollArea;
    //QDir pluginsDir;
    //QStringList pluginFileNames;

    Engine* m_engine;

    QMenu *fileMenu;
    QMenu *helpMenu;
    QAction *openAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *aboutAct;
    QAction *aboutQtAct;
};

#endif
