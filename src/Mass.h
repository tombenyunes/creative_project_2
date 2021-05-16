#pragma once

#include "ofMain.h"
#include "GameObject.h"

class Mass : public GameObject
{
public:

	Mass(ofVec2f pos, float mass, float radius);	

private:

	void update() override;		

	// Physics/movement
	void update_forces();
	ofVec2f get_fluid_force();
	void reset_forces();
	
	void drag_nodes();
	
	// GUI
	void update_gui();	

	// Draw
	void draw() override;
	
	void get_color() const;
	
	// Events
	void mouse_pressed(float x, float y, int button) override;
	void mouse_dragged(float x, float y, int button) override;
	void mouse_released(float x, float y, int button) override;

	ofVec2f force_;
	
};
