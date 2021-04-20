#pragma once

#include "AudioManager.h"
#include "Collisions.h"
#include "Controller.h"
#include "FluidManager.h"
#include "GUIManager.h"
#include "ofMain.h"

class GameObject {
	
public:

	GameObject(ofVec2f pos = { 0, 0 }, ofColor color = ofColor(255));
	void init(vector<GameObject*>* gameobjects, Controller* controller, GUIManager* gui_manager, Camera* cam, FluidManager* fluid_manager, AudioManager* audio_manager);

	void root_update();
	void root_draw();

	void root_key_pressed(int key);
	void root_key_released(int key);

	virtual void mouse_pressed(float x, float y, int button) {}
	virtual void mouse_dragged(float x, float y, int button) {}
	virtual void mouse_released(float x, float y, int button) {}

	string get_type() const                                  { return type_; }
	void set_type(const string type)                         { type_ = type;  }
	
	ofVec2f get_position() const                             { return pos_; }
	void set_position(const ofVec2f pos)                     { pos_ = pos; }

	void set_velocity(const ofVec2f vel)					 { vel_ = vel; }
	
	float get_radius() const                                 { return radius_; }
	void set_radius(const float radius)                      { radius_ = radius; }
	float get_mass() const						             { return mass_; }
	void set_mass(const float mass)                          { mass_ = mass; }

	ofColor get_color() const                                { return color_; }
	void set_color(const ofColor color)                      { color_ = color; }
	
	vector<ofVec2f> get_multiple_positions() const           { return node_positions_; }
	vector<float> get_multiple_radiuses() const	             { return node_radiuses_; }
	vector<float> get_multiple_masses() const                { return node_masses_; }	

	bool get_request_to_be_deleted() const                   { return request_to_be_deleted_; }
	void set_request_to_be_deleted(const bool req)           { request_to_be_deleted_ = req; }
	string get_request_to_be_deleted_event() const           { return request_to_be_deleted_event_; }
	void set_request_to_be_deleted_event(const string event) { request_to_be_deleted_event_ = event; }

	bool get_request_to_be_selected() const                  { return request_to_be_selected_; }
	void set_request_to_be_selected(const bool req)          { request_to_be_selected_ = req; }
	bool get_is_selected() const                             { return is_selected_; }
	void set_is_selected(const bool val)                     { is_selected_ = val; }

	bool can_collide() const					             { return ellipse_collider_enabled_; }
	
	virtual void is_colliding(GameObject* other, ofVec2f node_pos = { 0, 0 });

protected:

	void add_module(string id);	
	
	virtual void apply_force(ofVec2f& accel, ofVec2f force, bool limit = true, float limit_amount = MAXIMUM_ACCELERATION);
	virtual void add_forces(bool interp_pos);
	virtual ofVec2f get_interpolated_position();

	virtual void update(){}
	virtual void draw(){}

	virtual void key_pressed(int key){}
	virtual void key_released(int key){}

	vector<GameObject*>* game_objects_;
	Controller* game_controller_;
	GUIManager* gui_manager_;
	FluidManager* fluid_manager_;
	AudioManager* audio_manager_;

	Camera* cam_;

	Collisions collision_detector_;

	string type_;
	
	ofVec2f pos_;
	ofVec2f prev_pos_;
	float mass_;
	float radius_;

	vector<ofVec2f> node_positions_;
	vector<ofVec2f> node_velocities_;
	vector<ofVec2f> node_accelerations_;
	vector<float> node_radiuses_;
	vector<float> node_masses_;
	
	ofVec2f vel_;
	ofVec2f accel_;
	
	ofColor color_;
	
	bool infinite_mass_;
	bool affected_by_gravity_;
	int gravity_mult_;
	int collision_mult_;
		
	bool mouse_over_;
	int mouse_over_index_;
	int selected_node_index_;

	bool mouse_drag_;
	ofVec2f mouse_offset_from_center_;
	ofVec2f pos_before_drag_;
	bool started_dragging_;

	ofColor passive_color_;
	ofColor selected_color_;

	bool delete_key_down_;

	bool request_to_be_deleted_;
	string request_to_be_deleted_event_;
	bool request_to_be_selected_;
	bool is_selected_;

	bool gui_values_need_to_be_set_;

private:
	
	// Modules
	virtual void screen_wrap();
	virtual void screen_bounce();
	virtual void gravity();
	virtual void friction();
	virtual void mouse_hover();
	virtual void ellipse_collider();

	bool screen_wrap_enabled_;
	bool screen_bounce_enabled_;
	bool gravity_enabled_;
	bool friction_enabled_;
	bool mouse_hover_enabled_;
	bool ellipse_collider_enabled_;
	
};
