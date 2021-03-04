#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "MSAFluid.h"
#include "EntityManager.h"
#include "GameModeManager.h"

class SceneManager {
public:

		SceneManager();
		void init(Controller* _controller, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager, AudioManager* _audioManager, EntityManager* _entityManager, GameModeManager* _gamemodeManager);

		void update();

		void saveScene(string _sceneName);
		void getReadyForNewScene();
		void loadScene(string _path);
		void loadProceduralScene();
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
		GameModeManager* GameMode_Manager;

		Camera* cam;

};