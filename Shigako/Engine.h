#pragma once

#include <QWidget>
#include <QImage>
#include <QPaintEngine>

#include <map>

class Engine : 
    public QWidget{
    Q_OBJECT

public:
    Engine(QWidget *parent);
    ~Engine();



    private slots:


private:
    //Brush ID, Brush
    std::map<int, QBrush> m_brushes;

    QImage* m_currentImage;
    QPainter m_painter;
    QBrush m_currentBrush;
};
