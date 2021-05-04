#pragma once

#include "ofMain.h"

class PlayerTrail
{
public:

	PlayerTrail(ofVec2f pos, ofVec2f vel, float radius, ofColor color, int lifetime);
	~PlayerTrail() = default;

private:

	void update();
	void draw();

	ofVec2f pos_;
	ofVec2f vel_;
	ofVec2f accel_;
	float radius_;
	ofColor color_;
	
	int lifetime_;
	ofVec2f friction_;

};
