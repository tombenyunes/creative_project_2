#include "GamemodeManager.h"

GamemodeManager::GamemodeManager(const int game_mode_id)
	:	request_for_new_scene(false)
	,	gui_manager_(nullptr)
	,	current_mode_id_(game_mode_id)
{
	log_current_mode();
}

void GamemodeManager::init(GUIManager* gui_manager)
{
	gui_manager_ = gui_manager;
}

int GamemodeManager::get_current_mode_id() const
{
	return current_mode_id_;
}

string GamemodeManager::get_current_mode_string() const
{
	string mode_text;
	switch (current_mode_id_)
	{
	case 0:
		mode_text = "Sandbox";
		break;
	case 1:
		mode_text = "Procedural";
		break;
	default:
		mode_text = "[MODE ID UNDEFINED]";
		break;
	}
	return mode_text;
}

void GamemodeManager::set_current_mode_id(const int game_mode_id)
{
	current_mode_id_ = game_mode_id;

	log_current_mode();
}

void GamemodeManager::log_current_mode() const
{
	string mode_text;
	switch (current_mode_id_)
	{
	case 0:
		mode_text = "Sandbox";
		break;
	case 1:
		mode_text = "Procedural";
		break;
	default:
		mode_text = "[MODE ID UNDEFINED]";
		break;
	}
	cout << "----------GamemodeManager.cpp--------" << endl;
	cout << " - Game Mode: " << mode_text << endl;
	cout << "----------------------------------------" << endl;
}

void GamemodeManager::key_pressed(const int key)
{
	if (key == 57347) // f4 toggles between modes
	{
		int new_id = -1;
		if (current_mode_id_ == 0)
		{
			new_id = 1;
			gui_manager_->set_gui_visible(false);
			// load new scene
			request_for_new_scene = true;
		}
		else if (current_mode_id_ == 1)
		{
			new_id = 0;
			gui_manager_->set_gui_visible(true);
		}
		set_current_mode_id(new_id);
	}
}
