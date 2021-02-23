#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Springs : public GameObject {
	
public:

	Springs(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass, Controller* _controller);

	void update() override;
	
	//void mouseHover() override;

	void draw() override;

	void mousePressed(float _x, float _y, int _button) override;
	void mouseReleased(float _x, float _y, int _button) override;	

private:

	void updateForces();
	void applyAllForces();
	ofVec2f updateSprings(int _node);

	void dragNodes();

	void updateGUI();
	void resetForces();

	float k;
	float damping;
	float springmass;
	float timeStep;

	bool mouse_down_triggered;
	bool initiai_values_triggered;
	bool mouseDrag;


};