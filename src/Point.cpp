#include "Point.h"

Point::Point(ofVec2f _pos, float _mass, float _radius)
{
	type = "Point";

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

void Point::update()
{	
	updateForces();
	dragNodes();
	updateGUI();
	resetForces();
}

void Point::updateForces()
{
	addForces(false);
}

void Point::dragNodes()
{
	posBeforeDrag.set(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y);
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

void Point::updateGUI()
{
	if (GameController->getActive() == this) {
		if (gui_values_need_to_be_set) {
			GUI_Manager->updateValues(pos, vel, accel, mass, infiniteMass, radius, affectedByGravity, 2);
			gui_values_need_to_be_set = false;
		}
		else {
			GUI_Manager->updateValues(pos, vel, accel, GUI_Manager->selected_mass, GUI_Manager->selected_infiniteMass, GUI_Manager->selected_radius, GUI_Manager->selected_affectedByGravity, 2);
			if (infiniteMass) {
				mass = 999999999999;
			}
			else {
				mass = GUI_Manager->selected_mass;
			}
			radius = GUI_Manager->selected_radius;
			infiniteMass = GUI_Manager->selected_infiniteMass;
			affectedByGravity = GUI_Manager->selected_affectedByGravity;
		}
	}
}

void Point::resetForces()
{
	accel.set(0);
}

void Point::isColliding(GameObject* _other, ofVec2f _nodePos)
{
	needs_to_be_deleted = true;
}


// ----- EVENT FUNCTIONS ----- //


void Point::mousePressed(float _x, float _y, int _button)
{
	if (_button == 2 && mouseOver) {
		if (GameController->getActive() != this) {
			GameController->makeActive(this);
			gui_values_need_to_be_set = true;
		}
	}
}

void Point::mouseDragged(float _x, float _y, int _button)
{
	if (_button == 2) {
		if (mouseOver && GameController->getMouseDragged() == false) {
			if (posBeforeDrag.distance(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y)) > 2) {
				// the node will only be moved by the mouse if it has been moved by more than 1 pixel - this prevents accidentally stopping something by selecting it
				mouseDrag = true;
				GameController->setMouseDragged(true);
			}
		}
	}
}

void Point::mouseReleased(float _x, float _y, int _button)
{
	if (_button == 2) {
		mouseDrag = false;
		GameController->setMouseDragged(false);
	}
}


// ----- RENDER LOOP ----- //


void Point::draw()
{
	ofPushStyle();

	getColor();

	ofNoFill();
	ofSetLineWidth(ofMap(mass, MINIMUM_MASS, MAXIMUM_MASS, 0.1, 10));

	ofEllipse(pos.x, pos.y, radius, radius);

	ofPopStyle();
}

void Point::getColor()
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