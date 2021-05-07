#pragma once

#include "ofMain.h"

#define MINIMUM_MASS 1
#define MAXIMUM_MASS 5000

#define MASS_LOWER_BOUND 250
#define MASS_UPPER_BOUND 2500

#define RADIUS_MINIMUM 1
#define RADIUS_MAXIMUM 500

#define RADIUS_LOWER_BOUND 25
#define RADIUS_UPPER_BOUND 250

#define MAXIMUM_ACCELERATION 0.15f // 0.15
#define MAXIMUM_VELOCITY 100 // 15

#define FRICTION_FORCE 0.015f
#define GRAVITY_FORCE 0.0001f

#define WORLD_WIDTH (2000 * 2)
#define WORLD_HEIGHT (1500 * 2)

#define HALF_WORLD_WIDTH (WORLD_WIDTH / 2)
#define HALF_WORLD_HEIGHT (WORLD_HEIGHT / 2)



class Controller {

public:
	
	Controller();

	void set_gravity(bool value);
	bool get_gravity() const;

	void set_mouse_dragged(bool value);
	bool get_mouse_dragged() const;

	void set_use_hard_collisions(bool value);
	bool get_use_hard_collisions() const;	

private:
	
	bool gravity_;
	bool mouse_being_dragged_;
	bool hard_collisions_;
	
};