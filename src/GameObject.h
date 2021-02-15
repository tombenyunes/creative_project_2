#pragma once

#include "ofMain.h"
#include "Controller.h"
#include "guiController.h"
#include "Collisions.h"

#include "ParticleSystem.h"

class GameObject {
	
public:

	GameObject(ofVec2f _pos = { 0, 0 }, ofColor _color = ofColor(255));

	void root_update(vector<GameObject*>* _gameobjects, Controller* _controller, guiController* _guiController, msa::fluid::Solver* _fluidSolver, ParticleSystem* _particleSystem, Camera* _cam);
	void root_draw();

	virtual void isColliding(GameObject* _other, ofVec2f _nodePos = { 0, 0 });
	bool ellipseCollider_enabled;

	void root_keyPressed(int key);
	void root_keyReleased(int key);
	
	virtual void mousePressed(float _x, float _y, int _button);
	virtual void mouseDragged(float _x, float _y, int _button);
	virtual void mouseReleased(float _x, float _y, int _button);

	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce, int count = 10);

	vector<GameObject*>* GameObjects;
	Controller* GameController;
	guiController* gui_Controller;

	msa::fluid::Solver* fluidSolver;
	ParticleSystem* particleSystem;
	
	Collisions CollisionDetector;

	string type;

	ofVec2f pos;
	ofVec2f prevPos;
	float mass;
	float radius;

	ofVec2f nodePos1;
	ofVec2f nodeVel1;
	ofVec2f nodeAccel1;
	float nodeRadius1;
	float nodeMass1;

	ofVec2f nodePos2;
	ofVec2f nodeVel2;
	ofVec2f nodeAccel2;
	float nodeRadius2;
	float nodeMass2;

	bool isPlayer;
	bool isSpring;
	
	bool needs_to_be_deleted;
	ofVec2f mouseOffsetFromCenter;

protected:

	// Modules
	virtual void screenWrap();
	bool screenWrap_enabled;
	virtual void screenBounce();
	bool screenBounce_enabled;
	virtual void gravity();
	bool gravity_enabled;
	virtual void friction();
	bool friction_enabled;
	virtual void mouseHover();
	bool mouseHover_enabled;
	virtual void ellipseCollider();
	
	virtual void applyForce(ofVec2f& _accel, ofVec2f _force, bool _limit = true, float _limitAmount = MAXIMUM_ACCELERATION);
	virtual void addForces(bool _interpPos);
	virtual ofVec2f getInterpolatedPosition();

	virtual void update();
	virtual void draw();

	virtual void keyPressed(int key);
	virtual void keyReleased(int key);

	ofVec2f vel;
	ofVec2f accel;
	ofColor color;
	
	bool infiniteMass;
	bool affectedByGravity;
	
	Camera* cam;
	bool mouseOver;
	bool deleteKeyDown;

	void AddModule(string _id);

};