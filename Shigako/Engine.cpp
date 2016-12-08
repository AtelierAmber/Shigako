#include "Engine.h"

Engine::Engine(QWidget *parent)
    : QWidget(parent){
    m_currentImage = new QImage(200, 200, QImage::Format_ARGB32);

}

Engine::~Engine(){

}
