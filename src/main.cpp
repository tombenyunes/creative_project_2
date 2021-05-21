#include "ofMain.h"
#include "ofApp.h"

int main()
{
	ofGLWindowSettings settings;
	settings.setSize(1900, 1000);
	settings.setGLVersion(2, 1);
	ofCreateWindow(settings);
	ofRunApp(new ofApp());
}