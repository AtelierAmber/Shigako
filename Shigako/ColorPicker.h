#pragma once

#include <QWidget>
#include "Engine.h"

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


class ColorPicker : public ShigakoWidget{
    Q_OBJECT

public:
    ColorPicker(Engine *parent);
    ~ColorPicker(); 
	private:
	int r = 0;
	int g = 0;
	int b = 0;
	ShigakoImage* color;
	
private:
	Engine* m_engineParent;
};
