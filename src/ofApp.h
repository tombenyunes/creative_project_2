#pragma once

#include "ofMain.h"
#include "GameObject.h"
#include "Controller.h"
#include "Player.h"
#include "Object.h"
#include "Springs.h"

#include "ofxGui.h"
#include "guiController.h"
#include "Collisions.h"
#include "EventManager.h"

#include "MSAFluid.h"
#include "ParticleSystem.h"
#include "ofxSimpleGuiToo.h"

#include "SceneManager.h"

using namespace glm;

class ofApp : public ofBaseApp{

	public:
		void setup();
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
		

		// ---> Core Setup <--- //

		vector <GameObject*> vec;
		vector <GameObject*>* GameObjects = &vec; // the main vector of all objects in the scene

		Controller* GameController; // this controls global game properties (the selected object, gravity, etc)
		guiController* gui_Controller;
		
		Collisions CollisionDetector;
		EventManager Events; // simple system for allowing relevant interactions/gui interfacts - only used for the starting tutorial

		SceneManager Scene_Manager;

		ofxSimpleGuiToo gui;
		
		ofEasyCam cam;
		float zoomDistance;
		float scale;
		bool ctrlDown = false;
		bool followPlayer = false;
		
		ofLight keyLight;


		// ---> Fluid Setup <--- //

		float colorMult;
		float velocityMult;
		int fluidCellsX;
		bool resizeFluid;
		bool drawFluid;
		bool drawParticles;

		bool drawParticleGUI;

		msa::fluid::Solver fluidSolver;
		msa::fluid::DrawerGl fluidDrawer;

		ParticleSystem particleSystem;
		
		float vectorLength;
		
		void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce, int count = 10);

};