#include "Springs.h"

Springs::Springs(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass, Controller* _controller)
{
	GameController = _controller;
	
	type = "Spring";

	color = ofColor(255);

	mouseOver = false;
	mouseDrag = false;

	mouse_down_triggered = false;
	initiai_values_triggered = false;

	isSpring = true;
	affectedByGravity = true;

	k = _k;
	damping = _damping;
	springmass = _springmass;

	pos = _anchorPos;
	radius = 8;
	timeStep = 0.28;
	
	createNode(pos, ofVec2f(0, 0), ofVec2f(0, 0), _nodeRadius1, _nodeMass1);
	createNode(pos, ofVec2f(0, 0), ofVec2f(0, 0), _nodeRadius2, _nodeMass2);
	
	AddModule("screenBounce");
	AddModule("ellipseCollider");
	AddModule("gravity");
	AddModule("friction");
	AddModule("mouseHover");
}

void Springs::createNode(ofVec2f nodePos, ofVec2f nodeVel, ofVec2f nodeAccel, float nodeRadius, float nodeMass)
{
	nodePositions.push_back(nodePos);
	nodeVelocities.push_back(nodeVel);
	nodeAccelerations.push_back(nodeAccel);
	nodeRadiuses.push_back(nodeRadius);
	nodeMasses.push_back(nodeMass);
}

void Springs::update()
{
	updateForces();
	dragNodes();
	updateGUI();
	resetForces();
}

void Springs::updateForces()
{
	applyAllForces();

	addForces();
}

void Springs::applyAllForces()
{
	for (int i = 0; i < nodePositions.size(); i++) {
		applyForce(nodeAccelerations[i], updateSprings(i) * timeStep, false);
	}
}

ofVec2f Springs::updateSprings(int _node) {
	// Mass 1 Spring Force
	//ofVec2f mass1SpringForce;
	//mass1SpringForce.x = -k * (nodePositions[0].x - pos.x);
	//mass1SpringForce.y = -k * (nodePositions[0].y - pos.y);

	//// Mass 2 Spring Force
	//ofVec2f mass2SpringForce;
	//mass2SpringForce.x = -k * (nodePositions[1].x - nodePositions[0].x);
	//mass2SpringForce.y = -k * (nodePositions[1].y - nodePositions[0].y);

	//ofVec2f mass3SpringForce;
	//mass3SpringForce.x = -k * (nodePositions[2].x - nodePositions[1].x);
	//mass3SpringForce.y = -k * (nodePositions[2].y - nodePositions[1].y);

	//// Mass 1 Damping
	//ofVec2f mass1DampingForce;
	//mass1DampingForce.x = damping * nodeVelocities[0].x;
	//mass1DampingForce.y = damping * nodeVelocities[0].y;

	//// Mass 2 Damping
	//ofVec2f mass2DampingForce;
	//mass2DampingForce.x = damping * nodeVelocities[1].x;
	//mass2DampingForce.y = damping * nodeVelocities[1].y;

	//ofVec2f mass3DampingForce;
	//mass3DampingForce.x = damping * nodeVelocities[2].x;
	//mass3DampingForce.y = damping * nodeVelocities[2].y;

	//// Mass 1 Net Force
	//ofVec2f mass1Force;
	//mass1Force.x = mass1SpringForce.x - mass1DampingForce.x - mass2SpringForce.x + mass2DampingForce.x;
	//mass1Force.y = mass1SpringForce.y + springmass - mass1DampingForce.y - mass2SpringForce.y + mass2DampingForce.y;

	//// Mass 2 Net Force
	//ofVec2f mass2Force;
	//mass2Force.x = mass2SpringForce.x - mass2DampingForce.x;
	//mass2Force.y = mass2SpringForce.y + springmass - mass2DampingForce.y;

	//ofVec2f mass3Force;
	//mass3Force.x = mass3SpringForce.x - mass3DampingForce.x;
	//mass3Force.y = mass3SpringForce.y + springmass - mass3DampingForce.y;

	//if (_node == 0) {
	//	return mass1Force / springmass;
	//}
	//else if (_node == 1) {
	//	return mass2Force / springmass;
	//}
	//else if (_node == 2) {
	//	return mass3Force / springmass;
	//}

	vector<ofVec2f> massSpringForces;
	for (int i = 0; i < nodePositions.size(); i++) {
		if (i - 1 < 0) {
			ofVec2f force;
			force.x = -k * (nodePositions[i].x - pos.x);
			force.y = -k * (nodePositions[i].y - pos.y);
			massSpringForces.push_back(force);
		}
		else {
			ofVec2f force;
			force.x = -k * (nodePositions[i].x - nodePositions[i - 1].x);
			force.y = -k * (nodePositions[i].y - nodePositions[i - 1].y);
			massSpringForces.push_back(force);
		}
	}

	vector<ofVec2f> massDampingForces;
	for (int i = 0; i < nodePositions.size(); i++) {
		ofVec2f force;
		force.x = damping * nodeVelocities[i].x;
		force.y = damping * nodeVelocities[i].y;
		massDampingForces.push_back(force);
	}

	vector<ofVec2f> massForces;
	for (int i = 0; i < nodePositions.size(); i++) {
		if (i + 1 < nodePositions.size()) {
			ofVec2f force;
			force.x = massSpringForces[i].x - massDampingForces[i].x - massSpringForces[i+1].x + massDampingForces[i+1].x;
			force.y = massSpringForces[i].y + springmass - massDampingForces[i].y - massSpringForces[i+1].y + massDampingForces[i+1].y;
			massForces.push_back(force);
		}
		else {
			ofVec2f force;
			force.x = massSpringForces[i].x - massDampingForces[i].x;
			force.y = massSpringForces[i].y + springmass - massDampingForces[i].y;
			massForces.push_back(force);
		}
	}

	return massForces[_node] / springmass;
};

void Springs::dragNodes()
{
	if (mouseDrag) {
		if (mouseOverIndex != -1) {
			nodePositions[mouseOverIndex].set(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y) + mouseOffsetFromCenter);
			nodeVelocities[mouseOverIndex].set(0);
		}
		else {
			pos.set(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y));
		}
	}
}

void Springs::updateGUI()
{
	if (GameController->getActive() == this) {
		if (!initiai_values_triggered) {
			initiai_values_triggered = true;
			gui_Controller->updateMultipleValues(pos, nodePositions[0], nodeVelocities[0], nodeAccelerations[0], nodeMasses[0], nodeRadiuses[0], nodePositions[1], nodeVelocities[1], nodeAccelerations[1], nodeMasses[1], nodeRadiuses[1], k, damping, springmass, affectedByGravity);
		}
		else {
			gui_Controller->updateMultipleValues(pos, nodePositions[0], nodeVelocities[0], nodeAccelerations[0], gui_Controller->nodeMass1, gui_Controller->nodeRadius1, nodePositions[1], nodeVelocities[1], nodeAccelerations[1], gui_Controller->nodeMass2, gui_Controller->nodeRadius2, gui_Controller->k, gui_Controller->damping, gui_Controller->springmass, gui_Controller->spring_affectedByGravity);
			nodeMasses[0] = gui_Controller->nodeMass1;
			nodeRadiuses[0] = gui_Controller->nodeRadius1;
			nodeMasses[1] = gui_Controller->nodeMass2;
			nodeRadiuses[1] = gui_Controller->nodeRadius2;
			k = gui_Controller->k;
			damping = gui_Controller->damping;
			springmass = gui_Controller->springmass;
			affectedByGravity = gui_Controller->spring_affectedByGravity;
		}
	}
}

void Springs::resetForces()
{
	for (int i = 0; i < nodePositions.size(); i++) {
		nodeAccelerations[i].set(0);
	}
}

void Springs::addForces()
{
	for (int i = 0; i < nodePositions.size(); i++) {
		nodeVelocities[i] += nodeAccelerations[i];
		nodePositions[i] += nodeVelocities[i] * timeStep;
	}
}

void Springs::ellipseCollider()
{
	for (int i = 0; i < GameObjects->size(); i++) {
		if ((*GameObjects)[i]->ellipseCollider_enabled) {
			if ((*GameObjects)[i] != this && (*GameObjects)[i]->isSpring != true) {

				for (int j = 0; j < nodePositions.size(); j++) {
					if (CollisionDetector.EllipseCompare(nodePositions[j], nodeRadiuses[j], (*GameObjects)[i]->pos, (*GameObjects)[i]->radius)) {
						isColliding((*GameObjects)[i], j);
						(*GameObjects)[i]->isColliding(this, nodePositions[j]);
					}
				}
			}
		}
	}
}
void Springs::isColliding(GameObject* _other, int _node)
{
	ofVec2f forceVec = nodePositions[_node] - _other->pos;
	ofVec2f accel = forceVec / nodeMasses[_node];
	applyForce(nodeAccelerations[_node], accel, false);
}

void Springs::mouseHover()
{
	if (GameController->getMouseDragged() == false) {
		if (CollisionDetector.EllipseCompare(nodePositions[0], nodeRadiuses[0], GameController->getWorldMousePos(), 0)) {
			mouseOver = true;
			mouseOverIndex = 0;
			mouseOffsetFromCenter = nodePositions[0] - GameController->getWorldMousePos();
		}
		else if (CollisionDetector.EllipseCompare(nodePositions[1], nodeRadiuses[1], GameController->getWorldMousePos(), 0)) {
			mouseOver = true;
			mouseOverIndex = 1;
			mouseOffsetFromCenter = nodePositions[1] - GameController->getWorldMousePos();
		}
		else if (CollisionDetector.EllipseCompare(pos, radius, GameController->getWorldMousePos(), 0)) {
			mouseOver = true;
			mouseOverIndex = -1;
			mouseOffsetFromCenter = pos - GameController->getWorldMousePos();
		}
		else {			
			mouseOver = false;
			mouseOffsetFromCenter.set(0);
			
			color = ofColor(255);
		}
	}
}


// ----- EVENT FUNCTIONS ----- //


void Springs::mousePressed(float _x, float _y, int _button)
{
	if (_button == 2 && mouseOver) {
		if (GameController->getActive() != this) {
			GameController->makeActive(this);
		}
	}
	if (_button == 2) {
		if (GameController->getMouseDragged() == false) {
			if (mouseOver) {
				mouseDrag = true;
				GameController->setMouseDragged(true);
			}
		}
	}
}

void Springs::mouseReleased(float _x, float _y, int _button)
{
	if (_button == 2) {
		mouseDrag = false;
		GameController->setMouseDragged(false);
	}

}


// ----- RENDER LOOP ----- //


void Springs::draw()
{
	ofPushStyle();

	if (GameController->getActive() == this) {
		color = ofColor(255, 165, 0);
	}
	else {
		color = ofColor(255);
	}

	ofNoFill();
	ofSetColor(color);

	for (int i = 0; i < nodePositions.size(); i++) {
		if (i == 0) ofLine(nodePositions[i].x, nodePositions[i].y, pos.x, pos.y);
		else ofLine(nodePositions[i].x, nodePositions[i].y, nodePositions[i-1].x, nodePositions[i-1].y);
	}
	
	ofFill();
	getNodeColor(-1);
	ofEllipse(pos.x, pos.y, radius, radius);

	ofSetColor(0);
	for (int i = 0; i < nodePositions.size(); i++) {
		ofEllipse(nodePositions[i].x, nodePositions[i].y, nodeRadiuses[i], nodeRadiuses[i]);
	}

	for (int i = 0; i < nodePositions.size(); i++) {
		ofSetLineWidth(ofMap(nodeMasses[i], MINIMUM_MASS, MAXIMUM_MASS / 2, 0.1, 10));
		ofNoFill();
		getNodeColor(i);
		ofEllipse(nodePositions[i].x, nodePositions[i].y, nodeRadiuses[i], nodeRadiuses[i]);
	}
	
	ofPopStyle();
}

void Springs::getNodeColor(int _node)
{
	if ((GameController->getActive() == this) || (mouseOver || mouseDrag) && mouseOverIndex == _node) {
		ofSetColor(255, 165, 0);
	}
	else {
		ofSetColor(color);
	}
}