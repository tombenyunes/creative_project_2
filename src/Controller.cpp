#include "Controller.h"

Controller::Controller()
{
	ACTIVE_OBJECT = nullptr;
	GRAVITY = 0;
	NEED_TO_DELETE_ALL = false;
	OBJECT_SELECTED = false;
	MOUSE_BEING_DRAGGED = false;
	NEW_NODE_ID = 0;
	HARD_COLLISIONS = false;
	GUI_VISIBLE = false;
	USER_CAN_INPUT = false;
}

void Controller::update(Camera* cam)
{
	ofVec3f localPos = ofVec3f(ofGetMouseX() - WORLD_WIDTH / 2, ofGetMouseY() - WORLD_HEIGHT / 2, 0);
	ofVec3f worldPos = cam->screenToWorld(localPos);
	MOUSE_POS = worldPos;
}

ofVec3f Controller::getWorldMousePos()
{
	return MOUSE_POS;
}

void Controller::makeActive(GameObject* _this)
{
	ACTIVE_OBJECT = _this;
}
GameObject* Controller::getActive()
{
	return ACTIVE_OBJECT;
}
void Controller::setGravity(bool _value)
{
	(_value == 1) ? GRAVITY = 1 : GRAVITY = 0;
}
bool Controller::getGravity()
{
	return GRAVITY;
}
void Controller::setDeleteAll(bool _value)
{
	(_value == 1) ? NEED_TO_DELETE_ALL = 1 : NEED_TO_DELETE_ALL = 0;
}
bool Controller::getDeleteAll()
{
	return NEED_TO_DELETE_ALL;
}
void Controller::setObjectSelected(bool _value)
{
	(_value == 1) ? OBJECT_SELECTED = 1 : OBJECT_SELECTED = 0;
}
bool Controller::getObjectSelected()
{
	return OBJECT_SELECTED;
}
void Controller::setMouseDragged(bool _value)
{
	(_value == 1) ? MOUSE_BEING_DRAGGED = 1 : MOUSE_BEING_DRAGGED = 0;
}
bool Controller::getMouseDragged()
{
	return MOUSE_BEING_DRAGGED;
}
void Controller::setNewNodeType(int _value)
{
	NEW_NODE_ID = _value;
}
int Controller::getNewNodeType()
{
	return NEW_NODE_ID;
}
void Controller::setUseHardCollisions(bool _value)
{
	(_value == 1) ? HARD_COLLISIONS = 1 : HARD_COLLISIONS = 0;
}
bool Controller::getUseHardCollisions()
{
	return HARD_COLLISIONS;
}
void Controller::setGUIVisible(bool _value)
{
	(_value == 1) ? GUI_VISIBLE = 1 : GUI_VISIBLE = 0;
}
bool Controller::getGUIVisible()
{
	return GUI_VISIBLE;
}
void Controller::setCanUserInput(bool _value)
{
	(_value == 1) ? USER_CAN_INPUT = 1 : USER_CAN_INPUT = 0;
}
bool Controller::getCanUserInput()
{
	return USER_CAN_INPUT;
}