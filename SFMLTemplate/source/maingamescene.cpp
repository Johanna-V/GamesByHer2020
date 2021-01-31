#include "maingamescene.h"
#include <iostream>

const std::string cMainbackground = "../assets/gfx/space-background-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";

/*Overriding the function that is empty in the base class "Scene", 
telling it to print some text instead of doing nothing, for the MainGameScene class*/
void MainGameScene::onInitializeScene() {

	//Draw the background sprite
	/*create the node and point to the string for the file)
	(point to a string as a name for the node)
	(actually draw the node)*/
	std::shared_ptr<gbh::SpriteNode> background = std::make_shared<gbh::SpriteNode>(cMainbackground);
	background->setName("background");
	addChild(background);

	//Turns on physics and sets gravity to nothing because we are in space!
	createPhysicsWorld(sf::Vector2f(0.0f, 0.0f));

	//This is to see the edges of physics bodies, e.g., the boundary below, so we know if it works or not
	setDrawPhysicsDebug(true);

	//Boundary for physics bodies, that is almost as big as the screen (1270, 710) and centered.
	std::shared_ptr<gbh::Node> boundary = std::make_shared<gbh::Node>();
	boundary->setPhysicsBody(getPhysicsWorld()->createEdgeBox(sf::Vector2f(1270, 710)));
	boundary->getPhysicsBody()->setType(gbh::PhysicsBodyType::Static);
	boundary->setPosition(1280.0f / 2.0f, 720.0f / 2.0f);
	addChild(boundary);

	/*Draw the playership sprite (old sprite without physics)
	m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
	m_playerShip->setName("playership");
	m_playerShip->setOrigin(0.5f, 0.5f);
	m_playerShip->setPosition(640, 360);
	addChild(m_playerShip);*/

	//defining a size variable for the upcoming ship body
	const sf::Vector2f shipSize = sf::Vector2f(80.0f, 120.0f);

	m_playerShip = std::make_shared<gbh::SpriteNode>(kPlayerShip);
	m_playerShip->setPosition(640, 360);
	//instead of setOrigin() that we can not use for physics bodies:
	m_playerShip->setScale(0.5f, 0.5f); 
	/*attaches the ship to a physicsbody and creates a box with the size variable we made for the ship:
	Not sure what the last '0.5f' does. Somehow setting orgin/scale? 'Physicsmaterial'? Some dark magic of math?*/
	m_playerShip->setPhysicsBody(getPhysicsWorld()->createBox(shipSize * 0.5f));
	m_playerShip->getPhysicsBody()->setLinearDamping(2.0f);
	m_playerShip->getPhysicsBody()->setFixedRotation(true);
	addChild(m_playerShip);
}