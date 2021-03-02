#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Controller.h"

class GUIManager {

public:

	GUIManager();
	void update(Controller* _controller);
	void updateWorld();
	void updateValues(ofVec2f _pos, ofVec2f _vel, ofVec2f _accel, float _mass, bool _infmass, float _radius, bool _affectedByGravity, int panel);
	void updateSpringValues(ofVec2f _anchorpos, float _k, float _damping, float _springmass, bool _affectedByGravity, ofVec2f _selectedNodePos = ofVec2f(-1, -1), ofVec2f _selectedNodeVel = ofVec2f(-1, -1), ofVec2f _selectedNodeAccel = ofVec2f(-1, -1), float _selectedNodeMass = -1, float _selectedNodeRadius = -1);
	void updateCreateNodeValues();
	
	void windowResized(int w, int h);
	
	void setClearAll();

	Controller* GameController;

	// ----- Panels ----- //

	ofxPanel world_gui;
	ofxPanel player_gui;
	ofxPanel selected_gui;

	ofxPanel multi_selection_gui_spring;
	ofxPanel multi_selection_gui_node;

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
	// spring settings
	ofxFloatSlider k;
	ofxFloatSlider damping;
	ofxFloatSlider springmass;
	ofxToggle spring_affectedByGravity;
	// selected node from spring
	ofxLabel nodePos;
	ofxLabel nodeVel;
	ofxLabel nodeAccel;
	ofxFloatSlider nodeRadius;
	ofxFloatSlider nodeMass;
	// check if node is selected
	bool multiNodeSelected;

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