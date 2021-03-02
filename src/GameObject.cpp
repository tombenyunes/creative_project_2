#include "GameObject.h"

GameObject::GameObject(ofVec2f _pos, ofColor _color)
{
	pos.set(_pos);
	prevPos.set(99999, 99999);
	color.set(_color);

	vel.set(0);
	accel.set(0);
	radius = 35;
	mass = 10;
	infiniteMass = false;
	affectedByGravity = false;

	passiveColor = ofColor(255);
	selectedColor = ofColor(255, 165, 0);

	/*nodePos1.set(0);
	nodeVel1.set(0);
	nodeAccel1.set(0);
	nodeRadius1 = 0;
	nodeMass1 = 0;
	nodePos2.set(0);
	nodeVel2.set(0);
	nodeAccel2.set(0);
	nodeRadius2 = 0;
	nodeMass2 = 0;*/


	needs_to_be_deleted = false;
	mouseOver = false;
	mouseOffsetFromCenter.set(0);

	isPlayer = false;
	isSpring = false;

	deleteKeyDown = false;

	screenWrap_enabled = false;
	screenBounce_enabled = false;
	gravity_enabled = false;
	friction_enabled = false;
	ellipseCollider_enabled = false;
	mouseHover_enabled = false;
}

void GameObject::init(vector<GameObject*>* _gameobjects, Controller* _controller, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager)
{
	GameObjects = _gameobjects;
	GameController = _controller;
	GUI_Manager = _GUIManager;
	cam = _cam;
	Fluid_Manager = _fluidManager;
}

// root update is called prir to the main update function of a gameobject and is responsible for handling object deletion and updating user-added modules - it automatically updates the main update funcion
void GameObject::root_update()
{

	if (deleteKeyDown) {
		if (mouseOver) {
			needs_to_be_deleted = true;
		}		
	}
	if (!needs_to_be_deleted) {


		if (screenWrap_enabled) {
			screenWrap();
		}
		if (screenBounce_enabled) {
			screenBounce();
		}
		if (gravity_enabled) {
			gravity();
		}
		if (friction_enabled) {
			friction();
		}
		if (ellipseCollider_enabled) {
			ellipseCollider();
		}
		if (mouseHover_enabled) {
			mouseHover();
		}

		prevPos = pos;
		
		update(); // <--- user defined update function for every gameobject
	}
	else {		
		//cout << "Error: 'Dead' GameObject is still being updated" << endl;
	}
}

void GameObject::update()
{
	static bool initialized = false;
	if (!initialized)
	{
		//cout << "Error: User hasn't defined unique 'update' function for a GameObject" << endl;
		initialized = true;
	}
}

// ----- MODULES ----- //

// I made the module system to be an easy way to share functions to only the objects that need them

// Simply including `AddModule("module-name")` to the constructor of a new gameobject will cause this module to be updated

void GameObject::AddModule(string _id)
{
	if (_id == "screenWrap") {
		screenWrap_enabled = true;
	}
	else if (_id == "screenBounce") {
		screenBounce_enabled = true;
	}
	else if (_id == "gravity") {
		gravity_enabled = true;
	}
	else if (_id == "friction") {
		friction_enabled = true;
	}
	else if (_id == "ellipseCollider") {
		ellipseCollider_enabled = true;
	}
	else if (_id == "mouseHover") {
		mouseHover_enabled = true;
	}
	else {
		cout << "Error: Module ID is invalid" << endl;
	}
}

// upon reaching the screen edge, the object is placed at the opposite edge
void GameObject::screenWrap()
{
	if (pos.x > 0 + (WORLD_WIDTH / 2)) {
		pos.x = 0 - (WORLD_WIDTH / 2);
	}
	if (pos.x < 0 - (WORLD_WIDTH / 2)) {
		pos.x = 0 + (WORLD_WIDTH / 2);
	}
	if (pos.y < 0 - (WORLD_HEIGHT / 2)) {
		pos.y = 0 + (WORLD_HEIGHT / 2);
	}
	if (pos.y > 0 + (WORLD_HEIGHT / 2)) {
		pos.y = 0 - (WORLD_HEIGHT / 2);
	}
}

// object 'bounce' when hitting the screen edge
void GameObject::screenBounce()
{
	if (nodePositions.size() == 0) {
		if (pos.x > 0 + (WORLD_WIDTH / 2) - (radius) / 2) {
			vel.x *= -1;
			pos.x = 0 + (WORLD_WIDTH / 2) - (radius) / 2;
		}
		if (pos.x < 0 - (WORLD_WIDTH / 2) + (radius) / 2) {
			vel.x *= -1;
			pos.x = 0 - (WORLD_WIDTH / 2) + (radius) / 2;
		}
		if (pos.y < 0 - (WORLD_HEIGHT / 2) + (radius) / 2) {
			vel.y *= -1;
			pos.y = 0 - (WORLD_HEIGHT / 2) + (radius) / 2;
		}
		if (pos.y > 0 + (WORLD_HEIGHT / 2) - (radius) / 2) {
			vel.y *= -1;
			pos.y = 0 + (WORLD_HEIGHT / 2) - (radius) / 2;
		}
	}
	else {
		for (int i = 0; i < nodePositions.size(); i++) {
			if (nodePositions[i].x > 0 + (WORLD_WIDTH / 2) - (nodeRadiuses[i]) / 2) {
				nodeVelocities[i].x *= -1;
				nodePositions[i].x = 0 + (WORLD_WIDTH / 2) - (nodeRadiuses[i]) / 2;
			}
			if (nodePositions[i].x < 0 - (WORLD_WIDTH / 2) + (nodeRadiuses[i]) / 2) {
				nodeVelocities[i].x *= -1;
				nodePositions[i].x = 0 - (WORLD_WIDTH / 2) + (nodeRadiuses[i]) / 2;
			}
			if (nodePositions[i].y < 0 - (WORLD_HEIGHT / 2) + (nodeRadiuses[i]) / 2) {
				nodeVelocities[i].y *= -1;
				nodePositions[i].y = 0 - (WORLD_HEIGHT / 2) + (nodeRadiuses[i]) / 2;
			}
			if (nodePositions[i].y > 0 + (WORLD_HEIGHT / 2) - (nodeRadiuses[i]) / 2) {
				nodeVelocities[i].y *= -1;
				nodePositions[i].y = 0 + (WORLD_HEIGHT / 2) - (nodeRadiuses[i]) / 2;
			}
		}
	}
}

// simple ellipse collision detection
void GameObject::ellipseCollider()
{
	for (int i = 0; i < GameObjects->size(); i++) {
		if ((*GameObjects)[i]->ellipseCollider_enabled) {
			if ((*GameObjects)[i] != this) {
				if ((*GameObjects)[i]->isSpring == false) {
					if (CollisionDetector.EllipseCompare(pos, radius, (*GameObjects)[i]->pos, (*GameObjects)[i]->radius)) {
						isColliding((*GameObjects)[i]);
					}
				}
			}
		}
	}
}
// called when an object is currently colliding
void GameObject::isColliding(GameObject* _other, ofVec2f _nodePos)
{
	ofVec2f otherPos;
	if (_other->isSpring) {
		otherPos = _nodePos;
	}
	else {
		otherPos = _other->pos;
	}

	if (GameController->getUseHardCollisions()) {
		ofVec2f forceVec = pos - otherPos;
		if (prevPos != ofVec2f(99999, 99999)) pos = prevPos;
		//vel.set(0);
		applyForce(accel, (forceVec / mass), false);
	}
	else {
		ofVec2f forceVec = pos - otherPos;
		applyForce(accel, (forceVec / mass), true);
	}
}

void GameObject::gravity()
{
	if (nodePositions.size() == 0) {
		if (GameController->getGravity() == 1 || affectedByGravity) {
			ofVec2f newForce = { 0, (float)GRAVITY_FORCE * mass };
			applyForce(accel, newForce, false);
		}
	}
	else {
		for (int i = 0; i < nodePositions.size(); i++) {
			if (GameController->getGravity() == 1 || affectedByGravity) {
				ofVec2f newForce = { 0, (float)GRAVITY_FORCE * 400 * nodeMasses[i] };
				applyForce(nodeAccelerations[i], newForce, false);
			}
		}
	}
}

void GameObject::friction()
{
	if (nodePositions.size() == 0) {
		ofVec2f friction = vel * -1;
		friction *= FRICTION_FORCE;
		applyForce(accel, friction, true);
	}
	else {
		for (int i = 0; i < nodePositions.size(); i++) {
			ofVec2f friction = nodeVelocities[i] * -1;
			friction *= FRICTION_FORCE;
			applyForce(accel, friction, false);
		}
	}
}

// determines if the mouse is over an object
void GameObject::mouseHover()
{
	if (nodePositions.size() == 0) {
		if (GameController->getMouseDragged() == false) {
			if (CollisionDetector.EllipseCompare(pos, radius, ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y), 0)) {
					mouseOver = true;
					mouseOffsetFromCenter = pos - ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y);
			}
			else {
				mouseOver = false;			
			}
		}
	}
	else {
		if (GameController->getMouseDragged() == false) {
			for (int i = 0; i < nodePositions.size(); i++) {
				if (CollisionDetector.EllipseCompare(nodePositions[i], nodeRadiuses[i], GameController->getWorldMousePos(), 0)) {
					mouseOver = true;
					mouseOverIndex = i;
					mouseOffsetFromCenter = nodePositions[i] - GameController->getWorldMousePos();
					break;
				}
				else if (CollisionDetector.EllipseCompare(pos, radius, GameController->getWorldMousePos(), 0)) {
					mouseOver = true;
					mouseOverIndex = -1;
					mouseOffsetFromCenter = pos - GameController->getWorldMousePos();
					break;
				}
				else {
					mouseOver = false;
				}
			}
		}
	}
}


// Shared physics functions


void GameObject::applyForce(ofVec2f& _accel, ofVec2f _force, bool _limit, float _limitAmount)
{
	if (_limit) {
		_force.limit(_limitAmount);
		_accel += _force;
	}
	else {
		_accel += _force;
		addForces(false);
	}
}

void GameObject::addForces(bool _interpPos)
{
	if (nodePositions.size() == 0) {
		vel += accel;
		vel.limit(MAXIMUM_VELOCITY);
		if (_interpPos) {
			pos = getInterpolatedPosition();
		}
		else {
			pos += vel;
		}
	}
	else {
		/*for (int i = 0; i < nodePositions.size(); i++) {
			nodeVelocities[i] += nodeAccelerations[i];
			nodePositions[i] += nodeVelocities[i] * 0.28;
		}*/
	}
}

ofVec2f GameObject::getInterpolatedPosition()
{
	int progress = (ofGetFrameNum() % 100) / 100;
	ofVec2f powInterpIn;
	powInterpIn.x = ofNextPow2(progress);
	powInterpIn.y = ofNextPow2(progress);

	ofVec2f newPos;
	newPos.x = ofLerp(pos.x, pos.x + vel.x, powInterpIn.x);
	newPos.y = ofLerp(pos.y, pos.y + vel.y, powInterpIn.y);
	return newPos;
}


// ----- EVENT FUNCTIONS ----- //


// it is necessary for these functions to be declared here, as they are called on every gameobject
void GameObject::mousePressed(float _x, float _y, int _button)
{
}

void GameObject::mouseDragged(float _x, float _y, int _button)
{
}

void GameObject::mouseReleased(float _x, float _y, int _button)
{
}

// functions the same as root_update
void GameObject::root_keyPressed(int key)
{
	if (key == 120) {
		deleteKeyDown = true;
	}
	keyPressed(key);
}

void GameObject::keyPressed(int key)
{
}
// ^^
void GameObject::root_keyReleased(int key)
{
	if (key == 120) {
		deleteKeyDown = false;
	}
	keyReleased(key);
}

void GameObject::keyReleased(int key)
{
}


// ----- RENDER LOOP ----- //


void GameObject::root_draw()
{
	if (!needs_to_be_deleted) {
		draw();
	}
	else {
		//cout << "Error: 'Dead' GameObject is still being rendered" << endl;
	}
}

void GameObject::draw()
{
	static bool initialized = false;
	if (!initialized)
	{
		//cout << "Error: User hasn't defined unique 'draw' function for a GameObject" << endl;
		initialized = true;
	}
}