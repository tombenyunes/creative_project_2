#pragma once

#include "AudioManager.h"
#include "Controller.h"
#include "Camera.h"
#include "FluidManager.h"
#include "GamemodeManager.h"
#include "ofMain.h"
#include "ofxGui.h"

class GameObject;

class GUIManager
{
public:

	GUIManager();

	void init(Controller* controller, FluidManager* fluid_manager, AudioManager* audio_manager, GamemodeManager* game_mode_manager, Camera* cam);
	void update();
	
	// Update Values
	void update_world();
	void update_values(ofVec2f node_position, ofVec2f node_velocity, ofVec2f node_acceleration, float _node_mass, bool infmass, float _node_radius, bool is_affected_by_gravity, int panel);
	void update_spring_values(ofVec2f spring_anchor_position, float spring_k, float spring_damping, float spring_mass, bool is_affected_by_gravity, ofVec2f selected_node_pos = ofVec2f(-1, -1), ofVec2f selected_node_vel = ofVec2f(-1, -1), ofVec2f selected_node_accel = ofVec2f(-1, -1), float selected_node_mass = -1, float selected_node_radius = -1);
	void update_create_node_values();
	void inc_points_collected();
	void inc_max_point_count();
	void update_point_count(int count);

	// Draw
	void draw_required_gui(GameObject* selected_object, const bool is_spring);

	// Events
	void key_pressed(int key);
	void window_resized(int w, int h);
	
	void prepare_for_new_scene();

	
	// ----- Accessible Parameters ----- //
	
	// Player
	ofxLabel position;
	ofxLabel velocity;
	ofxLabel acceleration;
	ofxFloatSlider mass;
	ofxToggle infinite_mass;
	ofxFloatSlider radius;
	ofxToggle affected_by_gravity;

	// Selected Node
	ofxLabel selected_position;
	ofxLabel selected_velocity;
	ofxLabel selected_accel;
	ofxFloatSlider selected_mass;
	ofxToggle selected_infinite_mass;
	ofxFloatSlider selected_radius;
	ofxToggle selected_affected_by_gravity;


	// Multiple Selected Nodes
	// anchor (root position)
	ofxLabel anchor_pos;
	// spring settings
	ofxFloatSlider k;
	ofxFloatSlider damping;
	ofxFloatSlider springmass;
	ofxToggle spring_affected_by_gravity;
	// selected node from spring
	ofxLabel node_pos;
	ofxLabel node_vel;
	ofxLabel node_accel;
	ofxFloatSlider node_radius;
	ofxFloatSlider node_mass;
	// check if node is selected
	bool multi_node_selected;

private:		

	// Draw
	void draw_text() const;
	void draw_border() const;

	// Events
	void set_clear_all();

	Controller* game_controller_{};
	FluidManager* fluid_manager_{};
	AudioManager* audio_manager_{};
	GamemodeManager* game_mode_manager_{};

	Camera* cam_{};


	
	int point_count_;
	int points_collected_;
	int max_point_count_;				

	// ----- Panels ----- //

	ofxPanel world_gui_;
	ofxPanel player_gui_;
	ofVec2f player_gui_position_;
	ofxPanel selected_gui_;
	ofVec2f selected_gui_position_;

	ofxPanel multi_selection_gui_spring_;
	ofxPanel multi_selection_gui_node_;

	ofxPanel create_node_gui_;

	// ----- Parameters ----- //
	
	// World
	ofxButton new_scene_;
	ofxToggle gravity_;
	ofxToggle hard_collisions_;

	// Create New Node
	ofxLabel name_;
	ofxLabel how_to_move_;
	ofxLabel how_to_select_;
	ofxLabel how_to_drag_;
	ofxLabel how_to_create_;
	ofxLabel how_to_change_type_;
	ofxLabel how_to_delete_;

	int buffer_;

	bool draw_particle_gui_;
	bool draw_audio_gui_;

};
