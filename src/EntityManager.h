#pragma once

// Managers
#include "GameObject.h"
#include "GUIManager.h"
#include "FluidManager.h"
#include "AudioManager.h"

// Entity types
#include "Player.h"
#include "Mass.h"
#include "Spring.h"
#include "Point.h"

class EntityManager {
	
public:
	EntityManager();
	
	void init(Controller* _gameController, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager, AudioManager* _audioManager);
	vector<GameObject*>* getGameObjects();
	GameObject* getSelectedGameObject();
	void addGameObject(GameObject* _gameobject);

	void updateGameObjects();
	void drawGameObjects();

	void deleteAll(bool excludePlayer = true);

	void createEntity();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseScrolled(int x, int y, float scrollX, float scrollY);
	void mouseReleased(int x, int y, int button);

private:

	void deleteGameObjects();
	
	void findSelected();
	GameObject* selectedGameObject;
	int selectedGameObjectIndex;

	vector<GameObject*> vec;
	vector<GameObject*>* GameObjects = &vec; // the main vector of all objects in the scene

	Controller* Game_Controller;
	GUIManager* GUI_Manager;
	Camera* cam;
	FluidManager* Fluid_Manager;
	AudioManager* Audio_Manager;


};