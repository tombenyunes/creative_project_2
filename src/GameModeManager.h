#pragma once

#include "ofMain.h"

class GameModeManager {

public:

	GameModeManager(int gameModeID = 0);
	void init();
	int getCurrentModeID();
	string getCurrentModeString();
	void setCurrentModeID(int gameModeID);
	void logCurrentMode();

	void keyPressed(int key);

	bool request_for_new_scene;

private:

	int currentModeID;

};