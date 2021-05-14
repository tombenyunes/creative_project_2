#include "GamemodeManager.h"

GamemodeManager::GamemodeManager(const int game_mode_id)
	:	request_for_new_scene(false)
	,	gui_manager_(nullptr)
	,	current_mode_id_(game_mode_id)
	,	prev_mode_id_(game_mode_id)
	,	game_started_(false)
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
	case 2:
		mode_text = "Menu";
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
	
	switch (current_mode_id_)
	{
	case 0: // sandbox mode
		// show gui
		gui_manager_->set_gui_visible(true);
		if (!game_started_) game_started_ = true;
		break;
	case 1: // procedural mode
		// disable gui
		gui_manager_->set_gui_visible(false);
		if (!game_started_) game_started_ = true;
		// load new scene
		//request_for_new_scene = true;		
		break;
	case 2: // menu mode
		cout << " menu mode " << endl;
		break;
	default:
		break;
	}
	
	log_current_mode();
}

void GamemodeManager::log_current_mode() const
{
	cout << "----------GamemodeManager.cpp--------" << endl;
	cout << " - Game Mode: " << get_current_mode_string() << endl;
	cout << "----------------------------------------" << endl;
}

void GamemodeManager::key_pressed(const int key)
{
	if (key == 32) // 'space' toggles between modes
	{
		if (get_current_mode_id() == 0)
		{
			set_current_mode_id(1);
		}
		else if (get_current_mode_id() == 1)
		{
			set_current_mode_id(0);
		}
	}
	else if (key == 27) // 'escape'
	{
		// enter menu
		if (get_current_mode_id() != 2)
		{
			prev_mode_id_ = get_current_mode_id();
			set_current_mode_id(2);
		}
		// exit menu
		else
		{				
			if (game_started_)
			{
				set_current_mode_id(prev_mode_id_);
			}
			else
			{
				set_current_mode_id(0);
			}
		}
	}
}

void GamemodeManager::mouse_pressed(const int x, const int y, const int button)
{
	if (get_current_mode_id() == 2)
	{
		if (button == 0 && gui_manager_->main_mode_bounds.intersects(ofRectangle(x, y, 0, 0)))
		{
			set_current_mode_id(1);
			request_for_new_scene = true;
		}
		else if (button == 0 && gui_manager_->sandbox_mode_bounds.intersects(ofRectangle(x, y, 0, 0)))
		{
			set_current_mode_id(0);
			request_for_new_scene = true;
		}
	}
}
