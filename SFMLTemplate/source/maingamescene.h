#pragma once
#include "sfml-engine/scene.h"
#include "followcameranode.h"
#include <fstream>

//Ok so here the namespace works as a type/class for the class Scene, that we inherit from for the new class ManGameScene
class MainGameScene : public gbh::Scene {
protected:
	void onInitializeScene() override;
	void onUpdate(double deltaTime) override;
	void onBeginPhysicsContact(const gbh::PhysicsContact& contact);

	void onShowScene() override;
	void onHideScene() override;
	void loadLevel(const std::string& filename);

private:
	void advanceCheckpoints();

	std::shared_ptr<gbh::SpriteNode> m_playerShip;
	std::shared_ptr<FollowCameraNode> m_followCamera;
	std::vector<std::shared_ptr<gbh::SpriteNode>> m_checkPoints;
	int m_currentCheckPoint = -1;
	double m_playerTime;
	std::shared_ptr<gbh::TextNode> m_timerText;

	sf::Font m_robotoFont;
	
	//For the example that did not work
	//std::shared_ptr<gbh::SpriteNode> m_test;
};

