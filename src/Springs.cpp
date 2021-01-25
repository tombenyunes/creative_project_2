#include "Springs.h"

Springs::Springs(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass, Controller* _controller)
{
	GameController = _controller;
	
	color = ofColor(255);

	mouse_down_triggered = false;
	initiai_values_triggered = false;
	mouseOverNode1 = false;
	mouseOverNode2 = false;
	mouseOverAnchor = false;
	mouseDragNode1 = false;
	mouseDragNode2 = false;
	mouseDragAnchor = false;

	isSpring = true;
	affectedByGravity = true;

	k = _k;
	damping = _damping;
	springmass = _springmass;

	pos = _anchorPos;
	radius = 8;
	timeStep = 0.28;
	
	nodePos1.set(pos);
	nodeVel1.set(0);
	nodeAccel1.set(0);
	nodeRadius1 = _nodeRadius1;
	nodeMass1 = _nodeMass1;

	nodePos2.set(pos);
	nodeVel2.set(0);
	nodeAccel2.set(0);
	nodeRadius2 = _nodeRadius2;
	nodeMass2 = _nodeMass2;

	// seeing as each spring system contains multiple positions, velocities, etc, the below modules must be overriden to account for this
	AddModule("screenBounce");
	AddModule("ellipseCollider");
	AddModule("gravity");
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
	applyAllForces(1);
	applyAllForces(2);

	addForces();
}

void Springs::applyAllForces(int _node)
{
	// Adds friction to acceleration
	if (_node == 1) {
		applyForce(nodeAccel1, updateSprings(1) * timeStep, false);
		applyForce(nodeAccel1, applyFriction(1), false);
	}
	else if (_node == 2) {
		applyForce(nodeAccel2, updateSprings(2) * timeStep, false);
		applyForce(nodeAccel2, applyFriction(2), false);
	}
}

ofVec2f Springs::updateSprings(int _node) {
	// Mass 1 Spring Force
	ofVec2f mass1SpringForce;
	mass1SpringForce.x = -k * (nodePos1.x - pos.x);
	mass1SpringForce.y = -k * (nodePos1.y - pos.y);

	// Mass 2 Spring Force
	ofVec2f mass2SpringForce;
	mass2SpringForce.x = -k * (nodePos2.x -
		nodePos1.x);
	mass2SpringForce.y = -k * (nodePos2.y -
		nodePos1.y);

	// Mass 1 Damping
	ofVec2f mass1DampingForce;
	mass1DampingForce.x = damping * nodeVel1.x;
	mass1DampingForce.y = damping * nodeVel1.y;

	// Mass 2 Damping
	ofVec2f mass2DampingForce;
	mass2DampingForce.x = damping * nodeVel2.x;
	mass2DampingForce.y = damping * nodeVel2.y;

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
	if (mouseDragNode1) {
		nodePos1.set(ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2) + mouseOffsetFromCenter);
		nodeVel1.set(0);
	}
	else if (mouseDragNode2) {
		nodePos2.set(ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2) + mouseOffsetFromCenter);
		nodeVel2.set(0);
	}
	else if (mouseDragAnchor) {
		pos.set(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
	}
}

void Springs::updateGUI()
{
	if (GameController->getActive() == this) {
		if (!initiai_values_triggered) {
			initiai_values_triggered = true;
			gui_Controller->updateMultipleValues(pos, nodePos1, nodeVel1, nodeAccel1, nodeMass1, nodeRadius1, nodePos2, nodeVel2, nodeAccel2, nodeMass2, nodeRadius2, k, damping, springmass, affectedByGravity);
		}
		else {
			gui_Controller->updateMultipleValues(pos, nodePos1, nodeVel1, nodeAccel1, gui_Controller->nodeMass1, gui_Controller->nodeRadius1, nodePos2, nodeVel2, nodeAccel2, gui_Controller->nodeMass2, gui_Controller->nodeRadius2, gui_Controller->k, gui_Controller->damping, gui_Controller->springmass, gui_Controller->spring_affectedByGravity);
			nodeMass1 = gui_Controller->nodeMass1;
			nodeRadius1 = gui_Controller->nodeRadius1;
			nodeMass2 = gui_Controller->nodeMass2;
			nodeRadius2 = gui_Controller->nodeRadius2;
			k = gui_Controller->k;
			damping = gui_Controller->damping;
			springmass = gui_Controller->springmass;
			affectedByGravity = gui_Controller->spring_affectedByGravity;
		}
	}
}

void Springs::resetForces()
{
	nodeAccel1.set(0);
	nodeAccel2.set(0);
}


// seeing as the spring contains 3 separate nodes, and thus 3 separate positions, velocities, mass, etc, the class must override a lot of the functions that the player and standard node uses
// overriden modules below


void Springs::addForces()
{
	nodeVel1 += nodeAccel1;
	nodeVel2 += nodeAccel2;
	nodePos1 += nodeVel1 * timeStep;
	nodePos2 += nodeVel2 * timeStep;
}

void Springs::gravity()
{
	if (GameController->getGravity() == 1 || affectedByGravity) {
		ofVec2f gravity1 = { 0, (float)GRAVITY_FORCE * 400 * nodeMass1 };
		ofVec2f gravity2 = { 0, (float)GRAVITY_FORCE * 400 * nodeMass2 };
		applyForce(nodeAccel1, gravity1, false);
		applyForce(nodeAccel2, gravity2, false);
	}
}

ofVec2f Springs::applyFriction(int _node)
{
	ofVec2f friction;
	if (_node == 1) {
		friction = nodeVel1 * -1;
		friction *= FRICTION_FORCE;
		return friction;
	}
	else if (_node == 2) {
		friction = nodeVel2 * -1;
		friction *= FRICTION_FORCE;
		return friction;
	}
}

void Springs::ellipseCollider()
{
	for (int i = 0; i < GameObjects->size(); i++) {
		if ((*GameObjects)[i]->ellipseCollider_enabled) {
			if ((*GameObjects)[i] != this && (*GameObjects)[i]->isSpring != true) {
				if (CollisionDetector.EllipseCompare(nodePos1, nodeRadius1, (*GameObjects)[i]->pos, (*GameObjects)[i]->radius)) {
					isColliding((*GameObjects)[i], 1);
					(*GameObjects)[i]->isColliding(this, nodePos1);
				}
				if (CollisionDetector.EllipseCompare(nodePos2, nodeRadius2, (*GameObjects)[i]->pos, (*GameObjects)[i]->radius)) {
					isColliding((*GameObjects)[i], 2);
					(*GameObjects)[i]->isColliding(this, nodePos2);
				}
			}
		}
	}
}
void Springs::isColliding(GameObject* _other, int _node)
{
	if (_node == 1) {
		ofVec2f forceVec = nodePos1 - _other->pos;
		ofVec2f accel = forceVec / nodeMass1;
		applyForce(nodeAccel1, accel, false);
	}
	else if (_node == 2) {
		ofVec2f forceVec = nodePos2 - _other->pos;
		ofVec2f accel = forceVec / nodeMass2;
		applyForce(nodeAccel2, accel, false);
	}
}

void Springs::screenBounce()
{
	if (nodePos1.x > 0 + (ofGetWidth() / 2) - (nodeRadius1) / 2) {
		nodeVel1.x *= -1;
		nodePos1.x = 0 + (ofGetWidth() / 2) - (nodeRadius1) / 2;
	}
	if (nodePos1.x < 0 - (ofGetWidth() / 2) + (nodeRadius1) / 2) {
		nodeVel1.x *= -1;
		nodePos1.x = 0 - (ofGetWidth() / 2) + (nodeRadius1) / 2;
	}
	if (nodePos1.y < 0 - (ofGetHeight() / 2) + (nodeRadius1) / 2) {
		nodeVel1.y *= -1;
		nodePos1.y = 0 - (ofGetHeight() / 2) + (nodeRadius1) / 2;
	}
	if (nodePos1.y > 0 + (ofGetHeight() / 2) - (nodeRadius1) / 2) {
		nodeVel1.y *= -1;
		nodePos1.y = 0 + (ofGetHeight() / 2) - (nodeRadius1) / 2;
	}

	if (nodePos2.x > 0 + (ofGetWidth() / 2) - (nodeRadius2) / 2) {
		nodeVel2.x *= -1;
		nodePos2.x = 0 + (ofGetWidth() / 2) - (nodeRadius2) / 2;
	}
	if (nodePos2.x < 0 - (ofGetWidth() / 2) + (nodeRadius2) / 2) {
		nodeVel2.x *= -1;
		nodePos2.x = 0 - (ofGetWidth() / 2) + (nodeRadius2) / 2;
	}
	if (nodePos2.y < 0 - (ofGetHeight() / 2) + (nodeRadius2) / 2) {
		nodeVel2.y *= -1;
		nodePos2.y = 0 - (ofGetHeight() / 2) + (nodeRadius2) / 2;
	}
	if (nodePos2.y > 0 + (ofGetHeight() / 2) - (nodeRadius2) / 2) {
		nodeVel2.y *= -1;
		nodePos2.y = 0 + (ofGetHeight() / 2) - (nodeRadius2) / 2;
	}
}

void Springs::mouseHover()
{
	if (mouseOverNode1 || mouseOverNode2 || mouseOverAnchor) {
		mouseOver = true;
	}
	else {
		mouseOver = false;
	}
	if (GameController->getMouseDragged() == false) {
		if (CollisionDetector.EllipseCompare(nodePos1, nodeRadius1, ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2), 0)) {
			mouseOverNode1 = true;
			mouseOffsetFromCenter = nodePos1 - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
		}
		else if (CollisionDetector.EllipseCompare(nodePos2, nodeRadius2, ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2), 0)) {
			mouseOverNode2 = true;
			mouseOffsetFromCenter = nodePos2 - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
		}
		else if (CollisionDetector.EllipseCompare(pos, radius, ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2), 0)) {
			mouseOverAnchor = true;
			mouseOffsetFromCenter = pos - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
		}
		else {
			color = ofColor(255);
			mouseOverNode1 = false;
			mouseOverNode2 = false;
			mouseOverAnchor = false;

			mouseOffsetFromCenter.set(0);
		}
	}
}


// ----- EVENT FUNCTIONS ----- //


void Springs::mousePressed(int _x, int _y, int _button)
{
	if (!mouse_down_triggered) {
		mouse_down_triggered = true;
		if (_button == 2 && (mouseOverNode1 || mouseOverNode2 || mouseOverAnchor)) {
			if (GameController->getActive() != this) {
				initiai_values_triggered = false;
				GameController->makeActive(this);
			}
		}
	}
	if (_button == 2) {
		if (GameController->getMouseDragged() == false) {
			if (mouseOverNode1) {
				mouseDragNode1 = true;
				GameController->setMouseDragged(true);
			}
			else if (mouseOverNode2) {
				mouseDragNode2 = true;
				GameController->setMouseDragged(true);
			}
			else if (mouseOverAnchor) {
				mouseDragAnchor = true;
				GameController->setMouseDragged(true);
			}
		}
	}
}

void Springs::mouseReleased(int _x, int _y, int _button)
{
	if (_button == 2) {
		if (mouse_down_triggered) {
			mouse_down_triggered = false;
		}
		if (mouseDragNode1 || mouseDragNode2 || mouseDragAnchor) {
			mouseDragNode1 = false;
			mouseDragNode2 = false;
			mouseDragAnchor = false;
			GameController->setMouseDragged(false);
		}
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

	ofLine(nodePos1.x, nodePos1.y, pos.x, pos.y);
	ofLine(nodePos2.x, nodePos2.y, nodePos1.x, nodePos1.y);
	
	ofFill();
	getNodeColor(-1);
	ofEllipse(pos.x, pos.y, radius, radius);

	ofSetColor(0);
	ofEllipse(nodePos1.x, nodePos1.y, nodeRadius1, nodeRadius1);
	ofEllipse(nodePos2.x, nodePos2.y, nodeRadius2, nodeRadius2);

	ofSetLineWidth(ofMap(nodeMass1, MINIMUM_MASS, MAXIMUM_MASS/2, 0.1, 10));

	ofNoFill();
	getNodeColor(1);
	ofEllipse(nodePos1.x, nodePos1.y, nodeRadius1, nodeRadius1);	
	
	ofSetLineWidth(ofMap(nodeMass2, MINIMUM_MASS, MAXIMUM_MASS/2, 0.1, 10));

	getNodeColor(2);
	ofEllipse(nodePos2.x, nodePos2.y, nodeRadius2, nodeRadius2);
	
	ofPopStyle();
}

void Springs::getNodeColor(int _node)
{
	if (_node == 1) {
		if (mouseOverNode1 || mouseDragNode1 == true) {
			ofSetColor(255, 165, 0);
		}
		else {
			ofSetColor(color);
		}
	}
	else if (_node == 2) {
		if (mouseOverNode2 || mouseDragNode2 == true) {
			ofSetColor(255, 165, 0);
		}
		else {
			ofSetColor(color);
		}
	}
	else if (_node == -1) {
		if (mouseOverAnchor || mouseDragAnchor == true) {
			ofSetColor(255, 165, 0);
		}
		else {
			ofSetColor(color);
		}
	}
}