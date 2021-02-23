#include "SceneManager.h"

SceneManager::SceneManager()
{
	cout << "----------------------------------------" << endl;
	cout << "Press '1-4' to load preset scenes" << endl;
	cout << "Press '0' to save the current scene" << endl;
	cout << "Press '9' to load a saved scene" << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::saveScene(vector<GameObject*>* _gameobjects, int _fluidMode, string _sceneName)
{
	cout << "----------------------------------------" << endl;

	xml1.popTag();
	xml1.clear();

	xml1.addTag("Scene");
	xml1.pushTag("Scene");

	xml1.addValue("name", _sceneName);

	xml1.addTag("Fluid");
	xml1.pushTag("Fluid", 0);
	xml1.addValue("mode", _fluidMode);
	xml1.popTag();

	for (int i = 0; i < _gameobjects->size(); i++) {
		xml1.addTag("GameObject");
		xml1.pushTag("GameObject", i);
		xml1.addValue("type", (*_gameobjects)[i]->type);
		xml1.addValue("pos.x", (*_gameobjects)[i]->pos.x);
		xml1.addValue("pos.y", (*_gameobjects)[i]->pos.y);
		xml1.addValue("mass", (*_gameobjects)[i]->mass);
		xml1.addValue("radius", (*_gameobjects)[i]->radius);
		if ((*_gameobjects)[i]->type == "Spring") {
			xml1.addValue("mass1", (*_gameobjects)[i]->nodeMasses[0]);
			xml1.addValue("mass2", (*_gameobjects)[i]->nodeMasses[1]);
			xml1.addValue("radius1", (*_gameobjects)[i]->nodeRadiuses[0]);
			xml1.addValue("radius2", (*_gameobjects)[i]->nodeRadiuses[1]);
		}
		xml1.popTag();
	}

	xml1.save("newScene.xml");

	cout << "[ Current Scene Saved ]" << endl;
	cout << "- Scene Name: " << _sceneName << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::loadScene(string _path, vector<GameObject*>* _gameobjects, Controller* _controller, msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer)
{
	destroyCurrentScene(_gameobjects, _controller);
	resetFluid(_fluidSolver, _fluidDrawer);

	// load scene //

	if (xml.loadFile(_path + ".xml")) {
		xml.pushTag("Scene");

		cout << "----------------------------------------" << endl;
		cout << "[ Scene Loaded ]" << endl;
		cout << "- Scene Name: " << xml.getValue("name", "N/A") << endl;

		int FluidCount = xml.getNumTags("Fluid");
		for (int i = 0; i < FluidCount; i++) {
			xml.pushTag("Fluid", i);
			(int&)_fluidDrawer->drawMode = xml.getValue("mode", -1);
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

		cout << "- GameObject count: " << count << endl;
		cout << "----------------------------------------" << endl;

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

void SceneManager::keyPressed(int key, FluidManager& Fluid_Manager, Controller* GameController, vector<GameObject*>* GameObjects)
{
	if (key == '1') {
		// load scene 1
		msa::fluid::Solver* solver = Fluid_Manager.getSolver();
		msa::fluid::DrawerGl* drawer = Fluid_Manager.getDrawer();
		loadScene("Scene1", GameObjects, GameController, solver, drawer);

		Fluid_Manager.explosion(500);
	}
	else if (key == '2') {
		// load scene 2
		msa::fluid::Solver* solver = Fluid_Manager.getSolver();
		msa::fluid::DrawerGl* drawer = Fluid_Manager.getDrawer();
		loadScene("Scene2", GameObjects, GameController, solver, drawer);

		Fluid_Manager.explosion(500);
	}
	else if (key == '3') {
		// load scene 2
		msa::fluid::Solver* solver = Fluid_Manager.getSolver();
		msa::fluid::DrawerGl* drawer = Fluid_Manager.getDrawer();
		loadScene("Scene3", GameObjects, GameController, solver, drawer);

		Fluid_Manager.explosion(500);
	}
	else if (key == '4') {
		// load scene 2
		msa::fluid::Solver* solver = Fluid_Manager.getSolver();
		msa::fluid::DrawerGl* drawer = Fluid_Manager.getDrawer();
		loadScene("Scene4", GameObjects, GameController, solver, drawer);

		Fluid_Manager.explosion(500);
	}
	else if (key == '9') {
		// load saved scene
		msa::fluid::Solver* solver = Fluid_Manager.getSolver();
		msa::fluid::DrawerGl* drawer = Fluid_Manager.getDrawer();
		loadScene("newScene", GameObjects, GameController, solver, drawer);

		Fluid_Manager.explosion(500);
	}
	else if (key == '0') {
		// save scene
		saveScene(GameObjects, (int&)(Fluid_Manager.getDrawer()->drawMode), "newScene");
	}
}