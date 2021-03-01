#include "ofMain.h"
#include "ofApp.h"

int main( ){
	ofSetupOpenGL(1900, 1000, OF_WINDOW);
	//ofSetupOpenGL(1920, 1080, OF_FULLSCREEN);
	//ofSetupOpenGL(1920, 1080, OF_GAME_MODE);
	
	ofRunApp(new ofApp());
}