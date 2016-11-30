#include "Shigako.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Shigako w;
    w.show();
    return a.exec();
}
