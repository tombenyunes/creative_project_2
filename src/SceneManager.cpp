#include "SceneManager.h"

SceneManager::SceneManager()
{
}

void SceneManager::saveScene(vector<GameObject*>* _gameobjects)
{
	xml1.popTag();
	xml1.clear();

	xml1.addTag("Scene");
	xml1.pushTag("Scene");

	for (int i = 0; i < _gameobjects->size(); i++) {
		xml1.addTag("GameObject");
		xml1.pushTag("GameObject", i);
		xml1.addValue("type", (*_gameobjects)[i]->type);
		xml1.addValue("pos.x", (*_gameobjects)[i]->pos.x);
		xml1.addValue("pos.y", (*_gameobjects)[i]->pos.y);
		xml1.addValue("mass", (*_gameobjects)[i]->mass);
		xml1.addValue("radius", (*_gameobjects)[i]->radius);
		if ((*_gameobjects)[i]->type == "Spring") {
			xml1.addValue("mass1", (*_gameobjects)[i]->nodeMass1);
			xml1.addValue("mass2", (*_gameobjects)[i]->nodeMass2);
			xml1.addValue("radius1", (*_gameobjects)[i]->nodeRadius1);
			xml1.addValue("radius2", (*_gameobjects)[i]->nodeRadius2);
		}
		xml1.popTag();
	}

	xml1.save("newScene.xml");
}

void SceneManager::loadScene(string _path, vector<GameObject*>* _gameobjects, Controller* _controller, msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer)
{
	destroyCurrentScene(_gameobjects, _controller);
	resetFluid(_fluidSolver, _fluidDrawer);

	// load scene //

	if (xml.loadFile(_path + ".xml")) {
		xml.pushTag("Scene");

		int GameObjectCount = xml.getNumTags("GameObject");
		for (int i = 0; i < GameObjectCount; i++) {
			xml.pushTag("GameObject", i);

			string type = (xml.getValue("type", "N/A"));
			ofVec2f pos;
			pos.x = (xml.getValue("pos.x", -1));
			pos.y = (xml.getValue("pos.y", -1));
			
			if (type == "Player") {
				float mass = (xml.getValue("mass", -1));
				float radius = (xml.getValue("radius", -1));
				
				GameObject* player = new Player;
				_gameobjects->push_back(player);
			}
			else if (type == "Object") {
				float mass = (xml.getValue("mass", -1));
				float radius = (xml.getValue("radius", -1));

				GameObject* object = new Object(pos, mass, radius, _controller);
				_gameobjects->push_back(object);
			}
			else if (type == "Spring") {				
				float mass1 = (xml.getValue("mass1", -1));
				float mass2 = (xml.getValue("mass2", -1));
				float radius1 = (xml.getValue("radius1", -1));
				float radius2 = (xml.getValue("radius2", -1));

				GameObject* spring = new Springs(pos, radius1, mass1, radius2, mass2, 2, 2, 22, _controller);
				_gameobjects->push_back(spring);
			}

			xml.popTag();
		}
	}
}

void SceneManager::destroyCurrentScene(vector<GameObject*>* _gameobjects, Controller* _controller)
{
	for (int i = 0; i < _gameobjects->size(); i++) {
		(*_gameobjects)[i]->needs_to_be_deleted = true;
	}
}

void SceneManager::resetFluid(msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer)
{
	//_fluidSolver->setup(100, 100);
	//_fluidSolver->enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	//_fluidDrawer->setup(_fluidSolver);
	_fluidSolver->reset();
}