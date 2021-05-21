#include "EventManager.h"

EventManager::EventManager(): player_gui_visible(false),
                              entity_manager_(nullptr),
                              current_index_(0),
                              can_lmb_(false),
                              can_select_(false),
                              can_drag_(false),
                              can_keypress_(false),
                              full_input_(false)
{
}

void EventManager::init(EntityManager* entity_manager, GUIManager* gui_manager, GamemodeManager* gamemode_manager)
{
	entity_manager_ = entity_manager;
	gui_manager_ = gui_manager;
	gamemode_manager_ = gamemode_manager;
}

void EventManager::update()
{
	if (gamemode_manager_->get_current_mode_string() != "Menu" && gamemode_manager_->get_is_transitioning() != true)
	{
		full_input_ = true;
	}
	else
	{
		full_input_ = false;
	}
}

bool EventManager::is_event_allowed(const string event, const int button) const
{
	if (event == "key_pressed")
	{
		if ((full_input_) || (can_keypress_))
		{
			return true;
		}
	}
	else if (event == "key_released")
	{
		if ((full_input_) || (can_keypress_))
		{
			return true;
		}
	}
	else if (event == "mouse_dragged")
	{
		if ((full_input_) || (button == 0 && can_lmb_) || (button == 2 && can_drag_))
		{
			return true;
		}
	}
	else if (event == "mouse_pressed")
	{
		if ((full_input_) || (button == 0 && can_lmb_) || (button == 2 && can_select_))
		{
			return true;
		}
	}
	else if (event == "mouse_scrolled")
	{
		if (full_input_)
		{
			return true;
		}
	}
	else if (event == "mouse_released")
	{
		if ((full_input_) || (button == 0 && can_lmb_) || (button == 2 && can_select_))
		{
			return true;
		}
	}

	return false;
}
