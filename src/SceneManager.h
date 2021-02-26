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
		void init(vector<GameObject*>* _gameobjects, Controller* _controller, guiController* _guiController, Camera* _cam, FluidManager* _fluidManager);

		void saveScene(string _sceneName);
		void loadScene(string _path);
		void destroyCurrentScene();
		void resetFluid();

		void keyPressed(int key);

		ofxXmlSettings xml;
		ofxXmlSettings xml1;

private:

		vector<GameObject*>* GameObjects;
		Controller* GameController;
		guiController* gui_Controller;
		Camera* cam;
		FluidManager* Fluid_Manager;

};