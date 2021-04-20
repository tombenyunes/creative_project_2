#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Player : public GameObject {

public:

	Player(ofVec2f pos = { 0, 0 }, ofColor color = ofColor(255));

private:
	
	void update() override;

	// Physics/movement
	void update_forces();
	void apply_all_forces();
	void reset_forces();
	
	bool player_can_move() const;
	ofVec2f get_movement_vector() const;

	// Misc
	void pull_points();
	void boost_player();

	// GUI
	void update_gui();

	// Draw
	void draw() override;
	
	void draw_boost_direction() const;
	ofVec3f draw_vel_path() const;
	void draw_particle_trail() const;

	// Events
	void mouse_pressed(float x, float y, int button) override;
	void mouse_dragged(float x, float y, int button) override;
	void mouse_released(float x, float y, int button) override;
	
	void key_pressed(int key) override;
	void key_released(int key) override;


	
	bool mouse_down_;
	int mouse_button_;
	ofVec2f mouse_pos_;
	
	bool aiming_boost_;

};