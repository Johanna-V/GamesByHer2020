#pragma once 

#include "sfml-engine/scene.h"
#include <SFML/Audio.hpp>


class TitleScene : public gbh::Scene
{
protected:
	void onInitializeScene() override;

	virtual void onUpdate(double deltaTime);
	/*We are overriding that in the base class, Scene, onMouseEvent() does not do anything, 
	it just exists. For this sub class, TitleScene, we want onMouseEvent() to actually do something
	(what it will do is defined in title_scene.cpp)*/
	virtual void onMouseEvent(sf::Event& event) override;
	virtual void onShowScene();
	virtual void onHideScene();

private:
	sf::Font m_robotoFont;
	sf::Music m_titleMusic;
};
