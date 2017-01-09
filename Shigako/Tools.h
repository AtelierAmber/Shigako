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
};
