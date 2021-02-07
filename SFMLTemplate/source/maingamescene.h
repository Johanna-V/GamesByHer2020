#pragma once
#include "sfml-engine/scene.h"
#include "followcameranode.h"

//Ok so here the namespace works as a type/class for the class Scene, that we inherit from for the new class ManGameScene
class MainGameScene : public gbh::Scene {
protected:
	void onInitializeScene() override;
	void onUpdate(double deltaTime) override;

private:
	std::shared_ptr<gbh::SpriteNode> m_playerShip;
	std::shared_ptr<FollowCameraNode> m_followCamera;

	//For the example that did not work
	//std::shared_ptr<gbh::SpriteNode> m_test;
};

