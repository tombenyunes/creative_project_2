#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Point : public GameObject {

public:

	Point(ofVec2f _pos, float _mass, float _radius);

	void update() override;

	void mousePressed(float _x, float _y, int _button) override;
	void mouseDragged(float _x, float _y, int _button) override;
	void mouseReleased(float _x, float _y, int _button) override;

	void draw() override;
	void getColor();

private:

	void updateForces();

	void dragNodes();

	void updateGUI();
	void resetForces();

	virtual void isColliding(GameObject* _other, ofVec2f _nodePos = { 0, 0 }) override;

	bool gui_values_need_to_be_set;

	ofVec2f posBeforeDrag;
	bool startedDragging = false;


};