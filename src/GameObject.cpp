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

	hasMultipleNodes = false;

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

// root update is called prir to the main update function of a gameobject and is responsible for handling object deletion and updating user-added modules - it automatically updates the main update funcion
void GameObject::root_update(vector<GameObject*>* _gameobjects, Controller* _controller, guiController* _guiController, msa::fluid::Solver* _fluidSolver, ParticleSystem* _particleSystem, Camera* _cam)
{
	cam = _cam; // necessary for calculating mouse positions

	if (deleteKeyDown) {
		if (mouseOver) {
			needs_to_be_deleted = true;
		}		
	}
	if (!needs_to_be_deleted) {

		GameObjects = _gameobjects;
		GameController = _controller;
		gui_Controller = _guiController;
		fluidSolver = _fluidSolver;
		particleSystem = _particleSystem;

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

		//addToFluid(ofVec2f(ofMap(pos.x, -ofGetWidth() / 2, ofGetWidth() / 2, 0, 1), ofMap(pos.y, -ofGetHeight() / 2, ofGetHeight() / 2, 0, 1)), vel / 600, false, true);
		/*ofVec2f newPos;
		newPos.x = ofMap(pos.x + ofRandom(-radius / 4, radius / 4), -ofGetWidth() / 2, ofGetWidth() / 2, 0, 1);
		newPos.y = ofMap(pos.y + ofRandom(-radius / 4, radius / 4), -ofGetHeight() / 2, ofGetHeight() / 2, 0, 1);
		ofVec2f newVel;
		newVel.x = ((vel.x + ofRandom(-1, 1)) / 600) * -1;
		newVel.y = ((vel.y + ofRandom(-1, 1)) / 600) * -1;
		addToFluid(newPos, newVel, true, true);*/
		
		update(); // <--- user defined update function for every gameobject
	}
	else {		
		//cout << "Error: 'Dead' GameObject is still being updated" << endl;
	}
}

void GameObject::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce, int count) {
	float speed = vel.x * vel.x + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
	if (speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);

		int index = fluidSolver->getIndexForPos(pos);

		if (addColor) {
			//			Color drawColor(CM_HSV, (getElapsedFrames() % 360) / 360.0f, 1, 1);
			ofColor drawColor;
			drawColor.setHsb((ofGetFrameNum() % 255), 255, 255);

			fluidSolver->addColorAtIndex(index, drawColor * 1);

			particleSystem->addParticles(pos * ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), count);
		}

		if (addForce)
			fluidSolver->addForceAtIndex(index, vel * 1);

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
	if (!hasMultipleNodes) {
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
	if (!hasMultipleNodes) {
		for (int i = 0; i < GameObjects->size(); i++) {
			if ((*GameObjects)[i]->isSpring == false) {
				if (((*GameObjects)[i] != this) && ((*GameObjects)[i]->ellipseCollider_enabled)) {
					if (CollisionDetector.EllipseCompare(pos, radius, (*GameObjects)[i]->pos, (*GameObjects)[i]->radius)) {

						isColliding((*GameObjects)[i]);
					}					
				}
			}
		}
	}
	else {
		for (int i = 0; i < GameObjects->size(); i++) {
			if ((*GameObjects)[i]->isSpring == false) {
				if (((*GameObjects)[i] != this) && ((*GameObjects)[i]->ellipseCollider_enabled)) {
					for (int j = 0; j < nodePositions.size(); j++) {
						if (CollisionDetector.EllipseCompare(nodePositions[j], nodeRadiuses[j], (*GameObjects)[i]->pos, (*GameObjects)[i]->radius)) {
							
							isColliding((*GameObjects)[i], ofVec2f(0, 0), 0);
							(*GameObjects)[i]->isColliding(this, nodePositions[j]);
						}
					}
				}
			}
		}
	}

}
// called when an object is currently colliding
void GameObject::isColliding(GameObject* _other, ofVec2f _nodePos, int _nodeIndex)
{
	ofVec2f otherPos = _other->pos;
	if (_nodePos != ofVec2f(99999, 99999)) otherPos = _nodePos;

	if (!hasMultipleNodes) {
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
	else {
		ofVec2f forceVec = nodePositions[_nodeIndex] - _other->pos;
		ofVec2f accel = forceVec / nodeMasses[_nodeIndex];
		applyForce(nodeAccelerations[_nodeIndex], accel, false);
	}
}

void GameObject::gravity()
{
	if (!hasMultipleNodes) {
		if (GameController->getGravity() == 1 || affectedByGravity) {
			ofVec2f gravity = { 0, (float)GRAVITY_FORCE * mass };
			applyForce(accel, gravity, false);
		}
	}
	else {
		for (int i = 0; i < nodePositions.size(); i++) {
			if (GameController->getGravity() == 1 || affectedByGravity) {
				ofVec2f gravity = { 0, (float)GRAVITY_FORCE * 400 * nodeMasses[i] };
				applyForce(nodeAccelerations[i], gravity, false);
			}
		}
	}
}

void GameObject::friction()
{
	if (!hasMultipleNodes) {
		ofVec2f friction = vel * -1;
		friction *= FRICTION_FORCE;
		applyForce(accel, friction, true);
	}
	else {
		for (int i = 0; i < nodePositions.size(); i++) {
			ofVec2f friction = nodeVelocities[i] * -1;
			friction *= FRICTION_FORCE;
			applyForce(nodeAccelerations[i], friction, true);
		}
	}
}

// determines if the mouse is over an object
void GameObject::mouseHover()
{
	if (!hasMultipleNodes) {
		if (CollisionDetector.EllipseCompare(pos, radius, ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y), 0)) {
			if (GameController->getMouseDragged() == false) {
				color = ofColor(255, 165, 0);
				mouseOver = true;
				mouseOffsetFromCenter = pos - ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y);
			}
		}
		else {
			color = ofColor(255);
			mouseOver = false;
			mouseOffsetFromCenter.set(0);
		}
	}
	else {
		if (GameController->getMouseDragged() == false) {
			for (int i = 0; i < nodePositions.size(); i++) {
				// compare with all nodes
				if (CollisionDetector.EllipseCompare(nodePositions[i], nodeRadiuses[i], ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y), 0)) {
					color = ofColor(255, 165, 0);
					mouseOver = true;
					mouseOverIndex = i;
					mouseOffsetFromCenter = nodePositions[i] - ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y);
					break;
				}
				// compare with root / anchor node
				else if ((CollisionDetector.EllipseCompare(pos, radius, ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y), 0))) {
					color = ofColor(255, 165, 0);
					mouseOver = true;
					mouseOverIndex = -1;
					mouseOffsetFromCenter = pos - ofVec2f(GameController->getWorldMousePos(cam).x, GameController->getWorldMousePos(cam).y);
					//break;
				}
				else {
					color = ofColor(255);
					mouseOver = false;
					mouseOffsetFromCenter.set(0);
					//break;
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
		if (!hasMultipleNodes) addForces(false);
	}
}

void GameObject::addForces(bool _interpPos)
{
	if (!hasMultipleNodes) {
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
		for (int i = 0; i < nodePositions.size(); i++) {
			nodeVelocities[i] += nodeAccelerations[i];
			nodePositions[i] += nodeVelocities[i] * 0.28;
		}
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