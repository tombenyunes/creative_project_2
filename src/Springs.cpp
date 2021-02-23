#include "Springs.h"

Springs::Springs(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass, Controller* _controller)
{
	GameController = _controller;
	
	type = "Spring";

	color = ofColor(255);

	mouse_down_triggered = false;
	initiai_values_triggered = false;
	mouseDrag = false;

	isSpring = true;
	affectedByGravity = true;

	k = _k;
	damping = _damping;
	springmass = _springmass;

	pos = _anchorPos;
	radius = 8;
	timeStep = 0.28;

	hasMultipleNodes = true;

	nodePositions.push_back(pos);
	nodePositions.push_back(pos);

	nodeVelocities.push_back(ofVec2f(0, 0));
	nodeVelocities.push_back(ofVec2f(0, 0));

	nodeAccelerations.push_back(ofVec2f(0, 0));
	nodeAccelerations.push_back(ofVec2f(0, 0));

	nodeRadiuses.push_back(_nodeRadius1);
	nodeRadiuses.push_back(_nodeRadius2);

	nodeMasses.push_back(_nodeMass1);
	nodeMasses.push_back(_nodeMass2);

	// seeing as each spring system contains multiple positions, velocities, etc, the below modules must be overriden to account for this
	AddModule("screenBounce");
	AddModule("ellipseCollider");
	AddModule("gravity");
	AddModule("friction");
	AddModule("mouseHover");
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
	addForces(false);
}

void Springs::applyAllForces()
{
	for (int i = 0; i < nodePositions.size(); i++) {
		applyForce(nodeAccelerations[i], updateSprings(i+1) * timeStep, false);
	}
}

ofVec2f Springs::updateSprings(int _node) {
	// Mass 1 Spring Force
	ofVec2f mass1SpringForce;
	mass1SpringForce.x = -k * (nodePositions[0].x - pos.x);
	mass1SpringForce.y = -k * (nodePositions[0].y - pos.y);

	// Mass 2 Spring Force
	ofVec2f mass2SpringForce;
	mass2SpringForce.x = -k * (nodePositions[1].x -
		nodePositions[0].x);
	mass2SpringForce.y = -k * (nodePositions[1].y -
		nodePositions[0].y);

	// Mass 1 Damping
	ofVec2f mass1DampingForce;
	mass1DampingForce.x = damping * nodeVelocities[0].x;
	mass1DampingForce.y = damping * nodeVelocities[0].y;

	// Mass 2 Damping
	ofVec2f mass2DampingForce;
	mass2DampingForce.x = damping * nodeVelocities[1].x;
	mass2DampingForce.y = damping * nodeVelocities[1].y;

	// Mass 1 Net Force
	ofVec2f mass1Force;
	mass1Force.x = mass1SpringForce.x -
		mass1DampingForce.x - mass2SpringForce.x + mass2DampingForce.x;
	mass1Force.y = mass1SpringForce.y +
		springmass - mass1DampingForce.y - mass2SpringForce.y +
		mass2DampingForce.y;

	// Mass 2 Net Force
	ofVec2f mass2Force;
	mass2Force.x = mass2SpringForce.x - mass2DampingForce.x;
	mass2Force.y = mass2SpringForce.y + springmass - 
		mass2DampingForce.y;

	if (_node == 1) {
		return mass1Force / springmass;
	}
	else if (_node == 2) {
		return mass2Force / springmass;
	}
};

void Springs::dragNodes()
{
	if (mouseDrag) {
		if (mouseOverIndex != -1) {
			nodePositions[mouseOverIndex].set(ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y) + mouseOffsetFromCenter);
			nodeVelocities[mouseOverIndex].set(0);
		}
		else {
			pos.set(ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y));
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

//void Springs::mouseHover()
//{
//	if (mouseOverNode1 || mouseOverNode2 || mouseOverAnchor) {
//		mouseOver = true;
//	}
//	else {
//		mouseOver = false;
//	}
//	if (GameController->getMouseDragged() == false) {
//		if (CollisionDetector.EllipseCompare(nodePositions[0], nodeRadiuses[0], ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y), 0)) {
//			mouseOverNode1 = true;
//			mouseOffsetFromCenter = nodePositions[0] - ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y);
//		}
//		else if (CollisionDetector.EllipseCompare(nodePositions[1], nodeRadiuses[1], ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y), 0)) {
//			mouseOverNode2 = true;
//			mouseOffsetFromCenter = nodePositions[1] - ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y);
//		}
//		else if (CollisionDetector.EllipseCompare(pos, radius, ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y), 0)) {
//			mouseOverAnchor = true;
//			mouseOffsetFromCenter = pos - ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y);
//		}
//		else {
//			color = ofColor(255);
//			mouseOverNode1 = false;
//			mouseOverNode2 = false;
//			mouseOverAnchor = false;
//
//			mouseOffsetFromCenter.set(0);
//		}
//	}
//}


// ----- EVENT FUNCTIONS ----- //


void Springs::mousePressed(float _x, float _y, int _button)
{
	if (!mouse_down_triggered) {
		mouse_down_triggered = true;
		if (_button == 2 && (mouseOver)) {
			if (GameController->getActive() != this) {
				initiai_values_triggered = false;
				GameController->makeActive(this);
			}
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
		if (mouse_down_triggered) {
			mouse_down_triggered = false;
		}
		if (mouseDrag) {
			mouseDrag = false;
			GameController->setMouseDragged(false);
		}
	}

}


// ----- RENDER LOOP ----- //


void Springs::draw()
{
	ofPushStyle();

	if (GameController->getActive() == this) {
		ofSetColor(255, 165, 0);
	}
	else {
		ofSetColor(color);
	}

	// lines connecting nodes
	ofLine(nodePositions[0].x, nodePositions[0].y, pos.x, pos.y);
	ofLine(nodePositions[1].x, nodePositions[1].y, nodePositions[0].x, nodePositions[0].y);
	
	// black mask
	ofSetColor(0);
	ofFill();
	for (int i = 0; i < nodePositions.size(); i++) {
		ofEllipse(nodePositions[i].x, nodePositions[i].y, nodeRadiuses[i], nodeRadiuses[i]);
	}
	
	if (GameController->getActive() == this) {
		ofSetColor(255, 165, 0);
	}
	else {
		ofSetColor(color);
	}
	
	// anchor
	ofFill();
	ofEllipse(pos.x, pos.y, radius, radius);

	// nodes
	ofNoFill();
	for (int i = 0; i < nodePositions.size(); i++) {
		ofSetLineWidth(ofMap(nodeMasses[i], MINIMUM_MASS, MAXIMUM_MASS / 2, 0.1, 10));
		ofEllipse(nodePositions[i].x, nodePositions[i].y, nodeRadiuses[i], nodeRadiuses[i]);
	}
	
	ofPopStyle();
}