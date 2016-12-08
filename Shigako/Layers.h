#pragma once

#include <QObject>

//#include "Engine/Engine.h"

//Available functions
/*this->addButton(ButtonPic.png, Location, Text, vec2 size)
*this->addImage(Image.png, Location, vec2 size)
*this->addSlider(Line.png, Slide.png, Location, vec2 size, min, max)
*
*Location is defined as either LocationLayout::TOP, ::MID, ::LEFT, ::RIGHT, ::BOT
*or a vec2 of x, y in relation to the top left of the widget box
*
*vec2 is defined as vec2(x, y); It's just a x and y position
*/

class Layers : public QObject
{
    Q_OBJECT

public:
    Layers(QObject *parent);
    ~Layers();
};
