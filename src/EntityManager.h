#pragma once

// Managers
#include "GameObject.h"
#include "GUIManager.h"
#include "FluidManager.h"
#include "AudioManager.h"
#include "GamemodeManager.h"

// Entity types
#include "Player.h"
#include "Mass.h"
#include "Spring.h"
#include "Collectable.h"

class EntityManager {
	
public:
	EntityManager();
	
	void init(Controller* game_controller, GUIManager* gui_manager, Camera* cam, FluidManager* fluid_manager, AudioManager* audio_manager, GamemodeManager* gamemode_manager);
	vector<GameObject*>* get_game_objects() const;
	GameObject* get_selected_game_object() const;
	void add_game_object(GameObject* _gameobject) const;

	void update();
	void draw_game_objects() const;

	void delete_all(bool exclude_player = true) const;

	void set_new_node_type(int id);
	int get_new_node_type() const;
	void create_entity(string entity_type = "") const;

	int get_point_count() const;
	ofVec2f get_player_position() const;
	void set_player_position(ofVec2f pos);

	void key_pressed(int key);
	void key_released(int key) const;
	void mouse_moved(int x, int y);
	void mouse_dragged(int x, int y, int button) const;
	void mouse_pressed(int x, int y, int button);
	void mouse_scrolled(int x, int y, float scroll_x, float scroll_y);
	void mouse_released(int x, int y, int button) const;

private:

	void delete_game_objects();
	
	void find_selected();
	GameObject* selected_game_object_;
	int selected_game_object_index_;

	vector<GameObject*> vec_;
	vector<GameObject*>* game_objects_ = &vec_; // the main vector of all objects in the scene

	Controller* game_controller_;
	GUIManager* gui_manager_;
	FluidManager* fluid_manager_;
	AudioManager* audio_manager_;
	GamemodeManager* game_mode_manager_;
	Camera* cam_;

	ofVec2f player_position_;

	int new_node_type_id_;


};