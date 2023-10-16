#pragma once

#include "ofMain.h"

class PlayerParticle {

public:

	PlayerParticle(ofVec2f _pos, ofVec2f _vel, float _radius, ofColor _color, int _lifetime);

	void update();
	void draw();

	bool needs_to_be_deleted = false;

private:

	ofVec2f pos;
	ofVec2f vel;
	ofVec2f accel;
	float radius;
	int starting_radius;
	ofColor color;
	int starting_alpha;
	int alpha;

	int lifetime;
	int starting_lifetime;
	ofVec2f friction;

};