#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Springs : public GameObject {
	
public:

	Springs(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass, Controller* _controller);

	void update() override;

	void screenBounce() override;
	void gravity() override;
	void mouseHover() override;
	void ellipseCollider() override;
	void isColliding(GameObject* _other, int _node);

	void draw() override;
	void getNodeColor(int _node);

	void mousePressed(float _x, float _y, int _button) override;
	void mouseReleased(float _x, float _y, int _button) override;

	/*ofVec2f nodePos1;
	ofVec2f nodeVel1;
	ofVec2f nodeAccel1;
	float nodeRadius1;
	float nodeMass1;

	ofVec2f nodePos2;
	ofVec2f nodeVel2;
	ofVec2f nodeAccel2;
	float nodeRadius2;
	float nodeMass2;*/


private:

	void updateForces();
	void applyAllForces(int _node);
	ofVec2f updateSprings(int _node);
	ofVec2f applyFriction(int _node);
	void addForces();

	void dragNodes();

	void updateGUI();
	void resetForces();

	float k;
	float damping;
	float springmass;
	float timeStep;

	bool mouse_down_triggered;
	bool initiai_values_triggered;
	bool mouseOverNode1;
	bool mouseOverNode2;
	bool mouseOverAnchor;
	bool mouseDragNode1;
	bool mouseDragNode2;
	bool mouseDragAnchor;


};