#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "GameObject.h"
#include "Player.h"
#include "Object.h"
#include "Springs.h"

#include "MSAFluid.h"
#include "FluidManager.h"

class SceneManager {
	public:

		SceneManager();
		
		void saveScene(vector<GameObject*>* _gameobjects, int _fluidMode, string _sceneName);
		void loadScene(string _path, vector<GameObject*>* _gameobjects, Controller* _controller, msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer);
		void destroyCurrentScene(vector<GameObject*>* _gameobjects, Controller* _controller);
		void resetFluid(msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer);

		void keyPressed(int key, FluidManager& Fluid_Manager, Controller* GameController, vector<GameObject*>* _gameobjects);

		ofxXmlSettings xml;
		ofxXmlSettings xml1;

};