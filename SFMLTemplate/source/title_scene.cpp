#include "title_scene.h"

#include <iostream>

#include "sfml-engine/game.h"
#include "sfml-engine/shapenode.h"
#include "sfml-engine/spritenode.h"
#include "sfml-engine/textnode.h"

const std::string kTitleMusic = "../assets/music/titlescreen.ogg";
const std::string kTitleScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kTitleScreenBackground = "../assets/gfx/space-background-01.png";
//the old background const std::string kTitleScreenBackground = "../assets/gfx/starfield-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";



void TitleScene::onInitializeScene()
{
	m_robotoFont.loadFromFile(kTitleScreenFont);
	m_titleMusic.openFromFile(kTitleMusic);
	m_titleMusic.setVolume(25);
	

	std::shared_ptr<gbh::SpriteNode> spriteNode = std::make_shared<gbh::SpriteNode>(kTitleScreenBackground);
	spriteNode->setName("Background");
	addChild(spriteNode);

	/*A rotating asteroid ornamenting the title scene*/
	m_asteroid3 = std::make_shared<gbh::SpriteNode>("../assets/gfx/asteroid-medium-03.png");
	m_asteroid3->setPosition(1000, 200);
	m_asteroid3->setOrigin(0.5f, 0.5f);
	addChild(m_asteroid3);

	std::shared_ptr<gbh::TextNode> textNode = std::make_shared<gbh::TextNode>("Space Race", m_robotoFont, 60);
    textNode->setOrigin(0.5f, 0.5f);
	textNode->setPosition(640, 100);
	textNode->setName("Title");
	addChild(textNode);

	std::shared_ptr<gbh::SpriteNode> ship = std::make_shared<gbh::SpriteNode>(kPlayerShip);
	ship->setPosition(620, 300);
	addChild(ship);

	/*Why are there two nodes for the start button?
	Ah there is one for the rectangle shape and one for the text on top of it*/
	std::shared_ptr<gbh::ShapeNode> startButton = std::make_shared<gbh::ShapeNode>(sf::RectangleShape(sf::Vector2f(1280, 60)));
	startButton->setPosition(640, 560);
    startButton->setOrigin(0.5f, 0.5f);
	startButton->getShape()->setFillColor(sf::Color(0, 255, 0, 64));
	startButton->setName("StartButton");
	addChild(startButton);

	std::shared_ptr<gbh::TextNode> startNode = std::make_shared<gbh::TextNode>("Start Game", m_robotoFont);
	/*This position turned out outside of the screen after setOrigin was changed to 0.5f everywhere, 
	so I changed the origin back for this text since I think it makes more sense for text in a corner. 
	Text will change its position depending on how many characters it contains if the origin is set to the middle of it, and that is risky in a corner*/
	startNode->setPosition(20, 10);
	startNode->setOrigin(0.0f, 0.0f);
	startNode->setName("StartButton");
	startButton->addChild(startNode);
}


void TitleScene::onUpdate(double deltaTime)
{
    std::shared_ptr<gbh::Node> titleNode = getFirstChildWithName("Title", false);
    
    if (titleNode)
    {
        titleNode->move(0, 50.f * (float)deltaTime);
    }

	/*Rotates the asteroid*/
	const float degreesPerSecond = 45.0f;
	m_asteroid3->rotate(degreesPerSecond * deltaTime);
}


void TitleScene::onMouseEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed)
	{
		std::shared_ptr<gbh::Node> node = this->getNodeAtViewPoint((float)event.mouseButton.x, (float)event.mouseButton.y);

		if (node->getName()=="StartButton")
		{
			//If clicking on some part of the start button, go to the scene maingame
			gbh::Game::getInstance().changeScene("maingame");
			
			//Old code that used to be here - std::cout << "Clicked On: " << node->getName() << "\n";
		}
	}
}


void TitleScene::onShowScene()
{
	m_titleMusic.play();
}


void TitleScene::onHideScene()
{
	m_titleMusic.stop();
}
