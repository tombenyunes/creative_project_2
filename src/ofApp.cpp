#include "ofApp.h"

void ofApp::setup()
{
	ofSetWindowPosition(3849, 649);
	ofSetWindowTitle("iota");
	ofSetCircleResolution(176);
	ofBackground(0);
	ofSetVerticalSync(true);
	windowResized(ofGetWidth(), ofGetHeight());
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);

	Entity_Manager.init(&GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
	Scene_Manager.init(&GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager, &Entity_Manager);
	GUI_Manager.init(&GameController, &Fluid_Manager, &Audio_Manager);
	
	GameController.init(&cam);

	Event_Manager.init(&GameController, &Entity_Manager);
	Event_Manager.showTutorial(false);
	Event_Manager.setup();

	Audio_Manager.setup(this);
	
	Scene_Manager.loadScene("Scenes/StartingScene");

	fluidBlur.setup(WORLD_WIDTH, WORLD_HEIGHT, 32, .2, 2);
	//fluidBlur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//fluidBlur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));	
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels)
{
	//Audio_Manager.audioOut(output, bufferSize, nChannels);
}

void ofApp::update()
{	
	Entity_Manager.updateGameObjects();
	GameController.update();
	GUI_Manager.update();
	Event_Manager.update();
	Fluid_Manager.update();
	Audio_Manager.update();

	//fluidBlur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//fluidBlur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void ofApp::draw()
{
	cam.begin();

	fluidBlur.begin();
	Fluid_Manager.renderFluid();
	fluidBlur.end();
	fluidBlur.draw(); // blur only applies to background fluid
	
	Fluid_Manager.renderParticles();	

	Audio_Manager.draw(); // background animation effect

	ofPushMatrix();

	ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);
	Entity_Manager.drawGameObjects();
	Event_Manager.drawTutorial();

	ofPopMatrix();	
	cam.end();

	if (GameController.getActive() != nullptr && GameController.getActive()->isSpring) {
		GUI_Manager.drawRequiredGUI(true);
	}
	else {
		GUI_Manager.drawRequiredGUI(false);
	}
}

void ofApp::keyPressed(int key)
{
	Audio_Manager.keyPressed(key);

	cam.keyPressed(key);
	Fluid_Manager.keyPressed(key);
	Event_Manager.keyPressed(key);
	Scene_Manager.keyPressed(key);
	Fluid_Manager.keyPressed(key);
	GUI_Manager.keyPressed(key);

	if (Event_Manager.isEventAllowed("keyPressed")) {
		Entity_Manager.keyPressed(key);
	}

	if (key == 'f') {
		ofToggleFullscreen();
	}
}

void ofApp::keyReleased(int key)
{
	Audio_Manager.keyPressed(key);
	cam.keyReleased(key);

	if (Event_Manager.isEventAllowed("keyReleased")) {
		Entity_Manager.keyReleased(key);
	}
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
	if (Event_Manager.isEventAllowed("mouseDragged", button)) {
		Entity_Manager.mouseDragged(x, y, button);
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	if (Event_Manager.isEventAllowed("mousePressed", button)) {
		Entity_Manager.mousePressed(x, y, button);
	}
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	cam.mouseScrolled(x, y, scrollX, scrollY);
}

void ofApp::mouseReleased(int x, int y, int button)
{
	if (Event_Manager.isEventAllowed("mouseReleased", button)) {
		Entity_Manager.mouseReleased(x, y, button);
	}
}

void ofApp::mouseEntered(int x, int y)
{
}

void ofApp::mouseExited(int x, int y)
{
}

void ofApp::windowResized(int w, int h)
{
	GUI_Manager.windowResized(w, h);
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}