#pragma once

#include "ofMain.h"

#define MINIMUM_MASS 1
#define MAXIMUM_MASS 5000

#define MASS_LOWER_BOUND 250
#define MASS_UPPER_BOUND 2500

#define RADIUS_MINIMUM 1
#define RADIUS_MAXIMUM 500

#define RADIUS_LOWER_BOUND 25
#define RADIUS_UPPER_BOUND 250

#define MAXIMUM_ACCELERATION 0.15 // 0.15
#define MAXIMUM_VELOCITY 100 // 15

#define FRICTION_FORCE 0.015
#define GRAVITY_FORCE 0.0001

#define WORLD_WIDTH 2000*1
#define WORLD_HEIGHT 1500*1

class GameObject;

#include "Camera.h"


class Controller {

public:
	
	Controller();

	void update(Camera* cam);

	ofVec3f getWorldMousePos();

	void makeActive(GameObject* _this);
	GameObject* getActive();

	void setGravity(bool _value);
	bool getGravity();

	void setObjectSelected(bool _value);
	bool getObjectSelected();

	void setMouseDragged(bool _value);
	bool getMouseDragged();

	void setNewNodeType(bool _value);
	int getNewNodeType();

	void setUseHardCollisions(bool _value);
	bool getUseHardCollisions();

	void setDeleteAll(bool _value);
	bool getDeleteAll();

	void setGUIVisible(bool _value);
	bool getGUIVisible();

	void setCanUserInput(bool _value);
	bool getCanUserInput();

private:

	ofVec3f MOUSE_POS;
	GameObject* ACTIVE_OBJECT;
	bool GRAVITY;
	bool NEED_TO_DELETE_ALL;
	bool OBJECT_SELECTED;
	bool MOUSE_BEING_DRAGGED;
	int NEW_NODE_NAME;
	bool HARD_COLLISIONS;
	bool GUI_VISIBLE;
	bool USER_CAN_INPUT;
	
};