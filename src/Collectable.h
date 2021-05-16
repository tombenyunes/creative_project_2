#pragma once

#include "ofMain.h"
#include "GameObject.h"
#include "PlayerTrail.h"
#include "Mass.h"

class Collectable : public GameObject {

public:

	Collectable(ofVec2f pos, float mass, float radius, float emission_frequency, float emission_force, bool is_active);
	Collectable(ofVec2f pos, float mass, float radius, bool is_active);

	~Collectable();

	static void reset_ids()
	{
		first_point_ = true;
		last_id_collected_ = -404;
		cur_id_ = -1;
		points_collected_ = 0;
		collectable_count_ = 0;
	}
	static int get_points_collected()
	{
		return points_collected_;
	}
	static void set_points_collected(const int count)
	{
		Collectable::points_collected_ = count;
	}

private:

	void update() override;
	bool can_emit() const;

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
	void check_if_active();
	void draw_particle_burst() const;

	float get_attribute_by_name(const string name) const override
	{
		if (name == "emission_frequency")
			return emission_frequency_;
		else if (name == "emission_force")
			return emission_force_;
		else if (name == "is_active")
			return is_active_;
		else if (name == "id")
			return id_;
		else if (name == "starting_radius")
			return starting_radius_;
		else
			return -1;
	}	

	static bool first_point();
	static int get_cur_id();
	
	

	bool is_active_;
	bool make_active_on_next_emission_;
	float emission_frequency_;
	float emission_force_;
	float starting_radius_;
	bool needs_to_pulse_radius_;
	bool player_within_bounds_;

	//vector<PlayerTrail*>* particles_;
	float alpha_;
	bool can_be_collected_;
	static bool first_point_;

	int id_{};
	static int last_id_collected_;

	static int cur_id_;
	static int collectable_count_;

	static int points_collected_;
};