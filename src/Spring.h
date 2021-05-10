#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Spring : public GameObject {
	
public:

	Spring(ofVec2f anchor_pos, vector<float> node_radiuses, vector<float> node_masses, float k, float damping, float springmass);

private:	

	void update() override;	

	// Physics/spring calculations
	void update_forces();
	void apply_all_forces();
	ofVec2f update_springs(int node);
	void add_forces();
	void reset_forces();

	void drag_nodes();
	void create_node(ofVec2f node_pos, ofVec2f node_vel, ofVec2f node_accel, float node_radius, float node_mass);

	// Collision
	void ellipse_collider() override;
	void is_colliding(GameObject* other, int node_index);

	// GUI
	void update_gui();

	// Draw
	void draw() override;
	
	void get_node_color(int node_index);
	void draw_connecting_lines();
	float angle_between(ofVec2f from, ofVec2f to) const;
	ofVec2f get_point_on_circle(ofVec2f center, float radians, float  radius) const;
	
	// Events
	void mouse_pressed(float x, float y, int button) override;
	void mouse_dragged(float x, float y, int button) override;
	void mouse_released(float x, float y, int button) override;
	
	void key_pressed(int key) override;


	
	float k_;
	float damping_;
	float springmass_;
	float time_step_;	

	bool fill_ellipses_;
	
	vector<ofVec2f> fluid_velocities_;

};