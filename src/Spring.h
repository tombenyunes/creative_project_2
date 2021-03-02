#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Spring : public GameObject {
	
public:

	Spring(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass);

	void createNode(ofVec2f nodePos, ofVec2f nodeVel, ofVec2f nodeAccel, float nodeRadius, float nodeMass);

	void update() override;

	//void mouseHover() override;
	void ellipseCollider() override;
	void isColliding(GameObject* _other, int _nodeIndex);

	void draw() override;
	void getNodeColor(int _nodeIndex);
	void drawConnectingLines();
	float angleBetween(ofVec2f from, ofVec2f to);
	ofVec2f getPointOnCircle(ofVec2f center, float radians, float  radius);

	void mousePressed(float _x, float _y, int _button) override;
	void mouseDragged(float _x, float _y, int _button) override;
	void mouseReleased(float _x, float _y, int _button) override;
	void keyPressed(int key) override;

private:

	void updateForces();
	void applyAllForces();
	ofVec2f updateSprings(int _node);
	void addForces();

	void dragNodes();

	void updateGUI();
	void resetForces();

	float k;
	float damping;
	float springmass;
	float timeStep;

	ofVec2f posBeforeDrag;
	bool gui_values_need_to_be_set;	

	bool fillEllipses = false;

};