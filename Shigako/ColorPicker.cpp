#include "ColorPicker.h"

ColorPicker::ColorPicker(Engine *parent)
    : ShigakoWidget(parent),
	m_engineParent(parent){
		
	slider = addSlider(0, 255, r, Location);
	color=addImage("Assets/Blank.png",Location(0,0,3,3));
	addSlider(0,255,&r,true, Location(0,3,1,1),[this](){
		color->setColor(qRgba(r,g,b,255));
		m_engineParent->setPenColor(qRgba(r,g,b,255));
	});
	
	addSlider(0,255,&g,true, Location(1,3,1,1),[this](){
		color->setColor(qRgba(r,g,b,255));
		m_engineParent->setPenColor(qRgba(r,g,b,255));
	});
	
	addSlider(0,255,&b,true, Location(2,3,1,1),[this](){
		color->setColor(qRgba(r,g,b,255));
		m_engineParent->setPenColor(qRgba(r,g,b,255));
	});
}
ColorPicker::~ColorPicker(){

}
