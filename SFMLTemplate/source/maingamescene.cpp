#include "maingamescene.h"
#include "sfml-engine/mathutils.h"
#include <nlohmann/json.hpp>
#include <iostream>

const std::string cMainbackground = "../assets/gfx/space-background-01.png";
const std::string kPlayerShip = "../assets/gfx/player-ship.png";
const std::string kMainScreenFont = "../assets/fonts/roboto-regular.ttf";
const std::string kCheckPointSprite = "../assets/gfx/checkpoint.png";

//Color of the checkpoints
static const sf::Color kInactiveCheckpoint = sf::Color(255, 255, 255, 64);
static const sf::Color kNextCheckpoint = sf::Color(64, 64, 255, 192);
static const sf::Color kDoneCheckpoint = sf::Color(64, 255, 64, 128);

std::string filename = "../assets/levels/level_01.json";

void MainGameScene::onShowScene() {
	loadLevel("../assets/levels/level_01.json");
	advanceCheckpoints();

	//m_gameMusic.play();
}

void MainGameScene::onHideScene() {
	//m-gameMusic.stop();
}

/*Open a json file using the special json C++ parsing library nlohmann, catch exceptions.
If checkpoints (that we load to) is an array, iterate through it, get the xy coordinates from the file
and apply physics and everything we used to do in "oninitializescene" for the checkpoints*/
void MainGameScene::loadLevel(const std::string& filename)
{
	std::ifstream file(filename);
	nlohmann::json jsonFile;

	//Some JSON parsing error handling
	try
	{
		jsonFile = nlohmann::json::parse(file);
	}
	catch (const std::exception& ex)
	{
		std::cout << "Failed to load level from file: " << filename << ": " << ex.what() << "\n";
		return;
	}

	nlohmann::json checkpoints = jsonFile["checkpoints"];

	if (checkpoints.is_array())
	{
		for (int i = 0; i < checkpoints.size(); ++i)
		{
			float x = checkpoints[i]["x"].get<float>();
			float y = checkpoints[i]["y"].get<float>();

			std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckPointSprite);
			node->setColor(kInactiveCheckpoint);
			node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
			node->getPhysicsBody()->makeSensor();
			node->getPhysicsBody()->setEnabled(false);
			node->setPosition(x, y);
			node->setName("checkpoint");

			node->setBeginContactCallback([this](const gbh::PhysicsContact& contact) {
				if (contact.containsNode(m_playerShip.get()))
				{
					advanceCheckpoints();
				}
				});

			m_checkPoints.push_back(node);
			addChild(node);
		}

		m_currentCheckPoint = -1;
	}
}

/*Overriding the function that is empty in the base class "Scene", 
telling it to do lots of things instead of doing nothing, for the MainGameScene class*/
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
	/*attaches the ship to a physics body and creates a box with the size variable we made for the ship:
	Not sure what the last '0.5f' does. Somehow setting orgin/scale? 'Physicsmaterial'? Some dark magic of math?*/
	m_playerShip->setPhysicsBody(getPhysicsWorld()->createBox(shipSize * 0.5f));
	m_playerShip->getPhysicsBody()->setLinearDamping(2.0f);
	m_playerShip->getPhysicsBody()->setFixedRotation(true);
	//The ship was turned the wrong way originally so here we point the front up
	m_playerShip->rotate(180);
	addChild(m_playerShip);

	/*This makes the camera follow the ship around*/
	m_followCamera = std::make_shared<FollowCameraNode>();
	m_followCamera->setTarget(m_playerShip);
	m_followCamera->setPosition(640, 360);
	addChild(m_followCamera);
	setCamera(m_followCamera);

	//Adds a timer overlay text
	m_robotoFont.loadFromFile(kMainScreenFont);
	m_timerText = std::make_shared<gbh::TextNode>("0", m_robotoFont, 24);
	m_timerText->setOrigin(1.0f, 1.0f);		// Set origin to bottom right
	m_timerText->setPosition(1270, 700);	// Move to bottom right of screen
	getOverlay().addChild(m_timerText);		// Add to overlay so that it 'sticks' to the screen

	//The hard-coded version of adding checkpoints without using a json file for the coordinates. 
	//Most is moved to onSceneShow() but I can't remove the below for some reason
	std::vector<sf::Vector2f> checkPointPositions;
	checkPointPositions = {
		sf::Vector2f(640.0f, 720.0f),
		sf::Vector2f(1240.0f, 200.0f),
		sf::Vector2f(80.0f, 400.0f),
	};

	//A for-loop going through each checkpoint position, setting up that checkpoint's properties,
	//adding it to a member vector for storage and then drawing/adding the child to the scene
	for (int i = 0; i < checkPointPositions.size(); ++i) {
		
		sf::Vector2f position = checkPointPositions[i];

		std::shared_ptr<gbh::SpriteNode> node = std::make_shared<gbh::SpriteNode>(kCheckPointSprite);
		node->setColor(kInactiveCheckpoint);
		node->setPhysicsBody(getPhysicsWorld()->createCircle(50));
		node->getPhysicsBody()->makeSensor();
		node->getPhysicsBody()-> setEnabled(false);
		node->setPosition(checkPointPositions[i]);
		node->setName("checkpoint");

		m_checkPoints.push_back(node);
		addChild(node);

	}
	//A first call to make sure that we have one checkpoint that is enabled
	advanceCheckpoints();
}

/*If the ship collides with a checkpoint, the checkpoints should be advanced*/
void MainGameScene::onBeginPhysicsContact(const gbh::PhysicsContact& contact)
{
	/*Here I had to peek on someone else's solution since the playership could not be converted
	The trick was then to add .get() . I have no idea why...*/
	if (contact.containsNode(m_playerShip.get())) {
		gbh::Node* otherNode = contact.otherNode(m_playerShip.get());
		if (otherNode && otherNode->getName() == "checkpoint") {
			advanceCheckpoints();
		}
	}
}

/*If this is called, change the current checkpoint's color to done and disable its Physicsbody.
Then advance which the current checkpoint is. That one, we enable and give the 
"next checkpoint"-color*/
void MainGameScene::advanceCheckpoints()
{
	if (m_currentCheckPoint >= 0 && m_currentCheckPoint < m_checkPoints.size())
	{
		m_checkPoints[m_currentCheckPoint]->setColor(kDoneCheckpoint);
		m_checkPoints[m_currentCheckPoint]->getPhysicsBody()->setEnabled(false);
		m_currentCheckPoint++;
	}
	else
	{
		m_currentCheckPoint = 0;
	}

	if (m_currentCheckPoint < m_checkPoints.size())
	{
		m_checkPoints[m_currentCheckPoint]->setColor(kNextCheckpoint);
		m_checkPoints[m_currentCheckPoint]->getPhysicsBody()->setEnabled(true);
	}
	else
	{
		std::cout << "Completed Course! \n";
	}
}

/*Delta time describes the time difference between the previous frame that was drawn
and the current frame, and makes sure that movement speed will not change depending 
on what fps the current player's display can handle. This is how we make things tick 
with a more "reliable" framerate, to move something around the screen.*/
void MainGameScene::onUpdate(double deltaTime)
{
	//For tracking play time
	m_playerTime += deltaTime;
	m_timerText->setString(std::to_string(m_playerTime));

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