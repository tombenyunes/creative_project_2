#pragma once

#include "ofMain.h"
#include "GameObject.h"
#include "PlayerTrail.h"
#include "Mass.h"

class Collectable : public GameObject {

public:

	Collectable(ofVec2f pos, float mass, float radius, const float emission_frequency = static_cast<int>(ofRandom(25, 100)), const float emission_force = 0.1f, const bool is_active = false);

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

	float get_attribute_by_name(const string name) const override
	{
		if (name == "emission_frequency")
			return emission_frequency_;
		else if (name == "emission_force")
			return emission_force_;
		else if (name == "is_active")
			return is_active_;
		else
			return -1;
	}
	
	

	bool is_active_;
	float emission_frequency_;
	float emission_force_;
	float starting_radius_;
	bool needs_to_pulse_radius_;
	bool player_within_bounds_;

	vector<PlayerTrail*>* particles_;
	float alpha_;
};