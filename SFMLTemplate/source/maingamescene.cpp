#include "maingamescene.h"
#include <iostream>

/*Overriding the function that is empty in the base class "Scene", 
telling it to print some text instead of doing nothing, for the MainGameScene class*/
void MainGameScene::onInitializeScene() {
	std::cout << "Initializing MainGameScene\n";
}