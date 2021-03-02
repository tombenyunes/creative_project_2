#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Mass : public GameObject {
	
public:

	Mass(ofVec2f _pos, float _mass, float _radius);
	
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
	
	bool gui_values_need_to_be_set;
	
	ofVec2f posBeforeDrag;
	bool startedDragging = false;


};