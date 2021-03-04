#pragma once

#include "ofMain.h"

#include "GameObject.h"
#include "Controller.h"
#include "Camera.h"

#include "SceneManager.h"
#include "EntityManager.h"
#include "GUIManager.h"
#include "FluidManager.h"
#include "EventManager.h"
#include "AudioManager.h"
#include "GameModeManager.h"

#include "MSAFluid.h"
#include "ParticleSystem.h"
#include "ofxBlur.h"

using namespace glm;

class ofApp : public ofBaseApp{

	public:
		void setup();

		void audioOut(float* output, int bufferSize, int nChannels);

		void update();

		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseScrolled(int x, int y, float scrollX, float scrollY);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		EntityManager Entity_Manager;
		Controller GameController; // controls global game properties
		GUIManager GUI_Manager;
		SceneManager Scene_Manager;
		GameModeManager GameMode_Manager{ 1 };
		EventManager Event_Manager; // will be used for starting tutorial
		AudioManager Audio_Manager;

		Camera cam;		

		FluidManager Fluid_Manager;
		ofxBlur fluidBlur;	

};