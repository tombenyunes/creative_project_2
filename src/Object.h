#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Object : public GameObject {
	
public:

	Object(ofVec2f _pos, float _mass, float _radius, Controller* _controller);
	
	void update() override;

	void mousePressed(float _x, float _y, int _button) override;
	void mouseDragged(float _x, float _y, int _button) override;
	void mouseReleased(float _x, float _y, int _button) override;

	void draw() override;

private:

	void updateForces();

	void dragNodes();

	void updateGUI();
	void resetForces();
	
	bool mouse_down_triggered;
	bool initiai_values_triggered;
	
	ofVec2f posBeforeDrag;
	bool mouseDrag;
	bool startedDragging = false;


};