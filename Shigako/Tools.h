#pragma once

#include "Engine.h"

class Tools : public ShigakoWidget
{
    Q_OBJECT

public:
    Tools(Engine *parent);
    ~Tools();
        
private:
    void init();

    Engine* m_engineParent;
    ShigakoButton* m_pencil;
    ShigakoButton* m_eraser;
    ShigakoButton* m_fill;
    ShigakoButton* m_select;
    ShigakoButton* m_text;
};
