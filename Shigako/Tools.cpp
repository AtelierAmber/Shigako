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
    m_paint = addButton([this](){
        std::printf("Setting tool to Paint Brush\n");
        m_engineParent->setDrawTool(DrawTool::BRUSH);
    }, Location(0, 0), "Assets/Paintbrush.png");
    m_pencil = addButton([this](){ 
        std::printf("Setting tool to Pencil\n"); 
        m_engineParent->setDrawTool(DrawTool::PENCIL);
    }, Location(0, 1), "Assets/PencilTool.png");
    m_eraser = addButton([this](){ 
        std::printf("Setting tool to Eraser\n");
        m_engineParent->setDrawTool(DrawTool::ERASER);
    }, Location(0, 2), "Assets/EraserTool.png");
    m_fill = addButton([this](){ 
        std::printf("Setting tool to Fill\n"); 
        m_engineParent->setDrawTool(DrawTool::FILL);
    }, Location(0, 3), "Assets/FillTool.png");
    /*m_select = addButton([this](){
        std::printf("This tool has not been implemented yet\n");
    }, Location(0, 4), "Assets/SelectTool.png");
    m_text = addButton([this](){ 
        std::printf("This tool has not been implemented yetn"); 
    }, Location(0, 5), "Assets/TextTool.png");*/
}
