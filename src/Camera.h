#pragma once

#include "ofMain.h"
//#include "GameObject.h"

class Camera {

public:
	
	Camera();
	
	void update(int width, int height, ofVec2f _playerPos);

	void toggleZoomMode();

	ofVec3f getPosition();
	void begin();
	void end();
	ofVec3f screenToWorld(ofVec3f _view);

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseScrolled(int x, int y, float scrollX, float scrollY);

private:

	ofEasyCam cam;
	float zoomDistance;
	float scale;
	bool ctrlDown = false;
	bool followPlayer;
	
};