#pragma once
#include "sfml-engine/scene.h"

//Ok so here the namespace works as a type/class for the class Scene, that we inherit from for the new class ManGameScene
class MainGameScene : public gbh::Scene {
protected:
	void onInitializeScene() override;
	};

