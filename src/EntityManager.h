#pragma once

#include "GameObject.h"

class EntityManager {
	
public:
	EntityManager();
	void init(Controller* _gameController, Camera* _cam);

	vector<GameObject*>* getGameObjects();
	void addGameObject(GameObject* _gameobject);

	void updateGameObjects();
	void drawGameObjects(ofVec2f translation);

	void deleteAll(bool excludePlayer = true);

private:

	void deleteGameObjects();

	vector<GameObject*> vec;
	vector<GameObject*>* GameObjects = &vec; // the main vector of all objects in the scene

	Controller* Game_Controller;
	Camera* cam;

};