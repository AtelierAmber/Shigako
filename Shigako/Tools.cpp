#include "Tools.h"

Tools::Tools(Engine *parent)
    : ShigakoWidget(parent),
    m_engineParent(parent){
    init();
}

Tools::~Tools(){
    delete m_pencil;
}

void Tools::init(){
    m_pencil = addButton([this](){ std::printf("Pressed Button!"); }, Location(0, 0));
}
