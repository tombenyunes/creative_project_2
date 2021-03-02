#pragma once

#include "ofMain.h"
#include "GameObject.h"
#include "Controller.h"
#include "Player.h"
#include "Mass.h"
#include "Spring.h"

#include "ofxGui.h"
#include "GUIManager.h"
#include "Collisions.h"
#include "EventManager.h"

#include "MSAFluid.h"
#include "ParticleSystem.h"
#include "ofxSimpleGuiToo.h"

#include "SceneManager.h"
#include "Camera.h"

#include "FluidManager.h"

#include "ofxBlur.h"

#include "AudioManager.h"

using namespace glm;

class ofApp : public ofBaseApp{

	public:
		void setup();

		void audioOut(float* output, int bufferSize, int nChannels);

		void update();

		void draw();
		void drawRequiredGUI();

		void createNode();

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

		AudioManager Audio_Manager;

		// ---> Core Setup <--- //

		vector <GameObject*> vec;
		vector <GameObject*>* GameObjects = &vec; // the main vector of all objects in the scene

		Controller GameController; // this controls global game properties (the selected object, gravity, etc)
		GUIManager GUI_Manager;
		
		Collisions CollisionDetector;
		EventManager Events; // simple system for allowing relevant interactions/gui interfacts - only used for the starting tutorial

		SceneManager Scene_Manager;

		ofxSimpleGuiToo gui;
		
		Camera cam;

		ofxBlur blur;


		// ---> Fluid Setup <--- //

		FluidManager Fluid_Manager;
		bool drawParticleGUI;
		bool drawAudioGUI;
		
		float vectorLength;
		
		

};