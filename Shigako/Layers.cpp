#include "Layers.h"

Layers::Layers(Engine *parent)
    : ShigakoWidget(parent),
	m_engineParent(parent){
	m_add=addButton([this]) () {m_engineparent->addLayer(i); }, "Assets/Add.png");
	m_eye=addButton([this](){m_engineParent->togglelayer(i); }, "Assets/Eye.png");
	
    m_layer=addButton([this]) () {m_engineparent->setLayer(i); }, ;
	 addImage(Layer.getImage(), Location(i,2);
	 addLabel(Layer.name, Location(i,3));
	 //array=m_eyes
	 
	 for(int i =0; i< m_engineParent.getlayers()->size(); ++i)
	 {
		 m_eyes.push_back(addButton([this](){
			 m_engineParent->togglelayer(m_eyes[i]);
		}, Location(0, i, 1, 1), "Assets/eye.png");
		m_eyes[i]->setID(i);
		 
}

Layers::ShigakoLayer
Layers::~Layers(){


}


