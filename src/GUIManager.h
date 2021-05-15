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
	void update_player_values(ofVec2f pos, ofVec2f vel, ofVec2f accel, float mass, bool infmass, float radius);
	void update_mass_values(ofVec2f pos, ofVec2f vel, ofVec2f accel, float mass, bool infmass, float radius);
	void update_collectable_values(ofVec2f pos, ofVec2f vel, ofVec2f accel, float radius, float emission_frequency, float emission_force, bool is_active, int id);
	void update_spring_values(ofVec2f anchor_position, float k, float damping, float springmass);
	void update_spring_values(ofVec2f anchor_position, float k, float damping, float springmass, ofVec2f selected_node_pos, ofVec2f selected_node_vel, ofVec2f selected_node_accel, float selected_node_mass, float selected_node_radius);

	int get_max_point_count();
	void set_max_point_count(int count);
	int get_point_count();
	void set_point_count(int count);
	void reset_point_counters();

	int get_delete_all() const;
	void toggle_delete_all();
	void trigger_reset_fluid();

	void set_gui_visible(bool value);
	bool get_gui_visible() const;

	// Draw
	void draw_required_gui(GameObject* selected_object, int new_node_id, string current_gamemode);

	// Events
	void key_pressed(int key);
	void window_resized();

	

	// ----- Panels ----- //

	ofxPanel panel_world;
	ofxPanel panel_player;
	ofxPanel panel_node;
	ofxPanel panel_collectable;
	ofxPanel panel_spring_settings;
	ofxPanel panel_spring_node;
	ofxPanel panel_fluid;
	
	// ----- Parameters ----- //

	// World
	ofxButton gui_world_delete_all;
	ofxToggle gui_world_gravity;
	ofxToggle gui_world_hard_collisions;
	ofxToggle gui_world_calculate_fluid;
	ofxToggle gui_world_calculate_particles;
	ofxToggle gui_world_calculate_entities;
	ofxLabel gui_world_fps;
	ofxLabel gui_world_frametime;
	
	// Player
	ofxLabel gui_player_pos;
	ofxLabel gui_player_vel;
	ofxLabel gui_player_accel;
	ofxFloatSlider gui_player_mass;
	ofxToggle gui_player_infinite_mass;
	ofxFloatSlider gui_player_radius;

	// Node
	ofxLabel gui_node_pos;
	ofxLabel gui_node_vel;
	ofxLabel gui_node_accel;
	ofxFloatSlider gui_node_mass;
	ofxToggle gui_node_infinite_mass;
	ofxFloatSlider gui_node_radius;

	// Collectable
	ofxLabel gui_collectable_pos;
	ofxFloatSlider gui_collectable_radius;
	ofxFloatSlider gui_collectable_emission_frequency;
	ofxFloatSlider gui_collectable_emission_force;
	ofxToggle gui_collectable_is_active;
	ofxFloatField gui_collectable_id;
	
	// Spring Settings
	ofxLabel gui_spring_anchor_pos;
	ofxFloatSlider gui_spring_k;
	ofxFloatSlider gui_spring_damping;
	ofxFloatSlider gui_spring_springmass;
	ofxButton gui_spring_add_node;
	
	// Spring Node
	ofxLabel gui_spring_node_pos;
	ofxLabel gui_spring_node_vel;
	ofxLabel gui_spring_node_accel;
	ofxFloatSlider gui_spring_node_radius;
	ofxFloatSlider gui_spring_node_mass;

	// Fluid
	ofxIntSlider gui_fluid_cells;
	ofxButton gui_fluid_resize_fluid;
	ofxFloatSlider gui_fluid_color_mult;
	ofxFloatSlider gui_fluid_velocity_mult;
	ofxFloatSlider gui_fluid_viscocity;
	ofxFloatSlider gui_fluid_color_diffusion;
	ofxFloatSlider gui_fluid_fade_speed;
	ofxIntSlider gui_fluid_solver_iterations;
	ofxFloatSlider gui_fluid_delta_t;
	ofxIntSlider gui_fluid_draw_mode;
	ofxToggle gui_fluid_do_rgb;
	ofxToggle gui_fluid_do_vorticity_confinement;
	ofxToggle gui_fluid_draw_fluid;
	ofxToggle gui_fluid_draw_particles;
	ofxFloatSlider gui_fluid_vel_draw_mult;
	ofxFloatSlider gui_fluid_vel_draw_threshold;
	ofxFloatSlider gui_fluid_brightness;
	ofxToggle gui_fluid_use_additive_blending;
	ofxToggle gui_fluid_wrap_x;
	ofxToggle gui_fluid_wrap_y;
	ofxFloatSlider gui_fluid_tuio_x_scaler;
	ofxFloatSlider gui_fluid_tuio_y_scaler;

	ofRectangle main_mode_bounds;
	ofRectangle tutorial_mode_bounds;
	ofRectangle sandbox_mode_bounds;


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
	
	bool draw_audio_gui_;

	bool multi_node_selected_;
	
	int points_collected_;
	int max_point_count_;

	bool delete_all_;

	ofTrueTypeFont potta_one_title_;
	ofTrueTypeFont potta_one_main_;

};
