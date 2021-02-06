#include "maingamescene.h"
#include "sfml-engine/mathutils.h"
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
	//The ship was turned the wrong way originally so here we point the front up
	m_playerShip->rotate(180);
	addChild(m_playerShip);
}

/*Delta time describes the time difference between the previous frame that was drawn
and the current frame, and makes sure that movement speed will not change depending 
on what fps the current player's display can handle. This is how we make things tick 
with a more "reliable" framerate, to move something around the screen.*/
void MainGameScene::onUpdate(double deltaTime)
{
	static const float acceleration = 2000.0f;

	sf::Vector2f moveDirection;
	const float degreesPerSecond = 85.0f;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		//Old: moveDirection.y -= 1.0f;
		/*New: to get car-like movement, use the forwardVector() function from node.cpp, 
		which uses sine and cosine to translate the angle/rotation of the ship into 
		two forces pushing the ship forward through the x-y grid*/
		moveDirection = m_playerShip->forwardVector();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		//Old: moveDirection.y += 1.0f;
		//New: to get car-like movement:
		moveDirection = m_playerShip->backwardVector();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		//Old: moveDirection.x -= 1.0f;
		m_playerShip->rotate(-(degreesPerSecond)*deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		//Old: moveDirection.x += 1.0f;
		m_playerShip->rotate(+(degreesPerSecond)*deltaTime);
	}

	//This makes sure that the ship does not go faster when going diagonally
	moveDirection = gbh::math::normalize(moveDirection);
	m_playerShip->getPhysicsBody()->applyForceToCenter(moveDirection * acceleration);
}