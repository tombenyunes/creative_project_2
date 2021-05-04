#pragma once

#include "ofMain.h"
#include "Controller.h"

class GamemodeManager
{
public:

	GamemodeManager(int game_mode_id = 0);
	void init(Controller* game_controller);
	int get_current_mode_id() const;
	string get_current_mode_string() const;
	void set_current_mode_id(int game_mode_id);
	void log_current_mode() const;

	void key_pressed(int key);

	bool request_for_new_scene;

private:

	Controller* game_controller_;
	
	int current_mode_id_;
};
