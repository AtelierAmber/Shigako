#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Shigako.h"

class Shigako : public QMainWindow
{
    Q_OBJECT

public:
    Shigako(QWidget *parent = Q_NULLPTR);

private:
    Ui::ShigakoClass ui;
};
