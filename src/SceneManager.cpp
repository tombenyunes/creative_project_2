#include "SceneManager.h"

SceneManager::SceneManager()
{
	cout << "------------SceneManager.cpp------------" << endl;
	cout << "Press '1-4' to load preset scenes" << endl;
	cout << "Press '0' to save the current scene" << endl;
	cout << "Press '9' to load a saved scene" << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::init(vector<GameObject*>* _gameobjects, Controller* _controller, guiController* _guiController, Camera* _cam, FluidManager* _fluidManager)
{
	GameObjects = _gameobjects;
	GameController = _controller;
	gui_Controller = _guiController;
	cam = _cam;
	Fluid_Manager = _fluidManager;
}

void SceneManager::saveScene(string _sceneName)
{	
	cout << "------------SceneManager.cpp------------" << endl;

	xml1.popTag();
	xml1.clear();

	xml1.addTag("Scene");
	xml1.pushTag("Scene");

	xml1.addValue("name", _sceneName);

	xml1.addTag("Fluid");
	xml1.pushTag("Fluid", 0);
	xml1.addValue("mode", (int&)(Fluid_Manager->getDrawer()->drawMode));
	xml1.popTag();

	for (int i = 0; i < GameObjects->size(); i++) {
		xml1.addTag("GameObject");
		xml1.pushTag("GameObject", i);
		xml1.addValue("type", (*GameObjects)[i]->type);
		xml1.addValue("pos.x", (*GameObjects)[i]->pos.x);
		xml1.addValue("pos.y", (*GameObjects)[i]->pos.y);
		xml1.addValue("mass", (*GameObjects)[i]->mass);
		xml1.addValue("radius", (*GameObjects)[i]->radius);
		if ((*GameObjects)[i]->type == "Spring") {
			xml1.addValue("mass1", (*GameObjects)[i]->nodeMasses[0]);
			xml1.addValue("mass2", (*GameObjects)[i]->nodeMasses[1]);
			xml1.addValue("radius1", (*GameObjects)[i]->nodeRadiuses[0]);
			xml1.addValue("radius2", (*GameObjects)[i]->nodeRadiuses[1]);
		}
		xml1.popTag();
	}

	xml1.save(_sceneName + ".xml");

	cout << "[ Current Scene Saved ]" << endl;
	cout << "- Scene Name: " << _sceneName << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::loadScene(string _path)
{
	destroyCurrentScene();
	resetFluid();

	// load scene //

	if (xml.loadFile(_path + ".xml")) {
		xml.pushTag("Scene");

		cout << "------------SceneManager.cpp------------" << endl;
		cout << "[ Scene Loaded ]" << endl;
		cout << "- Scene Name: " << xml.getValue("name", "N/A") << endl;

		int FluidCount = xml.getNumTags("Fluid");
		for (int i = 0; i < FluidCount; i++) {
			xml.pushTag("Fluid", i);
			(int&)Fluid_Manager->getDrawer()->drawMode = xml.getValue("mode", -1);
			cout << "- Fluid Mode: " << xml.getValue("mode", -1) << endl;
			xml.popTag();
		}
		int count = 0;
		int GameObjectCount = xml.getNumTags("GameObject");
		for (int i = 0; i < GameObjectCount; i++) {
			count++;
			xml.pushTag("GameObject", i);

			string type = (xml.getValue("type", "N/A"));
			ofVec2f pos;
			pos.x = (xml.getValue("pos.x", -1));
			pos.y = (xml.getValue("pos.y", -1));

			if (type == "Player") {
				float mass = (xml.getValue("mass", -1));
				float radius = (xml.getValue("radius", -1));

				GameObject* player = new Player;
				player->init(GameObjects, GameController, gui_Controller, cam, Fluid_Manager);
				GameObjects->push_back(player);
			}
			else if (type == "Object") {
				float mass = (xml.getValue("mass", -1));
				float radius = (xml.getValue("radius", -1));

				GameObject* object = new Object(pos, mass, radius);
				object->init(GameObjects, GameController, gui_Controller, cam, Fluid_Manager);
				GameObjects->push_back(object);
			}
			else if (type == "Spring") {
				float mass1 = (xml.getValue("mass1", -1));
				float mass2 = (xml.getValue("mass2", -1));
				float radius1 = (xml.getValue("radius1", -1));
				float radius2 = (xml.getValue("radius2", -1));

				GameObject* spring = new Springs(pos, radius1, mass1, radius2, mass2, 2, 2, 22);
				spring->init(GameObjects, GameController, gui_Controller, cam, Fluid_Manager);
				GameObjects->push_back(spring);
			}

			xml.popTag();
		}

		cout << "- GameObject count: " << count << endl;
		cout << "----------------------------------------" << endl;

	}
}

void SceneManager::destroyCurrentScene()
{
	for (int i = 0; i < GameObjects->size(); i++) {
		(*GameObjects)[i]->needs_to_be_deleted = true;
	}
}

void SceneManager::resetFluid()
{
	Fluid_Manager->resetFluid();
}

void SceneManager::keyPressed(int key)
{
	if (key == '1') {
		// load scene 1
		loadScene("Scenes/Scene1");

		Fluid_Manager->explosion(500);
	}
	else if (key == '2') {
		// load scene 2
		loadScene("Scenes/Scene2");

		Fluid_Manager->explosion(500);
	}
	else if (key == '3') {
		// load scene 2
		loadScene("Scenes/Scene3");

		Fluid_Manager->explosion(500);
	}
	else if (key == '4') {
		// load scene 2
		loadScene("Scenes/Scene4");

		Fluid_Manager->explosion(500);
	}
	else if (key == '9') {
		// load saved scene
		loadScene("Scenes/newScene");

		Fluid_Manager->explosion(500);
	}
	else if (key == '0') {
		// save scene
		saveScene("Scenes/newScene");
	}
}