#pragma once

#include "ofMain.h"
#include "GameObject.h"
#include "PlayerTrail.h"
#include "Mass.h"

class Collectable : public GameObject {

public:

	Collectable(ofVec2f pos, float mass, float radius);	

private:

	void update() override;

	// Physics/movement
	void update_forces();
	void reset_forces();
	void drag_nodes();

	// GUI
	void update_gui();
	
	// Draw
	void draw() override;
	
	void get_color() const;

	// Events
	void mouse_pressed(float x, float y, int button) override;
	void mouse_dragged(float x, float y, int button) override;
	void mouse_released(float x, float y, int button) override;	

	// Collisions (pull range)
	void is_colliding(GameObject* other, ofVec2f node_pos = { 0, 0 }) override;

	void random_forces();
	void pulse_radius();
	void draw_particle_burst() const;

	bool is_active_;
	float emission_frequency_;
	float emission_force_;
	float starting_radius_;
	bool needs_to_pulse_radius_;
	bool player_within_bounds_;

	vector<PlayerTrail*>* particles_;
};