#include "GameModeManager.h"

GameModeManager::GameModeManager(int gameModeID)
{
	currentModeID = gameModeID;
	request_for_new_scene = false;
	
	logCurrentMode();
}

void GameModeManager::init()
{
}

int GameModeManager::getCurrentModeID()
{
	return currentModeID;
}

void GameModeManager::setCurrentModeID(int gameModeID)
{
	currentModeID = gameModeID;

	logCurrentMode();
}

void GameModeManager::logCurrentMode()
{
	string modeText;
	switch (currentModeID)
	{
	case 0:
		modeText = "Sandbox";
		break;
	case 1:
		modeText = "Procedural";
		break;
	default:
		modeText = "[MODE ID UNDEFINED]";
		break;
	}
	cout << "----------GameModeManager.cpp--------" << endl;
	cout << " - Game Mode: " << modeText << endl;
	cout << "----------------------------------------" << endl;
}

void GameModeManager::keyPressed(int key)
{
	if (key == 57347) { // f4
		int newID = -1;
		if (currentModeID == 0) { // toggles between modes
			newID = 1;
			request_for_new_scene = true;
		}
		else if (currentModeID == 1) {
			newID = 0;
		}
		setCurrentModeID(newID);
	}
}