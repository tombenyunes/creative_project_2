#pragma once

#include "ofMain.h"
#include "GameObject.h"

class PlayerTrail : public GameObject
{
public:

	PlayerTrail(ofVec2f pos, ofVec2f vel, float radius, ofColor color, int lifetime);

private:

	void update() override;
	void draw() override;

	int lifetime_;
	ofVec2f friction_;
};
