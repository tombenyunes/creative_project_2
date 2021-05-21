#pragma once

#include "EntityManager.h"
#include "GUIManager.h"

class EventManager
{
public:

	EventManager();

	void init(EntityManager* entity_manager, GUIManager* gui_manager, GamemodeManager* gamemode_manager);
	
	void update();

	bool is_event_allowed(string event, int button = -1) const;

	bool player_gui_visible;

private:
	
	EntityManager* entity_manager_;
	GUIManager* gui_manager_;
	GamemodeManager* gamemode_manager_;

	vector<string> dialogues_;
	vector<ofVec2f> positions_;
	int current_index_;
	ofTrueTypeFont potta_one_main_;
	ofTrueTypeFont potta_one_context_;
	
	bool can_lmb_;
	bool can_select_;
	bool can_drag_;
	bool can_keypress_;
	bool full_input_;
};
