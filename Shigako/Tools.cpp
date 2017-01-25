#include "Tools.h"

Tools::Tools(Engine *parent)
    : ShigakoWidget(parent),
    m_engineParent(parent){
    r = 0;
    init();
}

Tools::~Tools(){
    delete m_pencil;
}

void Tools::init(){
    m_pencil = addButton([this](){ 
        std::printf("Setting tool to Pencil\n"); 
        m_engineParent->setDrawTool(DrawTool::PENCIL);
    }, Location(0, 0), "Assets/PencilTool.png");
    m_eraser = addButton([this](){ 
        std::printf("Setting tool to Eraser\n");
        m_engineParent->setDrawTool(DrawTool::ERASER);
    }, Location(0, 1), "Assets/EraserTool.png");
    m_fill = addButton([this](){ 
        std::printf("Setting tool to Fill\n"); 
    }, Location(0, 2), "Assets/FillTool.png");
    m_select = addButton([this](){
        std::printf("Setting tool to Select\n");
    }, Location(0, 3), "Assets/SelectTool.png");
    m_text = addButton([this](){ 
        std::printf("Setting tool to Text\n"); 
    }, Location(0, 4), "Assets/TextTool.png");

    m_color = addImage("Assets/Blank.png", Location(0, 5, 3, 3));
    addSlider(0, 255, &r, true, Location(0, 8, 1, 2), [this](){ 
        std::printf("%i\n", r); 
        m_color->setColor(qRgba(r, g, b, 255));
    });
    addSlider(0, 255, &g, true, Location(1, 8, 1, 2), [this](){
        std::printf("%i\n", g);
        m_color->setColor(qRgba(r, g, b, 255));
    });
    addSlider(0, 255, &b, true, Location(2, 8, 1, 2), [this](){
        std::printf("%i\n", b);
        m_color->setColor(qRgba(r, g, b, 255));
    });
}
