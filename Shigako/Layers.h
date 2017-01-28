#pragma once

#include <QWidget>
#include <vector>
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

class Layers : public ShigakoWidget{
    Q_OBJECT

public:
    Layers(Engine *parent);
    ~Layers();
	
	std::vector<ShigakoButton*> m_eyes;
	ShigakoButton* m_add;
	std::vector<ShigakoButton* > m_layerButtons;
	ShigakoButton* m_eye;

private:
		Engine* m_engineParent;
};
