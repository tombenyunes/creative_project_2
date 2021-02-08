#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Player : public GameObject {

public:

	Player(ofVec2f _pos = { 0, 0 }, ofColor _color = ofColor(255));

	void update() override;

	void mousePressed(float _x, float _y, int _button) override;
	void mouseDragged(float _x, float _y, int _button) override;
	void mouseReleased(float _x, float _y, int _button) override;
	void keyPressed(int key) override;
	void keyReleased(int key) override;

	void draw() override;

private:
	
	void updateForces();
	void applyAllForces();

	bool playerCanMove();
	ofVec2f getMovementVector();

	void updateGUI();
	void resetForces();

	void boostPlayer();

	void drawBoostDirection();
	ofVec3f drawVelPath();
	void drawParticleTrail();

	bool mouse_down;
	int mouse_button;
	ofVec2f mouse_pos;
	
	bool aimingBoost;

};