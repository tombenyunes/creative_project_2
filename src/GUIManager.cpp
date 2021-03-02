#include "GUIManager.h"

GUIManager::GUIManager()
{
	buffer = 5; // buffer between all panels and each other + screen edges
	string errorMessage = "Error: Updating failed"; // error message will show for all parameters that require but haven't received an update
	int errorInt = 404;

	ofVec2f kBounds = { 0.1, 20 };
	ofVec2f dampingBounds = { 0.1, 8 };
	ofVec2f springmassBounds = { 0.1, 50 };

	newScene.addListener(this, &GUIManager::setClearAll);

	world_gui.setup("World", "", buffer, buffer);
	world_gui.add(newScene.setup("clear all"));
	world_gui.add(gravity.setup("global gravity", false));
	world_gui.add(hardCollisions.setup("hard collisions", false));
	
	player_gui.setup("Player", "", world_gui.getPosition().x + world_gui.getWidth() + buffer, buffer);
	player_gui.add(position.setup("pos", errorMessage));
	player_gui.add(velocity.setup("vel", errorMessage));
	player_gui.add(accel.setup("accel", errorMessage));
	player_gui.add(mass.setup("mass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	player_gui.add(infiniteMass.setup("infinite mass", false));
	player_gui.add(radius.setup("radius", errorInt, RADIUS_MINIMUM, RADIUS_MAXIMUM));
	player_gui.add(affectedByGravity.setup("gravity", false));

	selected_gui.setup("Selected Object", "", ofGetWidth() - selected_gui.getWidth() - buffer, buffer);
	selected_gui.add(selected_position.setup("pos", errorMessage));
	selected_gui.add(selected_velocity.setup("vel", errorMessage));
	selected_gui.add(selected_accel.setup("accel", errorMessage));
	selected_gui.add(selected_mass.setup("mass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	selected_gui.add(selected_infiniteMass.setup("infinite mass", false));
	selected_gui.add(selected_radius.setup("radius", errorInt, RADIUS_MINIMUM, RADIUS_MAXIMUM));
	selected_gui.add(selected_affectedByGravity.setup("gravity", false));	

	multi_selection_gui_spring.setup("Spring Settings", "", ofGetWidth() - multi_selection_gui_spring.getWidth() - buffer, buffer);
	multi_selection_gui_spring.add(anchorPos.setup("anchor pos", errorMessage));
	multi_selection_gui_spring.add(k.setup("springiness", errorInt, kBounds.x, kBounds.y));
	multi_selection_gui_spring.add(damping.setup("damping", errorInt, dampingBounds.x, dampingBounds.y));
	multi_selection_gui_spring.add(springmass.setup("springmass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	multi_selection_gui_spring.add(spring_affectedByGravity.setup("gravity", false));

	multi_selection_gui_node.setup("Selected Node", "", ofGetWidth() - multi_selection_gui_spring.getWidth() - buffer, multi_selection_gui_spring.getPosition().y + multi_selection_gui_spring.getHeight() + buffer);
	multi_selection_gui_node.add(nodePos.setup("pos", errorMessage));
	multi_selection_gui_node.add(nodeVel.setup("vel", errorMessage));
	multi_selection_gui_node.add(nodeAccel.setup("accel", errorMessage));
	multi_selection_gui_node.add(nodeMass.setup("mass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	multi_selection_gui_node.add(nodeRadius.setup("radius", errorInt, RADIUS_MINIMUM, RADIUS_MAXIMUM));

	create_node_gui.setup("Create", "", ofGetWidth() / 2 - create_node_gui.getWidth() / 2, buffer);
	create_node_gui.add(howToMove.setup("", "Hold M1 to Move Player"));
	create_node_gui.add(howToDrag.setup("", "Hold M2 to Drag Node"));
	create_node_gui.add(howToSelect.setup("", "Press M2 to Select Node"));
	create_node_gui.add(howToDelete.setup("", "Press 'x' to Delete Node"));
	create_node_gui.add(howToCreate.setup("", "Press 'c' to Create Node"));
	create_node_gui.add(howToChangeType.setup("", "MouseWheel Changes Type:"));
	create_node_gui.add(name.setup("Type", errorMessage));
}

void GUIManager::update(Controller* _controller)
{
	GameController = _controller;
	updateWorld();
	updateCreateNodeValues();
}

void GUIManager::updateWorld()
{
	GameController->setGravity(gravity);
	GameController->setUseHardCollisions(hardCollisions);
}

void GUIManager::updateCreateNodeValues()
{
	switch (GameController->getNewNodeType()) {
		case 0:
			name = "Mass";
			break;
		case 1:
			name = "Spring";
			break;
		case 2:
			name = "Point";
			break;
	}
}

void GUIManager::updateValues(ofVec2f _pos, ofVec2f _vel, ofVec2f _accel, float _mass, bool _infmass, float _radius, bool _affectedByGravity, int panel)
{
	if (panel == 1) {
		position = ofToString(roundf(_pos.x)) + ", " + ofToString(roundf(_pos.y));
		velocity = ofToString(roundf(_vel.x * 100) / 100) + ", " + ofToString(roundf(_vel.y * 100) / 100);
		accel = ofToString(roundf(_accel.x * 10000) / 10000) + ", " + ofToString(roundf(_accel.y * 10000) / 10000);
		if (_infmass) {
			mass.setTextColor(0);
			infiniteMass = true;
		}
		else {
			mass.setTextColor(255);
			mass = _mass;
			infiniteMass = false;
		}
		radius = _radius;
		affectedByGravity = _affectedByGravity;
	}
	else if (panel == 2) {
		selected_position = ofToString(roundf(_pos.x)) + ", " + ofToString(roundf(_pos.y));
		selected_velocity = ofToString(roundf(_vel.x * 100) / 100) + ", " + ofToString(roundf(_vel.y * 100) / 100);
		selected_accel = ofToString(roundf(_accel.x * 10000) / 10000) + ", " + ofToString(roundf(_accel.y * 10000) / 10000);
		if (_infmass) {
			selected_mass.setTextColor(0);
			selected_infiniteMass = true;
		}
		else {
			selected_mass.setTextColor(255);
			selected_mass = _mass;
			selected_infiniteMass = false;
		}
		selected_radius = _radius;
		selected_affectedByGravity = _affectedByGravity;
	}
}

void GUIManager::updateSpringValues(ofVec2f _anchorpos, float _k, float _damping, float _springmass, bool _affectedByGravity, ofVec2f _selectedNodePos, ofVec2f _selectedNodeVel, ofVec2f _selectedNodeAccel, float _selectedNodeMass, float _selectedNodeRadius)
{
	anchorPos = ofToString(roundf(_anchorpos.x)) + ", " + ofToString(roundf(_anchorpos.y));

	k = _k;
	damping = _damping;
	springmass = _springmass;
	spring_affectedByGravity = _affectedByGravity;

	if (_selectedNodePos == ofVec2f(-1, -1) && _selectedNodeVel == ofVec2f(-1, -1) && _selectedNodeAccel == ofVec2f(-1, -1) && _selectedNodeMass == -1 && _selectedNodeRadius == -1) {
		// can't draw
		multiNodeSelected = false;
	}
	else {
		multiNodeSelected = true;
		nodePos = ofToString(roundf(_selectedNodePos.x)) + ", " + ofToString(roundf(_selectedNodePos.y));
		nodeVel = ofToString(roundf(_selectedNodeVel.x * 100) / 100) + ", " + ofToString(roundf(_selectedNodeVel.y * 100) / 100);
		nodeAccel = ofToString(roundf(_selectedNodeAccel.x * 10000) / 10000) + ", " + ofToString(roundf(_selectedNodeAccel.y * 10000) / 10000);
		nodeMass = _selectedNodeMass;
		nodeRadius = _selectedNodeRadius;
	}
}

void GUIManager::windowResized(int w, int h)
{
	create_node_gui.setPosition(ofGetWidth() / 2 - create_node_gui.getWidth() / 2, buffer);
	selected_gui.setPosition(ofGetWidth() - selected_gui.getWidth() - buffer, buffer);
	multi_selection_gui_spring.setPosition(ofGetWidth() - multi_selection_gui_spring.getWidth() - buffer, buffer);
	multi_selection_gui_node.setPosition(ofGetWidth() - multi_selection_gui_spring.getWidth() - buffer, multi_selection_gui_spring.getPosition().y + multi_selection_gui_spring.getHeight() + buffer);
}

void GUIManager::setClearAll()
{
	GameController->setDeleteAll(true);
}