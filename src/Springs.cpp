#include "Springs.h"

Springs::Springs(ofVec2f _anchorPos, float _nodeRadius1, float _nodeMass1, float _nodeRadius2, float _nodeMass2, float _k, float _damping, float _springmass)
{
	type = "Spring";

	color = ofColor(passiveColor);

	mouseOver = false;
	mouseDrag = false;

	gui_values_need_to_be_set = true;

	isSpring = true;
	affectedByGravity = true;

	k = _k;
	damping = _damping;
	springmass = _springmass;

	pos = _anchorPos;
	radius = 8;
	timeStep = 0.28;
	
	createNode(ofVec2f(pos.x + ofRandom(-50, 50), pos.y), ofVec2f(0, 0), ofVec2f(0, 0), _nodeRadius1, _nodeMass1);
	createNode(ofVec2f(pos.x + ofRandom(-50, 50), pos.y), ofVec2f(0, 0), ofVec2f(0, 0), _nodeRadius2, _nodeMass2);
	
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
	posBeforeDrag.set(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y);
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
		if (gui_values_need_to_be_set) {					
			if (selectedNodeIndex != -1) {
				gui_Controller->updateSpringValues(pos, k, damping, springmass, affectedByGravity, nodePositions[selectedNodeIndex], nodeVelocities[selectedNodeIndex], nodeAccelerations[selectedNodeIndex], nodeMasses[selectedNodeIndex], nodeRadiuses[selectedNodeIndex]);
			}
			else {
				gui_Controller->updateSpringValues(pos, k, damping, springmass, affectedByGravity);
			}
			gui_values_need_to_be_set = false;
		}
		else {		
			if (selectedNodeIndex != -1) {				
				gui_Controller->updateSpringValues(pos, k, damping, springmass, affectedByGravity, nodePositions[selectedNodeIndex], nodeVelocities[selectedNodeIndex], nodeAccelerations[selectedNodeIndex], gui_Controller->nodeMass, gui_Controller->nodeRadius);
				nodeMasses[selectedNodeIndex] = gui_Controller->nodeMass;
				nodeRadiuses[selectedNodeIndex] = gui_Controller->nodeRadius;
			}
			else {
				gui_Controller->updateSpringValues(pos, gui_Controller->k, gui_Controller->damping, gui_Controller->springmass, gui_Controller->spring_affectedByGravity);
				k = gui_Controller->k;
				damping = gui_Controller->damping;
				springmass = gui_Controller->springmass;
				affectedByGravity = gui_Controller->spring_affectedByGravity;
			}
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

//void Springs::mouseHover()
//{
//	if (GameController->getMouseDragged() == false) {
//		if (CollisionDetector.EllipseCompare(nodePositions[0], nodeRadiuses[0], GameController->getWorldMousePos(), 0)) {
//			mouseOver = true;
//			mouseOverIndex = 0;
//			mouseOffsetFromCenter = nodePositions[0] - GameController->getWorldMousePos();
//		}
//		else if (CollisionDetector.EllipseCompare(nodePositions[1], nodeRadiuses[1], GameController->getWorldMousePos(), 0)) {
//			mouseOver = true;
//			mouseOverIndex = 1;
//			mouseOffsetFromCenter = nodePositions[1] - GameController->getWorldMousePos();
//		}
//		else if (CollisionDetector.EllipseCompare(pos, radius, GameController->getWorldMousePos(), 0)) {
//			mouseOver = true;
//			mouseOverIndex = -1;
//			mouseOffsetFromCenter = pos - GameController->getWorldMousePos();
//		}
//		else {			
//			mouseOver = false;
//			mouseOffsetFromCenter.set(0);
//			
//			color = ofColor(passiveColor);
//		}
//	}
//}


// ----- EVENT FUNCTIONS ----- //


void Springs::mousePressed(float _x, float _y, int _button)
{
	if (_button == 2 && mouseOver) {
		if (GameController->getActive() != this) {
			GameController->makeActive(this);
		}
		gui_values_need_to_be_set = true;
		selectedNodeIndex = mouseOverIndex;		
	}
}

void Springs::mouseDragged(float _x, float _y, int _button)
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

void Springs::mouseReleased(float _x, float _y, int _button)
{
	if (_button == 2) {
		mouseDrag = false;
		GameController->setMouseDragged(false);
	}

}

void Springs::keyPressed(int key)
{
	if (key == 'a') {
		createNode(ofVec2f(pos.x + ofRandom(-50, 50), pos.y), ofVec2f(0, 0), ofVec2f(0, 0), nodeRadiuses[nodeRadiuses.size() - 1], nodeMasses[nodeRadiuses.size() - 1]);
	}
}


// ----- RENDER LOOP ----- //


void Springs::draw()
{
	ofPushStyle();

	ofNoFill();
	ofSetColor(color);
	getNodeColor(-2);

	drawConnectingLines();
	
	(fillEllipses) ? ofFill() : ofNoFill();
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
	if ((GameController->getActive() == this) && (selectedNodeIndex == -1 || selectedNodeIndex == _node || ((mouseOver || mouseDrag) && mouseOverIndex == _node))) {
		ofSetColor(selectedColor);
	}
	else {
		ofSetColor(passiveColor);
	}
}

void Springs::drawConnectingLines()
{
	for (int i = 0; i < nodePositions.size(); i++) {
		if (i == 0) {
			if (fillEllipses) {
				ofLine(nodePositions[i].x, nodePositions[i].y, pos.x, pos.y);
			}
			else {
				float from = angleBetween(nodePositions[i], pos);
				float to = angleBetween(pos, nodePositions[i]);

				ofVec2f pointFrom = getPointOnCircle(nodePositions[i], from, nodeRadiuses[i] / 2);
				ofVec2f pointTo = getPointOnCircle(pos, to, radius / 2);

				if (CollisionDetector.EllipseCompare(nodePositions[i], nodeRadiuses[i], pos, radius) == false) {
					ofLine(pointFrom.x, pointFrom.y, pointTo.x, pointTo.y);
				}
			}
		}
		else {
			if (fillEllipses) {
				ofLine(nodePositions[i].x, nodePositions[i].y, nodePositions[i - 1].x, nodePositions[i - 1].y);
			}
			else {
				float from = angleBetween(nodePositions[i], nodePositions[i - 1]);
				float to = angleBetween(nodePositions[i - 1], nodePositions[i]);

				ofVec2f pointFrom = getPointOnCircle(nodePositions[i], from, nodeRadiuses[i] / 2);
				ofVec2f pointTo = getPointOnCircle(nodePositions[i - 1], to, nodeRadiuses[i - 1] / 2);

				if (CollisionDetector.EllipseCompare(nodePositions[i], nodeRadiuses[i], nodePositions[i - 1], nodeRadiuses[i - 1]) == false) {
					ofLine(pointFrom.x, pointFrom.y, pointTo.x, pointTo.y);
				}
			}
		}
	}
}

float Springs::angleBetween(ofVec2f from, ofVec2f to)
{
	float x = from.x;
	float y = from.y;

	float deltaX = to.x - x;
	float deltaY = to.y - y;

	float rotation = -atan2(deltaX, deltaY);
	rotation += 3.14159 / 2;

	return rotation;
}

ofVec2f Springs::getPointOnCircle(ofVec2f center, float radians, float  radius) {

	float x = center.x;
	float y = center.y;

	//radians = radians - Math.toRadians(90.0); // 0 becomes the top
	
	float xPosy = round((float)(x + cos(radians) * radius));
	float yPosy = round((float)(y + sin(radians) * radius));

	ofVec2f point = ofVec2f(xPosy, yPosy);

	return point;

}