#include "Player.h"

Player::Player(ofVec2f _pos, ofColor _color)
{
	type = "Player";

	pos.set(_pos);
	color.set(_color);

	vel.set(0);
	accel.set(0);
	radius = 14; // 35
	mass = 500;

	isPlayer = true;
	mouse_down = false;
	mouse_button = -1;	
	aimingBoost = false;

	// modules are updated automatically
	AddModule("screenBounce");
	AddModule("ellipseCollider");
	AddModule("gravity");
	AddModule("friction");
}

void Player::update()
{
	drawParticleTrail();
	updateForces();
	updateGUI();
	resetForces();
}

void Player::updateForces()
{
	applyAllForces();
	addForces(true);
}

void Player::applyAllForces()
{
	if (playerCanMove()) applyForce(accel, getMovementVector(), true, 0.15);
}

bool Player::playerCanMove()
{
	if (mouse_down && mouse_button == 0) {
		return true;
	}
	else {
		return false;
	}
}

ofVec2f Player::getMovementVector()
{
	ofVec2f movementVec = pos - mouse_pos;
	movementVec.scale(5);
	return movementVec;
}

void Player::updateGUI()
{
	static bool initiai_values_triggered = false; // initial values are sent to the gui_manager initially, after which it will update the results internally, and the object can receive the values back
	if (!initiai_values_triggered) {
		initiai_values_triggered = true;
		GUI_Manager->updateValues(pos, vel, accel, mass, infiniteMass, radius, affectedByGravity, 1);
	}
	else {
		GUI_Manager->updateValues(pos, vel, accel, GUI_Manager->mass, GUI_Manager->infiniteMass, GUI_Manager->radius, GUI_Manager->affectedByGravity, 1); // receiving and updating the results from the GUI_Manager
		if (infiniteMass) mass = 9999999999999999999; else mass = GUI_Manager->mass;
		radius = GUI_Manager->radius;
		infiniteMass = GUI_Manager->infiniteMass;
		affectedByGravity = GUI_Manager->affectedByGravity;
	}
}

void Player::resetForces()
{
	accel.set(0);
}


// ----- EVENT FUNCTIONS ----- //


void Player::mousePressed(float _x, float _y, int _button)
{
	mouse_down = true;
	mouse_button = _button;
	mouse_pos = { (float)_x, (float)_y };
}

void Player::mouseDragged(float _x, float _y, int _button)
{
	mouse_down = true;
	mouse_button = _button;
	mouse_pos = { (float)_x, (float)_y };
}

void Player::mouseReleased(float _x, float _y, int _button)
{
	mouse_down = false;
}

void Player::keyPressed(int key)
{
	if (key == 114) // r
	{
		accel = { 0, 0 };
		vel = { 0, 0 };
		pos = { 0, 0 };
	}
	if (key == 32)
	{
		//aimingBoost = true;
	}
}

void Player::keyReleased(int key)
{
	if (key == 32)
	{
		//boostPlayer();
	}
}

void Player::boostPlayer()
{
	aimingBoost = false;
	if (vel.length() < 5) {
		applyForce(accel, (pos - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2)), true, 10);
	}
}


// ----- RENDER LOOP ----- //


void Player::draw()
{
	ofSetColor(255);

	if (aimingBoost) drawBoostDirection();
	//drawParticleTrail();

	ofSetColor(color);
	ofFill();
	ofEllipse(pos.x, pos.y, radius, radius);
}

void Player::drawBoostDirection() // draws dotted line in the direction the player is aiming
{
	ofPushMatrix();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(8, 0xAAAA);
	glBegin(GL_LINES);
	glVertex3f(pos.x, pos.y, 0);
	glVertex3f(drawVelPath().x, drawVelPath().y, drawVelPath().z);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glFlush();

	ofPopMatrix();
}

ofVec3f Player::drawVelPath()
{
	ofVec2f vec = pos - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
	return ofVec3f(vec.x * ofGetWidth(), vec.y * ofGetHeight(), 0);
}

void Player::drawParticleTrail() // draws particle trail following the player when moving
{
	if (mouse_down && mouse_button == 0) {
		// draw new particles
		//float posX = ofMap(pos.x, -ofGetWidth()/2, ofGetWidth()/2, 0, 1);
		//float posY = ofMap(pos.y, -ofGetHeight() / 2, ofGetHeight() / 2, 0, 1);
		//ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
		//ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
		//addToFluid(ofVec2f(posX, posY), vel / 100 * -1, true, true, 1);
		
		ofVec2f newPos;
		newPos.x = ofMap(pos.x/* + ofRandom(-radius / 4, radius / 4)*/, -WORLD_WIDTH / 2, WORLD_WIDTH / 2, 0, 1);
		newPos.y = ofMap(pos.y/* + ofRandom(-radius / 4, radius / 4)*/, -WORLD_HEIGHT / 2, WORLD_HEIGHT / 2, 0, 1);
		
		ofVec2f newVel;
		//newVel.x = ((vel.x + ofRandom(-1, 1)) / 600) * -1;
		//newVel.y = ((vel.y + ofRandom(-1, 1)) / 600) * -1;
		newVel.x = ((getMovementVector().x + ofRandom(-1, 1)) / 6400) * -1;
		newVel.y = ((getMovementVector().y + ofRandom(-1, 1)) / 6400) * -1;

		Fluid_Manager->addToFluid(newPos, newVel, true, true);
	}
}