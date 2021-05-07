#pragma once

#include "AudioManager.h"
#include "Camera.h"
#include "Controller.h"
#include "FluidManager.h"
#include "ofMain.h"
#include "ofxGui.h"

class GameObject;

class GUIManager
{
public:

	GUIManager();

	void init(Controller* controller, FluidManager* fluid_manager, AudioManager* audio_manager, Camera* cam);
	
	void update();
	void update_world();
	
	void update_values(string entity_type, ofVec2f node_position, ofVec2f node_velocity, ofVec2f node_acceleration, float _node_mass, bool infmass, float _node_radius, bool is_affected_by_gravity, int emission_frequency = -1);
	void update_multiple_values(ofVec2f spring_anchor_position, float spring_k, float spring_damping, float spring_mass, bool is_affected_by_gravity, ofVec2f selected_node_pos = ofVec2f(-1, -1), ofVec2f selected_node_vel = ofVec2f(-1, -1), ofVec2f selected_node_accel = ofVec2f(-1, -1), float selected_node_mass = -1, float selected_node_radius = -1);	
	
	void inc_points_collected();
	void inc_max_point_count();
	void update_point_count(int count);
	void reset_point_counters();

	int get_delete_all() const;
	void toggle_delete_all();

	void set_gui_visible(bool value);
	bool get_gui_visible() const;

	// Draw
	void draw_required_gui(GameObject* selected_object, bool is_spring, int new_node_id, string current_gamemode);

	// Events
	void key_pressed(int key);	

	

	// ----- Panels ----- //

	ofxPanel panel_world;
	ofxPanel panel_player;
	ofxPanel panel_node;
	ofxPanel panel_spring_settings;
	ofxPanel panel_spring_node;
	
	// ----- Parameters ----- //

	// World
	ofxButton gui_world_delete_all;
	ofxToggle gui_world_gravity;
	ofxToggle gui_world_hard_collisions;	
	
	// Player
	ofxLabel gui_player_pos;
	ofxLabel gui_player_vel;
	ofxLabel gui_player_accel;
	ofxFloatSlider gui_player_mass;
	ofxToggle gui_player_infinite_mass;
	ofxFloatSlider gui_player_radius;
	ofxToggle gui_player_affected_by_gravity;

	// Selected Node
	ofxLabel gui_node_pos;
	ofxLabel gui_node_vel;
	ofxLabel gui_node_accel;
	ofxFloatSlider gui_node_mass;
	ofxToggle gui_node_infinite_mass;
	ofxFloatSlider gui_node_radius;
	ofxToggle gui_node_affected_by_gravity;
	ofxFloatSlider gui_node_emission_frequency;
	
	// Spring Settings
	ofxLabel gui_spring_anchor_pos;
	ofxFloatSlider gui_spring_k;
	ofxFloatSlider gui_spring_damping;
	ofxFloatSlider gui_spring_springmass;
	ofxToggle gui_spring_affected_by_gravity;
	
	// Spring Node
	ofxLabel gui_spring_node_pos;
	ofxLabel gui_spring_node_vel;
	ofxLabel gui_spring_node_accel;
	ofxFloatSlider gui_spring_node_radius;
	ofxFloatSlider gui_spring_node_mass;	
	


private:		
	
	// Draw
	void draw_text(int new_node_id, string current_gamemode) const;
	void draw_border() const;
	
	Controller* game_controller_{};
	FluidManager* fluid_manager_{};
	AudioManager* audio_manager_{};
	Camera* cam_{};	

	bool gui_visible_;	
	int panel_pixel_buffer_;

	bool draw_particle_gui_;
	bool draw_audio_gui_;

	bool multi_node_selected_;
	
	int point_count_;
	int points_collected_;
	int max_point_count_;

	bool delete_all_;

};
