#pragma once

#include "AudioManager.h"
#include "Camera.h"
#include "Controller.h"
#include "ofMain.h"
#include "ofxGui.h"

class GameObject;

class GUIManager
{
public:

	GUIManager();

	void init(Controller* controller, AudioManager* audio_manager, Camera* cam);
	
	void update();
	void update_world();

	void update_player_values(ofVec2f pos, ofVec2f vel, ofVec2f accel, float mass, bool infmass, float radius, bool affected_by_gravity);
	void update_mass_values(ofVec2f pos, ofVec2f vel, ofVec2f accel, float mass, bool infmass, float radius, bool affected_by_gravity);
	void update_collectable_values(ofVec2f pos, ofVec2f vel, ofVec2f accel, float mass, bool infmass, float radius, bool affected_by_gravity, float emission_frequency, float emission_force, bool is_active);
	void update_spring_values(ofVec2f anchor_position, float k, float damping, float springmass, bool affected_by_gravity);
	void update_spring_values(ofVec2f anchor_position, float k, float damping, float springmass, bool affected_by_gravity, ofVec2f selected_node_pos, ofVec2f selected_node_vel, ofVec2f selected_node_accel, float selected_node_mass, float selected_node_radius);
	
	void inc_points_collected();
	void inc_max_point_count();
	void update_point_count(int count);
	void reset_point_counters();

	int get_delete_all() const;
	void toggle_delete_all();

	void set_gui_visible(bool value);
	bool get_gui_visible() const;

	// Draw
	void draw_required_gui(GameObject* selected_object, int new_node_id, string current_gamemode);

	// Events
	void key_pressed(int key);
	void toggle_menu_screen();

	

	// ----- Panels ----- //

	ofxPanel panel_world;
	ofxPanel panel_player;
	ofxPanel panel_node;
	ofxPanel panel_collectable;
	ofxPanel panel_spring_settings;
	ofxPanel panel_spring_node;
	
	// ----- Parameters ----- //

	// World
	ofxButton gui_world_delete_all;
	ofxToggle gui_world_gravity;
	ofxToggle gui_world_hard_collisions;
	ofxToggle gui_world_calculate_fluid;
	ofxToggle gui_world_calculate_particles;
	ofxToggle gui_world_calculate_entities;
	
	// Player
	ofxLabel gui_player_pos;
	ofxLabel gui_player_vel;
	ofxLabel gui_player_accel;
	ofxFloatSlider gui_player_mass;
	ofxToggle gui_player_infinite_mass;
	ofxFloatSlider gui_player_radius;
	ofxToggle gui_player_affected_by_gravity;

	// Node
	ofxLabel gui_node_pos;
	ofxLabel gui_node_vel;
	ofxLabel gui_node_accel;
	ofxFloatSlider gui_node_mass;
	ofxToggle gui_node_infinite_mass;
	ofxFloatSlider gui_node_radius;
	ofxToggle gui_node_affected_by_gravity;

	// Collectable
	ofxLabel gui_collectable_pos;
	ofxLabel gui_collectable_vel;
	ofxLabel gui_collectable_accel;
	ofxFloatSlider gui_collectable_mass;
	ofxToggle gui_collectable_infinite_mass;
	ofxFloatSlider gui_collectable_radius;
	ofxToggle gui_collectable_affected_by_gravity;
	ofxFloatSlider gui_collectable_emission_frequency;
	ofxFloatSlider gui_collectable_emission_force;
	ofxToggle gui_collectable_is_active;
	
	// Spring Settings
	ofxLabel gui_spring_anchor_pos;
	ofxFloatSlider gui_spring_k;
	ofxFloatSlider gui_spring_damping;
	ofxFloatSlider gui_spring_springmass;
	ofxToggle gui_spring_affected_by_gravity;
	ofxButton gui_spring_add_node;
	
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
	void draw_menu() const;
	
	Controller* game_controller_{};
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

	bool showing_menu_;

};
