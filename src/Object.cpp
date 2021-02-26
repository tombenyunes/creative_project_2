#include "Object.h"

Object::Object(ofVec2f _pos, float _mass, float _radius)
{
	type = "Object";

	pos.set(_pos);
	color = ofColor(passiveColor);
	mass = _mass;
	radius = _radius;

	gui_values_need_to_be_set = true;
	mouseDrag = false;

	AddModule("screenBounce");
	AddModule("ellipseCollider");
	AddModule("gravity");
	AddModule("friction");
	AddModule("mouseHover");
}

void Object::update()
{
	posBeforeDrag.set(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y);
	updateForces();
	dragNodes();
	updateGUI();
	resetForces();
}

void Object::updateForces()
{
	addForces(false);
}

void Object::dragNodes()
{
	static ofVec2f mousePosBeforeDrag;
	if (mouseDrag) {
		ofVec2f prevPos2 = ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y) + mouseOffsetFromCenter;

		ofVec2f newPos;
		newPos.x = ofLerp(pos.x, prevPos2.x, 0.1);
		newPos.y = ofLerp(pos.y, prevPos2.y, 0.1);

		pos.set(newPos);

		vel.set(0);

		startedDragging = true;
		mousePosBeforeDrag = ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y);
	}
	else {
		if (startedDragging == true) {
			startedDragging = false;
			ofVec2f mousespeed = (ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y) - mousePosBeforeDrag) / 3;
			applyForce(accel, mousespeed, false);
		}
	}
}

void Object::updateGUI()
{
	if (GameController->getActive() == this) {
		if (gui_values_need_to_be_set) {
			gui_Controller->updateValues(pos, vel, accel, mass, infiniteMass, radius, affectedByGravity, 2);
			gui_values_need_to_be_set = false;
		}
		else {
			gui_Controller->updateValues(pos, vel, accel, gui_Controller->selected_mass, gui_Controller->selected_infiniteMass, gui_Controller->selected_radius, gui_Controller->selected_affectedByGravity, 2);
			if (infiniteMass) {
				mass = 999999999999;
			}
			else {
				mass = gui_Controller->selected_mass;
			}
			radius = gui_Controller->selected_radius;
			infiniteMass = gui_Controller->selected_infiniteMass;
			affectedByGravity = gui_Controller->selected_affectedByGravity;
		}
	}
}

void Object::resetForces()
{
	accel.set(0);
}


// ----- EVENT FUNCTIONS ----- //


void Object::mousePressed(float _x, float _y, int _button)
{
	if (_button == 2 && mouseOver) {
		if (GameController->getActive() != this) {
			GameController->makeActive(this);
			gui_values_need_to_be_set = true;
		}
	}
}

void Object::mouseDragged(float _x, float _y, int _button)
{
	if (_button == 2) {
		if (mouseOver && GameController->getMouseDragged() == false) {			
			if (posBeforeDrag.distance(ofVec2f(ofGetMouseX() / 2 - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2)) > 2) {
				// the node will only be moved by the mouse if it has been moved by more than 1 pixel - this prevents accidentally stopping something by selecting it
				mouseDrag = true;
				GameController->setMouseDragged(true);				
			}
		}
	}
}

void Object::mouseReleased(float _x, float _y, int _button)
{
	if (_button == 2) {
		mouseDrag = false;
		GameController->setMouseDragged(false);
	}
}


// ----- RENDER LOOP ----- //


void Object::draw()
{
	ofPushStyle();

	getColor();

	ofNoFill();
	ofSetLineWidth(ofMap(mass, MINIMUM_MASS, MAXIMUM_MASS, 0.1, 10));

	ofEllipse(pos.x, pos.y, radius, radius);

	ofPopStyle();
}

void Object::getColor()
{	
	if ((GameController->getActive() == this) || (mouseOver || mouseDrag)) {
		ofSetColor(selectedColor);
	}
	else if (infiniteMass) {
		ofSetColor(passiveColor);
	}
	else {
		ofSetColor(color);
	}
}