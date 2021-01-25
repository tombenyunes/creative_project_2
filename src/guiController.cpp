#include "guiController.h"

guiController::guiController()
{
	buffer = 5; // buffer between all panels and each other + screen edges
	string errorMessage = "Error: Updating failed"; // error message will show for all parameters that require but haven't received an update
	int errorInt = 404;

	ofVec2f kBounds = { 0.1, 20 };
	ofVec2f dampingBounds = { 0.1, 8 };
	ofVec2f springmassBounds = { 0.1, 50 };

	newScene.addListener(this, &guiController::setClearAll);

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

	multi_selection_gui_anchor.setup("Selected Object", "", ofGetWidth() - multi_selection_gui_anchor.getWidth() - buffer, buffer);
	multi_selection_gui_anchor.add(anchorPos.setup("pos", errorMessage));

	multi_selection_gui_node1.setup("Node 1", "", ofGetWidth() - multi_selection_gui_node1.getWidth() - buffer, multi_selection_gui_anchor.getPosition().y + multi_selection_gui_anchor.getHeight() + buffer);
	multi_selection_gui_node1.add(nodePos1.setup("pos", errorMessage));
	multi_selection_gui_node1.add(nodeVel1.setup("vel", errorMessage));
	multi_selection_gui_node1.add(nodeAccel1.setup("accel", errorMessage));
	multi_selection_gui_node1.add(nodeMass1.setup("mass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	multi_selection_gui_node1.add(nodeRadius1.setup("radius", errorInt, RADIUS_MINIMUM, RADIUS_MAXIMUM));
	
	multi_selection_gui_node2.setup("Node 2", "", ofGetWidth() - multi_selection_gui_node2.getWidth() - buffer, multi_selection_gui_node1.getPosition().y + multi_selection_gui_node1.getHeight() + buffer);
	multi_selection_gui_node2.add(nodePos2.setup("pos", errorMessage));
	multi_selection_gui_node2.add(nodeVel2.setup("vel", errorMessage));
	multi_selection_gui_node2.add(nodeAccel2.setup("accel", errorMessage));
	multi_selection_gui_node2.add(nodeMass2.setup("mass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	multi_selection_gui_node2.add(nodeRadius2.setup("radius", errorInt, RADIUS_MINIMUM, RADIUS_MAXIMUM));

	multi_selection_gui_spring.setup("Spring Settings", "", ofGetWidth() - multi_selection_gui_spring.getWidth() - buffer, multi_selection_gui_node2.getPosition().y + multi_selection_gui_node2.getHeight() + buffer);
	multi_selection_gui_spring.add(k.setup("springiness", errorInt, kBounds.x, kBounds.y));
	multi_selection_gui_spring.add(damping.setup("damping", errorInt, dampingBounds.x, dampingBounds.y));
	multi_selection_gui_spring.add(springmass.setup("springmass", errorInt, MINIMUM_MASS, MAXIMUM_MASS));
	multi_selection_gui_spring.add(spring_affectedByGravity.setup("gravity", false));

	create_node_gui.setup("Create", "", ofGetWidth() / 2 - create_node_gui.getWidth() / 2, buffer);
	create_node_gui.add(howToMove.setup("", "Hold M1 to Move Player"));
	create_node_gui.add(howToDrag.setup("", "Hold M2 to Drag Node"));
	create_node_gui.add(howToSelect.setup("", "Press M2 to Select Node"));
	create_node_gui.add(howToDelete.setup("", "Press 'x' to Delete Node"));
	create_node_gui.add(howToCreate.setup("", "Press 'c' to Create Node"));
	create_node_gui.add(howToChangeType.setup("", "MouseWheel Changes Type:"));
	create_node_gui.add(name.setup("Type", errorMessage));
}

void guiController::update(Controller* _controller)
{
	GameController = _controller;
	updateWorld();
	updateCreateNodeValues();
}

void guiController::updateWorld()
{
	GameController->setGravity(gravity);
	GameController->setUseHardCollisions(hardCollisions);
}

void guiController::updateCreateNodeValues()
{
	switch (GameController->getNewNodeType()) {
		case 0:
			name = "Mass";
			break;
		case 1:
			name = "Spring";
			break;
	}
}

void guiController::updateValues(ofVec2f _pos, ofVec2f _vel, ofVec2f _accel, float _mass, bool _infmass, float _radius, bool _affectedByGravity, int panel)
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

void guiController::updateMultipleValues(ofVec2f _anchorpos, ofVec2f _nodePos1, ofVec2f _nodeVel1, ofVec2f _nodeAccel1, float _nodeMass1, float _nodeRadius1, ofVec2f _nodePos2, ofVec2f _nodeVel2, ofVec2f _nodeAccel2, float _nodeMass2, float _nodeRadius2, float _k, float _damping, float _springmass, bool _affectedByGravity)
{
	anchorPos = ofToString(roundf(_anchorpos.x)) + ", " + ofToString(roundf(_anchorpos.y));
	
	nodePos1 = ofToString(roundf(_nodePos1.x)) + ", " + ofToString(roundf(_nodePos1.y));
	nodeVel1 = ofToString(roundf(_nodeVel1.x * 100) / 100) + ", " + ofToString(roundf(_nodeVel1.y * 100) / 100);
	nodeAccel1 = ofToString(roundf(_nodeAccel1.x * 10000) / 10000) + ", " + ofToString(roundf(_nodeAccel1.y * 10000) / 10000);
	nodeMass1 = _nodeMass1;
	nodeRadius1 = _nodeRadius1;
	
	nodePos2 = ofToString(roundf(_nodePos2.x)) + ", " + ofToString(roundf(_nodePos2.y));
	nodeVel2 = ofToString(roundf(_nodeVel2.x * 100) / 100) + ", " + ofToString(roundf(_nodeVel2.y * 100) / 100);
	nodeAccel2 = ofToString(roundf(_nodeAccel2.x * 10000) / 10000) + ", " + ofToString(roundf(_nodeAccel2.y * 10000) / 10000);
	nodeMass2 = _nodeMass2;
	nodeRadius2 = _nodeRadius2;

	k = _k;
	damping = _damping;
	springmass = _springmass;
	spring_affectedByGravity = _affectedByGravity;
}

void guiController::windowResized(int w, int h)
{
	create_node_gui.setPosition(ofGetWidth() / 2 - create_node_gui.getWidth() / 2, buffer);
	selected_gui.setPosition(ofGetWidth() - selected_gui.getWidth() - buffer, buffer);
	multi_selection_gui_anchor.setPosition(ofGetWidth() - multi_selection_gui_anchor.getWidth() - buffer, buffer);
	multi_selection_gui_node1.setPosition(ofGetWidth() - multi_selection_gui_node1.getWidth() - buffer, multi_selection_gui_anchor.getPosition().y + multi_selection_gui_anchor.getHeight()+ buffer);
	multi_selection_gui_node2.setPosition(ofGetWidth() - multi_selection_gui_node2.getWidth() - buffer, multi_selection_gui_node1.getPosition().y + multi_selection_gui_node1.getHeight() + buffer);
	multi_selection_gui_spring.setPosition(ofGetWidth() - multi_selection_gui_spring.getWidth() - buffer, multi_selection_gui_node2.getPosition().y + multi_selection_gui_node2.getHeight() + buffer);
}

void guiController::setClearAll()
{
	GameController->setDeleteAll(true);
}