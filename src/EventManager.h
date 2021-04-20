#pragma once

#include "Controller.h"
#include "EntityManager.h"

class EventManager
{
public:

	EventManager();

	void init(Controller* _gameController, EntityManager* _entityManager);

	void setup();
	void update();
	void draw_tutorial();

	void key_pressed(int key);

	void show_tutorial(bool value);

	bool is_event_allowed(string event, int button = -1) const;

	bool player_gui_visible;

private:

	Controller* game_controller_;
	EntityManager* entity_manager_;

	vector<string> dialogues_;
	vector<ofVec2f> positions_;
	int current_index_;
	ofTrueTypeFont potta_one_main_;
	ofTrueTypeFont potta_one_context_;

	bool tutorial_enabled_;
	bool can_lmb_;
	bool can_select_;
	bool can_drag_;
	bool can_keypress_;
	bool full_input_;
};
