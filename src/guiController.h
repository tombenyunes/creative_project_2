#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Controller.h"

class guiController {

public:

	guiController();
	void update(Controller* _controller);
	void updateWorld();
	void updateValues(ofVec2f _pos, ofVec2f _vel, ofVec2f _accel, float _mass, bool _infmass, float _radius, bool _affectedByGravity, int panel);
	void updateMultipleValues(ofVec2f _anchorpos, ofVec2f _nodePos1, ofVec2f _nodeVel1, ofVec2f _nodeAccel1, float _nodeMass1, float _nodeRadius1, ofVec2f _nodePos2, ofVec2f _nodeVel2, ofVec2f _nodeAccel2, float _nodeMass2, float _nodeRadius2, float _k, float _damping, float _springmass, bool _affectedByGravity);
	void updateCreateNodeValues();
	
	void windowResized(int w, int h);
	
	void setClearAll();

	Controller* GameController;

	// ----- Panels ----- //

	ofxPanel world_gui;
	ofxPanel player_gui;
	ofxPanel selected_gui;

	ofxPanel multi_selection_gui_anchor;
	ofxPanel multi_selection_gui_node1;
	ofxPanel multi_selection_gui_node2;
	ofxPanel multi_selection_gui_spring;

	ofxPanel create_node_gui;

	// ----- Parameters ----- //
	
	// World
	ofxButton newScene;
	ofxToggle gravity;
	ofxToggle hardCollisions;
	
	// Player
	ofxLabel position;
	ofxLabel velocity;
	ofxLabel accel;
	ofxFloatSlider mass;
	ofxToggle infiniteMass;
	ofxFloatSlider radius;
	ofxToggle affectedByGravity;

	// Selected Node
	ofxLabel selected_position;
	ofxLabel selected_velocity;
	ofxLabel selected_accel;
	ofxFloatSlider selected_mass;
	ofxToggle selected_infiniteMass;
	ofxFloatSlider selected_radius;
	ofxToggle selected_affectedByGravity;

	// Multiple Selected Nodes
	// anchor (root position)
	ofxLabel anchorPos;

	// node 1
	ofxLabel nodePos1;
	ofxLabel nodeVel1;
	ofxLabel nodeAccel1;
	ofxFloatSlider nodeRadius1;
	ofxFloatSlider nodeMass1;
	
	// node 2
	ofxLabel nodePos2;
	ofxLabel nodeVel2;
	ofxLabel nodeAccel2;
	ofxFloatSlider nodeRadius2;
	ofxFloatSlider nodeMass2;

	// spring settings
	ofxFloatSlider k;
	ofxFloatSlider damping;
	ofxFloatSlider springmass;
	ofxToggle spring_affectedByGravity;

	// Create New Node
	ofxLabel name;
	ofxLabel howToMove;
	ofxLabel howToSelect;
	ofxLabel howToDrag;
	ofxLabel howToCreate;
	ofxLabel howToChangeType;
	ofxLabel howToDelete;

	int buffer;

};