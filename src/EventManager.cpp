#include "EventManager.h"

EventManager::EventManager(): player_gui_visible(false),
                              entity_manager_(nullptr),
                              current_index_(0),
                              tutorial_enabled_(false),
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

void EventManager::setup()
{
	/*if (tutorial_enabled_)
	{
		potta_one_main_.load("PottaOne-Regular.ttf", 12, true, true);
		potta_one_context_.load("PottaOne-Regular.ttf", 10, true, true);

		dialogues_ = {
			"example dialogue",
			"example dialogue 2",
		};
		positions_ = {
			{static_cast<float>(3.95 * static_cast<float>(dialogues_[0].length()) * -1), -125},
			{static_cast<float>(3.95 * static_cast<float>(dialogues_[1].length()) * -1), -125},
		};
	}*/
}

void EventManager::update()
{
	/*if (tutorial_enabled_)
	{
		static bool i0 = false;
		if (!i0)
		{
			i0 = true;
			gui_manager_->set_gui_visible(false);
		}
		static bool i1 = false;
		if (current_index_ >= 1 && !i1)
		{
			i1 = true;
			gui_manager_->set_gui_visible(true);
			can_keypress_ = true;
			full_input_ = true;
		}
	}
	else
	{
		static bool t;
		if (!t)
		{
			t = true;
			full_input_ = true;
		}
	}*/

	if (gamemode_manager_->get_current_mode_string() != "Menu")
	{
		full_input_ = true;
	}
	else
	{
		full_input_ = false;
	}
}

void EventManager::draw_tutorial()
{
	/*if (tutorial_enabled_)
	{
		potta_one_main_.drawString(dialogues_[current_index_], positions_[current_index_].x, positions_[current_index_].y);
		if (current_index_ <= 0) potta_one_context_.drawString("Use left/right arrow keys to navigate the tutorial", -177, 490);
	}*/
}

void EventManager::key_pressed(const int key)
{
	/*if (tutorial_enabled_)
	{
		if (key == 57356 && current_index_ != 0)
		{
			current_index_--;
		}
		else if (key == 57358 && current_index_ != dialogues_.size() - 1)
		{
			current_index_++;
		}
	}*/
}

void EventManager::show_tutorial(const bool value)
{
	(value == 0) ? tutorial_enabled_ = false : tutorial_enabled_ = true;
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
