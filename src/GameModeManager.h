#pragma once

#include "ofMain.h"

class GamemodeManager
{
public:

	GamemodeManager(int game_mode_id = 0);
	void init();
	int get_current_mode_id() const;
	string get_current_mode_string() const;
	void set_current_mode_id(int game_mode_id);
	void log_current_mode() const;

	void key_pressed(int key);

	bool request_for_new_scene;

private:

	int current_mode_id_;
};
