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
	event_manager.show_tutorial(false);
	event_manager.setup();

	audio_manager.setup(this);
	
	//scene_manager.loadScene("Scenes/StartingScene");
	scene_manager.load_procedural_scene();	
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels)
{
	//audio_manager.audioOut(output, bufferSize, nChannels);
}

void ofApp::update()
{
	entity_manager.update_game_objects();
	game_controller.update();
	gui_manager.update();
	event_manager.update();
	fluid_manager.update();
	audio_manager.update();
	scene_manager.update();
}

void ofApp::draw()
{
	//ofDisableDepthTest();
	cam.begin();
	
	fluid_manager.render_fluid();
	fluid_manager.render_particles();

	//audio_manager.draw(); // background animation effect

	ofPushMatrix();
	
	ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);
	entity_manager.draw_game_objects();
	event_manager.draw_tutorial();

	ofPopMatrix();
	
	cam.end();

	if (game_controller.get_active() != nullptr && game_controller.get_active()->get_type() == "Spring") {
		gui_manager.draw_required_gui(true);
	}
	else {
		gui_manager.draw_required_gui(false);
	}
}

void ofApp::keyPressed(int key)
{
	audio_manager.keyPressed(key);

	cam.key_pressed(key);
	fluid_manager.key_pressed(key);
	event_manager.key_pressed(key);
	scene_manager.key_pressed(key);
	gamemode_manager.key_pressed(key);
	fluid_manager.key_pressed(key);
	gui_manager.key_pressed(key);

	if (event_manager.is_event_allowed("key_pressed")) {
		entity_manager.key_pressed(key);
	}

	if (key == 'f') {
		ofToggleFullscreen();
	}
}

void ofApp::keyReleased(int key)
{
	audio_manager.keyPressed(key);
	cam.key_released(key);

	if (event_manager.is_event_allowed("key_released")) {
		entity_manager.key_released(key);
	}
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
	if (event_manager.is_event_allowed("mouse_dragged", button)) {
		entity_manager.mouse_dragged(x, y, button);
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	if (event_manager.is_event_allowed("mouse_pressed", button)) {
		entity_manager.mouse_pressed(x, y, button);
	}
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	if (event_manager.is_event_allowed("mouse_scrolled")) {
		cam.mouse_scrolled(x, y, scrollX, scrollY);
	}
}

void ofApp::mouseReleased(int x, int y, int button)
{
	if (event_manager.is_event_allowed("mouse_released", button)) {
		entity_manager.mouse_released(x, y, button);
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
	gui_manager.window_resized(w, h);
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}