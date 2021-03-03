#include "EntityManager.h"

EntityManager::EntityManager()
{
	Game_Controller = nullptr;
	cam = nullptr;
}

void EntityManager::init(Controller* _gameController, Camera* _cam)
{
	Game_Controller = _gameController;
	cam = _cam;
}

vector<GameObject*>* EntityManager::getGameObjects()
{
	return GameObjects;
}

void EntityManager::addGameObject(GameObject* _gameobject)
{
	GameObjects->push_back(_gameobject);
}

void EntityManager::updateGameObjects()
{
	deleteGameObjects();

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

void EntityManager::drawGameObjects(ofVec2f translation)
{
	ofTranslate(translation);
	for (int i = 0; i < getGameObjects()->size(); i++) {
		(*getGameObjects())[i]->root_draw();
	}
}