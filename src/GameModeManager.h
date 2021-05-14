#pragma once

#include "ofMain.h"
#include "Controller.h"
#include "GUIManager.h"

class GamemodeManager
{
public:

	GamemodeManager(int game_mode_id = 0);
	void init(GUIManager* gui_manager);
	int get_current_mode_id() const;
	string get_current_mode_string() const;
	void set_current_mode_id(int game_mode_id);
	void log_current_mode() const;

	void key_pressed(int key);
	void mouse_pressed(int x, int y, int button);

	bool request_for_new_scene;
	bool game_started_;

private:

	GUIManager* gui_manager_;
	
	int current_mode_id_;
	int prev_mode_id_;

};
