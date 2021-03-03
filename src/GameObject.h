#pragma once

#include "ofMain.h"
#include "Controller.h"
#include "GUIManager.h"
#include "Collisions.h"
#include "FluidManager.h"
#include "AudioManager.h"

class GameObject {
	
public:

	GameObject(ofVec2f _pos = { 0, 0 }, ofColor _color = ofColor(255));
	void init(vector<GameObject*>* _gameobjects, Controller* _controller, GUIManager* _GUIManager, Camera* _cam, FluidManager* _fluidManager, AudioManager* _audioManager);

	void root_update();
	void root_draw();

	virtual void isColliding(GameObject* _other, ofVec2f _nodePos = { 0, 0 });
	bool ellipseCollider_enabled;

	void root_keyPressed(int key);
	void root_keyReleased(int key);
	
	virtual void mousePressed(float _x, float _y, int _button);
	virtual void mouseDragged(float _x, float _y, int _button);
	virtual void mouseReleased(float _x, float _y, int _button);

	vector<GameObject*>* GameObjects;
	Controller* GameController;
	GUIManager* GUI_Manager;
	FluidManager* Fluid_Manager;
	AudioManager* Audio_Manager;
	
	Camera* cam;
	
	Collisions CollisionDetector;

	string type;

	ofVec2f pos;
	ofVec2f prevPos;
	float mass;
	float radius;	

	vector<ofVec2f> nodePositions;
	vector<ofVec2f> nodeVelocities;
	vector<ofVec2f> nodeAccelerations;
	vector<float> nodeRadiuses;
	vector<float> nodeMasses;

	bool isPlayer;
	bool isSpring;
	
	bool needs_to_be_deleted;
	ofVec2f mouseOffsetFromCenter;

	bool requestToBeSelected;
	bool isSelected;

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
	int gravityMult = 1;
	int collisionMult = 1;
		
	bool mouseOver;
	int mouseOverIndex;
	int selectedNodeIndex;
	bool mouseDrag;

	ofColor passiveColor;
	ofColor selectedColor;

	bool deleteKeyDown;

	void AddModule(string _id);

};