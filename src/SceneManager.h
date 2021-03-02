#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"

#include "GameObject.h"
#include "Player.h"
#include "Mass.h"
#include "Spring.h"

#include "MSAFluid.h"
#include "FluidManager.h"
#include "AudioManager.h"

class SceneManager {
public:

		SceneManager();
		void init(vector<GameObject*>* _gameobjects, Controller* _controller, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager, AudioManager* _audioManager);

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
		GUIManager* GUI_Manager;
		FluidManager* Fluid_Manager;
		AudioManager* Audio_Manager;

		Camera* cam;

};