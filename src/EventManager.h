#pragma once

#include "Controller.h"
#include "GameObject.h"
#include "Mass.h"
#include "Spring.h"
#include "EntityManager.h"

class EventManager
{

public:
	
	EventManager();

	void init(Controller* _gameController, EntityManager* _entityManager);

	void setup();
	void update();
	void drawTutorial();

	void keyPressed(int _key);

	void showTutorial(bool _value);
	bool tutorialEnabled;

	bool isEventAllowed(string event, int button = -1);

	bool playerGUIVisible;	

private:

	vector<GameObject*>* GameObjects;
	Controller* Game_Controller;
	EntityManager* Entity_Manager;

	vector<string> dialogues;
	vector<ofVec2f> positions;
	int currentIndex;
	ofTrueTypeFont PottaOne_main;
	ofTrueTypeFont PottaOne_context;

	bool canLMB;
	bool canSelect;
	bool canDrag;
	bool canKeypress;
	bool fullInput;

};

