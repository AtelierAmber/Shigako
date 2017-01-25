#include "Layers.h"

Layers::Layers(Engine *parent)
    : ShigakoWidget(parent),
	m_engineParent(parent){	
	int i = 0;
	for(i = 0; i< m_engineParent->getLayers()->size(); ++i)
	{
		m_eyes.push_back(addButton([this, i](){
			m_engineParent->toggleLayer(m_eyes[i]->ID());
		}, Location(0, i, 1, 1), "Assets/eye.png"));
		m_eyes[i]->setID(i);
		
		m_layerButton.push_back(addButton([this, i] () {
			m_engineParent->setlayer(i->ID());
		},Location(0, i, 1, 1), "Assets/layerButton.png"));
	}
	
	m_add = addButton([this, i] () {
				if (m_engineParent->getLayers()->size() > 0 ){
					m_engineParent->newLayer(m_engineParent->getLayers()->at(0).data()->size());
				}
			}, Location(0 ,i ,1 ,1 ), "Assets/Add Layer.png"));
}

Layers::~Layers(){


}


