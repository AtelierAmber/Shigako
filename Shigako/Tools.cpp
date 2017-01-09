#include "Tools.h"

Tools::Tools(Engine *parent)
    : ShigakoWidget(parent),
    m_engineParent(parent){
    init();
}

Tools::~Tools(){

}

void Tools::init(){
    addButton([this](){ std::printf("Pressed Button!"); }, vec2(16, 16), Location::MIDDLE | Location::TOP);
    addButton([this](){ std::printf("Pressed Button2!"); }, vec2(16, 16), Location::MIDDLE | Location::TOP);
    addButton([this](){ std::printf("Pressed Button3!"); }, vec2(16, 16), Location::MIDDLE | Location::TOP);
}
