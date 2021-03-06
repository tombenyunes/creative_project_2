#include "EntityManager.h"

EntityManager::EntityManager()
{
	Game_Controller = nullptr;
	GUI_Manager = nullptr;
	Fluid_Manager = nullptr;
	Audio_Manager = nullptr;

	cam = nullptr;

	selectedGameObject = nullptr;
	selectedGameObjectIndex = -1;
}

void EntityManager::init(Controller* _gameController, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager, AudioManager* _audioManager, GameModeManager* _gamemodeManager)
{
	Game_Controller = _gameController;
	GUI_Manager = _GUIManager;
	Fluid_Manager = _fluidManager;
	Audio_Manager = _audioManager;
	GameMode_Manager = _gamemodeManager;

	cam = _cam;
}

vector<GameObject*>* EntityManager::getGameObjects()
{
	return GameObjects;
}

GameObject* EntityManager::getSelectedGameObject()
{
	return selectedGameObject;
}

void EntityManager::addGameObject(GameObject* _gameobject)
{
	GameObjects->push_back(_gameobject);
}

void EntityManager::updateGameObjects()
{
	deleteGameObjects();
	findSelected();

	// update all gameobjects
	for (int i = 0; i < getGameObjects()->size(); i++) {
		(*getGameObjects())[i]->root_update();
	}
}

void EntityManager::deleteGameObjects()
{
	// erase objects that need to be deleted and free memory
	for (int i = 0; i < getGameObjects()->size(); i++) {
		if ((*getGameObjects())[i]->isPlayer == true) {
			cam->update(WORLD_WIDTH, WORLD_HEIGHT, ofVec2f((*getGameObjects())[i]->pos.x + WORLD_WIDTH / 2, (*getGameObjects())[i]->pos.y + WORLD_HEIGHT / 2));
		}
		if ((*getGameObjects())[i]->needs_to_be_deleted == true) {
			if ((*getGameObjects())[i] == Game_Controller->getActive()) {
				Game_Controller->makeActive(nullptr);
			}
			delete (*getGameObjects())[i];
			getGameObjects()->erase(getGameObjects()->begin() + i);
		}
	}

	// delete all if gui requests it
	if (Game_Controller->getDeleteAll()) {
		Game_Controller->setDeleteAll(false);
		deleteAll(true);
	}
}

void EntityManager::findSelected()
{
	// find gameobject/gameobjects that are selected
	for (int i = 0; i < getGameObjects()->size(); i++) {
		if ((*getGameObjects())[i]->requestToBeSelected == true) {
			
			if (selectedGameObject != (*getGameObjects())[i]) {
				// accept request

				// remove previously selected object
				if (selectedGameObject != nullptr) (*getGameObjects())[selectedGameObjectIndex]->isSelected = false;

				// make new object selected
				selectedGameObject = (*getGameObjects())[i];
				selectedGameObjectIndex = i;
				(*getGameObjects())[i]->isSelected = true;
			}
			else {
				// deny request
				(*getGameObjects())[i]->isSelected = false;
			}

			(*getGameObjects())[i]->requestToBeSelected = false;
		}
	}
}

void EntityManager::deleteAll(bool excludePlayer)
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		if (excludePlayer) {
			if ((*getGameObjects())[i]->isPlayer != true) {
				(*getGameObjects())[i]->needs_to_be_deleted = true;
			}
		}
		else {
			(*getGameObjects())[i]->needs_to_be_deleted = true;
		}
	}
}

void EntityManager::drawGameObjects()
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		(*getGameObjects())[i]->root_draw();
	}
}

void EntityManager::createEntity(string entityType)
{
	int typeID;
	if (entityType == "") {
		typeID = Game_Controller->getNewNodeType();
	}
	else {
		typeID = -1;
	}
	
	if (entityType == "Player") {
		GameObject* player = new Player;
		player->init(getGameObjects(), Game_Controller, GUI_Manager, cam, Fluid_Manager, Audio_Manager);
		addGameObject(player);
	}
	if (typeID == 0 || entityType == "Mass") {
		cout << "Mass created" << endl;
		GameObject* object = new Mass(ofVec2f(Game_Controller->getWorldMousePos().x, Game_Controller->getWorldMousePos().y), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(getGameObjects(), Game_Controller, GUI_Manager, cam, Fluid_Manager, Audio_Manager);
		addGameObject(object);
	}
	else if (typeID == 1 || entityType == "Spring") {
		cout << "Spring created" << endl;
		GameObject* spring = new Spring(ofVec2f(Game_Controller->getWorldMousePos().x, Game_Controller->getWorldMousePos().y), ofRandom(25, 50), ofRandom(25, 75), ofRandom(25, 50), ofRandom(25, 75), 2, 2, 22);
		spring->init(getGameObjects(), Game_Controller, GUI_Manager, cam, Fluid_Manager, Audio_Manager);
		addGameObject(spring);
	}
	else if (typeID == 2 || entityType == "Point") {
		cout << "Point created" << endl;
		GameObject* point = new Point(ofVec2f(ofRandom(-WORLD_WIDTH / 2, WORLD_WIDTH / 2), ofRandom(-WORLD_HEIGHT / 2, WORLD_HEIGHT / 2)), 15, 25);
		point->init(getGameObjects(), Game_Controller, GUI_Manager, cam, Fluid_Manager, Audio_Manager);
		addGameObject(point);
	}
}

int EntityManager::getPointCount()
{
	int pointCount = 0;
	for (int i = 0; i < getGameObjects()->size(); i++) {
		if ((*getGameObjects())[i]->type == "Point") {
			pointCount++;
		}
	}
	return pointCount;
}

void EntityManager::keyPressed(int key)
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		(*getGameObjects())[i]->root_keyPressed(key);
	}

	if (GameMode_Manager->getCurrentModeID() == 0) {
		if (key == 'c') {
			createEntity();
		}
		if (key == 57358) {
			if (Game_Controller->getNewNodeType() < 2) {
				Game_Controller->setNewNodeType(Game_Controller->getNewNodeType() + 1); // next
			}
		}
		else if (key == 57356) {
			if (Game_Controller->getNewNodeType() > 0) {
				Game_Controller->setNewNodeType(Game_Controller->getNewNodeType() - 1); // previous
			}
			else {
				Game_Controller->setNewNodeType(0);
			}
		}
	}
}

void EntityManager::keyReleased(int key)
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		(*getGameObjects())[i]->root_keyReleased(key);
	}
}

void EntityManager::mouseMoved(int x, int y)
{
}

void EntityManager::mouseDragged(int x, int y, int button)
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam->getPosition().z);
		ofVec3f worldView = cam->screenToWorld(localView);
		(*getGameObjects())[i]->mouseDragged(worldView.x, worldView.y, button);
	}
}

void EntityManager::mousePressed(int x, int y, int button)
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam->getPosition().z);
		ofVec3f worldView = cam->screenToWorld(localView);
		(*getGameObjects())[i]->mousePressed(worldView.x, worldView.y, button);
	}
}

void EntityManager::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
}

void EntityManager::mouseReleased(int x, int y, int button)
{
	for (int i = 0; i < getGameObjects()->size(); i++) {
		(*getGameObjects())[i]->mouseReleased(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), button);
	}
}