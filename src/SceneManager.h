#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "GameObject.h"
#include "Object.h"
#include "Springs.h"

#include "MSAFluid.h"

class SceneManager {
	public:

		SceneManager();
		
		void loadScene(string _path, vector<GameObject*>* _gameobjects, Controller* _controller, msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer);
		void destroyCurrentScene(vector<GameObject*>* _gameobjects, Controller* _controller);
		void resetFluid(msa::fluid::Solver* _fluidSolver, msa::fluid::DrawerGl* _fluidDrawer);

		ofxXmlSettings xml;

};