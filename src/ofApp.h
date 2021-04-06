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
		void setup() override;

		void audioOut(float* output, int bufferSize, int nChannels) override;

		void update() override;

		void draw() override;

		void keyPressed(int key) override;
		void keyReleased(int key) override;
		void mouseMoved(int x, int y ) override;
		void mouseDragged(int x, int y, int button) override;
		void mousePressed(int x, int y, int button) override;
		void mouseScrolled(int x, int y, float scrollX, float scrollY) override;
		void mouseReleased(int x, int y, int button) override;
		void mouseEntered(int x, int y) override;
		void mouseExited(int x, int y) override;
		void windowResized(int w, int h) override;
		void dragEvent(ofDragInfo dragInfo) override;
		void gotMessage(ofMessage msg) override;		

		EntityManager entity_manager;
		Controller game_controller; // controls global game properties
		GUIManager gui_manager;
		SceneManager scene_manager;
		GameModeManager gamemode_manager{ 1 };
		EventManager event_manager; // will be used for starting tutorial
		AudioManager audio_manager;

		Camera cam;		

		FluidManager fluid_manager;
		ofxBlur fluid_blur;

		ofShader test_shader;

};