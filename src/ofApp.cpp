#include "ofApp.h"

void ofApp::setup()
{
	//ofSetWindowPosition(3849, 649);
	ofSetWindowShape(1920, 1080);
	ofSetWindowPosition((ofGetScreenWidth() - ofGetWindowWidth()) / 2, (ofGetScreenHeight() - ofGetWindowHeight()) / 2);
	ofSetWindowTitle("iota");
	ofSetCircleResolution(176);
	ofBackground(0);
	ofSetVerticalSync(true);
	windowResized(ofGetWidth(), ofGetHeight());
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);

	entity_manager.init(&game_controller, &gui_manager, &cam, &fluid_manager, &audio_manager, &gamemode_manager);
	scene_manager.init(&game_controller, &gui_manager, &cam, &fluid_manager, &audio_manager, &entity_manager, &gamemode_manager);
	gui_manager.init(&game_controller, &fluid_manager, &audio_manager, &gamemode_manager, &cam);
	
	game_controller.init(&cam);

	event_manager.init(&game_controller, &entity_manager);
	event_manager.showTutorial(false);
	event_manager.setup();

	audio_manager.setup(this);
	
	//Scene_Manager.loadScene("Scenes/StartingScene");
	scene_manager.loadProceduralScene();

	fluid_blur.setup(WORLD_WIDTH, WORLD_HEIGHT, 32, .2, 2);
	//fluidBlur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//fluidBlur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));

	//testShader.load("testShader");
	test_shader.load("testShader.vert", "testShader.frag");
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels)
{
	//Audio_Manager.audioOut(output, bufferSize, nChannels);
}

void ofApp::update()
{	
	entity_manager.updateGameObjects();
	game_controller.update();
	gui_manager.update();
	event_manager.update();
	fluid_manager.update();
	audio_manager.update();
	scene_manager.update();

	//fluidBlur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//fluidBlur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void ofApp::draw()
{
	//ofDisableDepthTest();

	cam.begin();

	fluid_blur.begin();
	fluid_manager.renderFluid();
	fluid_blur.end();
	fluid_blur.draw(); // blur only applies to background fluid
	
	fluid_manager.renderParticles();

	//Audio_Manager.draw(); // background animation effect

	ofPushMatrix();
	//cout << ofGetElapsedTimef() << endl;	
	auto t = ofGetFrameNum();
	t /= 100;
	//cout << (t / 100) << endl;
	ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);

	//testShader.begin();
	test_shader.setUniform1f("time", t);
	test_shader.setUniform1f("amplitude", 1);
	test_shader.setUniform1f("freqTime", 10);
	test_shader.setUniform1f("freqSpace", 1);
	test_shader.setUniform1f("coordinateSpace", 4);
	test_shader.setUniformMatrix4f("modelViewMatrix", cam.getModelViewMatrix());
	test_shader.setUniformMatrix4f("modelMatrix", cam.getLocalTransformMatrix());
	test_shader.setUniformMatrix4f("viewMatrix", cam.getGlobalTransformMatrix());
	test_shader.setUniformMatrix4f("projectionMatrix", cam.getProjectionMatrix());
	test_shader.setUniformMatrix4f("modelViewProjectionMatrix", cam.getModelViewProjectionMatrix());
	
	entity_manager.drawGameObjects();
	//testShader.end();

	event_manager.drawTutorial();

	ofPopMatrix();	
	cam.end();

	if (game_controller.getActive() != nullptr && game_controller.getActive()->isSpring) {
		gui_manager.drawRequiredGUI(true);
	}
	else {
		gui_manager.drawRequiredGUI(false);
	}
}

void ofApp::keyPressed(int key)
{
	audio_manager.keyPressed(key);

	cam.keyPressed(key);
	fluid_manager.keyPressed(key);
	event_manager.keyPressed(key);
	scene_manager.keyPressed(key);
	gamemode_manager.keyPressed(key);
	fluid_manager.keyPressed(key);
	gui_manager.keyPressed(key);

	if (event_manager.isEventAllowed("keyPressed")) {
		entity_manager.keyPressed(key);
	}

	if (key == 'f') {
		ofToggleFullscreen();
	}
}

void ofApp::keyReleased(int key)
{
	audio_manager.keyPressed(key);
	cam.keyReleased(key);

	if (event_manager.isEventAllowed("keyReleased")) {
		entity_manager.keyReleased(key);
	}
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
	if (event_manager.isEventAllowed("mouseDragged", button)) {
		entity_manager.mouseDragged(x, y, button);
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	if (event_manager.isEventAllowed("mousePressed", button)) {
		entity_manager.mousePressed(x, y, button);
	}
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	cam.mouseScrolled(x, y, scrollX, scrollY);
}

void ofApp::mouseReleased(int x, int y, int button)
{
	if (event_manager.isEventAllowed("mouseReleased", button)) {
		entity_manager.mouseReleased(x, y, button);
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
	gui_manager.windowResized(w, h);
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}