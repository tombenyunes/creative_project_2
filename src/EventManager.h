#pragma once

#include "Controller.h"
#include "GameObject.h"
#include "Mass.h"
#include "Spring.h"

class EventManager
{

public:
	
	EventManager();
	void setup();
	void update(Controller* _controller, vector<GameObject*>* _gameobjects);
	void draw();

	void keyPressed(int _key);

	void showTutorial(bool _value);
	bool tutorialEnabled;

	bool playerGUIVisible;
	bool canLMB;
	bool canSelect;
	bool canDrag;
	bool canKeypress;
	bool fullInput;

private:

	Controller* GameController;
	vector<string> dialogues;
	vector<ofVec2f> positions;
	int currentIndex;
	ofTrueTypeFont PottaOne_main;
	ofTrueTypeFont PottaOne_context;

};

