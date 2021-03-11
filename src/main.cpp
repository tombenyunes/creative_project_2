#include "ofMain.h"
#include "ofApp.h"

int main( ){
	//ofSetupOpenGL(1900, 1000, OF_WINDOW);

	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
	//ofSetupOpenGL(1920, 1080, OF_GAME_MODE);
	
	//ofRunApp(new ofApp());

	ofGLWindowSettings settings;
	settings.setSize(1900, 1000);
	//settings.setGLVersion(3, 2);
	settings.setGLVersion(2, 1);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}