#include "EventManager.h"

EventManager::EventManager()
{
	tutorialEnabled = false;
}

void EventManager::init(Controller* _gameController, EntityManager* _entityManager)
{
	Game_Controller = _gameController;
	Entity_Manager = _entityManager;
}
															// IGNORE ALL THIS STUFF FOR NOW
void EventManager::setup()
{
	if (tutorialEnabled) {
		currentIndex = 0;
		PottaOne_main.load("PottaOne-Regular.ttf", 12, true, true);
		PottaOne_context.load("PottaOne-Regular.ttf", 10, true, true);
		
		dialogues = {
			"example dialogue",
			"example dialogue 2",
		};
		positions = {
			{ (float)(3.95 * (float)(dialogues[0].length()) * -1), -125 },
			{ (float)(3.95 * (float)(dialogues[1].length()) * -1), -125 },
		};

		playerGUIVisible = false;
		canLMB = false;
		canSelect = false;
		canDrag = false;
		canKeypress = false;
		fullInput = false;
	}
}

void EventManager::update()
{
	if (tutorialEnabled) {
		static bool i0 = false;
		if (!i0) {
			i0 = true;
			Game_Controller->setGUIVisible(false);
		}
		static bool i1 = false;
		if (currentIndex >= 1 && !i1) {
			i1 = true;
			Game_Controller->setGUIVisible(true);
			canKeypress = true;
			fullInput = true;
		}
	}
	else {
		static bool t;
		if (!t) {
			t = true;
			fullInput = true;
		}
	}
}

void EventManager::drawTutorial()
{
	if (tutorialEnabled) {
		PottaOne_main.drawString(dialogues[currentIndex], positions[currentIndex].x, positions[currentIndex].y);
		if (currentIndex <= 0) PottaOne_context.drawString("Use left/right arrow keys to navigate the tutorial", -177, 490);
	}
}

void EventManager::keyPressed(int _key)
{
	if (tutorialEnabled) {
		if (_key == 57356 && currentIndex != 0) {
			currentIndex--;
		}
		else if (_key == 57358 && currentIndex != dialogues.size() - 1) {
			currentIndex++;
		}
	}
}

void EventManager::showTutorial(bool _value)
{
	(_value == 0) ? tutorialEnabled = false : tutorialEnabled = true;
}

bool EventManager::isEventAllowed(string event, int button)
{
	if (event == "keyPressed") {
		if ((fullInput) || (canKeypress)) {
			return true;
		}
	}
	else if (event == "keyReleased") {
		if ((fullInput) || (canKeypress)) {
			return true;
		}
	}
	else if (event == "mouseMoved") {

	}
	else if (event == "mouseDragged") {
		if ((fullInput) || (button == 0 && canLMB) || (button == 2 && canDrag)) {
			return true;
		}
	}
	else if (event == "mousePressed") {
		if ((fullInput) || (button == 0 && canLMB) || (button == 2 && canSelect)) {
			return true;
		}
	}
	else if (event == "mouseScrolled") {

	}
	else if (event == "mouseReleased") {
		if ((fullInput) || (button == 0 && canLMB) || (button == 2 && canSelect)) {
			return true;
		}
	}
}
