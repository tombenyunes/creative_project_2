#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "MSAFluid.h"
#include "EntityManager.h"

class SceneManager {
public:

		SceneManager();
		void init(Controller* _controller, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager, AudioManager* _audioManager, EntityManager* _entityManager);

		void saveScene(string _sceneName);
		void loadScene(string _path);
		void destroyCurrentScene();
		void resetFluid();

		void keyPressed(int key);

		ofxXmlSettings xml;
		ofxXmlSettings xml1;

private:

		Controller* GameController;
		GUIManager* GUI_Manager;
		FluidManager* Fluid_Manager;
		AudioManager* Audio_Manager;
		EntityManager* Entity_Manager;

		Camera* cam;

};