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
    m_pencil = addButton([this](){ std::printf("Pressed Button!\n"); }, Location(0, 0), "Assets/PencilTool.png");
    m_eraser = addButton([this](){ std::printf("Pressed Button!\n"); }, Location(0, 1), "Assets/Drawing tool.png");
    m_eraser->setButtonIcon("Assets/EraserTool.png");
    m_fill = addButton([this](){ std::printf("Pressed Button!\n"); }, Location(0, 2), "Assets/FillTool.png");
    m_select = addButton([this](){ std::printf("Pressed Button!\n"); }, Location(0, 3), "Assets/SelectTool.png");
    m_text = addButton([this](){ std::printf("Pressed Button!\n"); }, Location(0, 4), "Assets/TextTool.png");
}
