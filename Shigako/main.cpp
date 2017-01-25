#include "MainWindow.h"
#include <QtWidgets/QApplication>
#include <sstream>
#include <fstream>

#ifdef _DEBUG
#include <vld.h>
#endif

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    std::ifstream in;
    in.open("DarkTheme.stylesheet");
    if (in.fail()){
        throw std::exception();
    }
    std::stringstream sstr;
    sstr << in.rdbuf();
    a.setStyleSheet(QString::fromStdString(sstr.str()));
    MainWindow w;
    w.show();
    return a.exec();
}
