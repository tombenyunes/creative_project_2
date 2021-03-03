#include "ofApp.h"

void ofApp::setup()
{
	// ---> Core setup <--- //

	ofSetWindowPosition(3849, 649);
	ofSetWindowTitle("iota");
	ofSetCircleResolution(176);
	ofBackground(0);
	ofSetVerticalSync(true);
	windowResized(ofGetWidth(), ofGetHeight()); // force this at start (cause I don't think it is called)
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);

	Entity_Manager.init(&GameController, &cam);
	Scene_Manager.init(Entity_Manager.getGameObjects(), &GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
	GUI_Manager.init(&GameController);

	Audio_Manager.setup(this);
	
	Events.init(&Entity_Manager);
	Events.showTutorial(false);
	Events.setup();


	GameObject* player = new Player;
	player->init(Entity_Manager.getGameObjects(), &GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
	Entity_Manager.addGameObject(player);

	for (int i = 0; i < 100; i++) {
		GameObject* object = new Mass(ofVec2f(ofRandom(-WORLD_WIDTH / 2, WORLD_WIDTH / 2), ofRandom(-WORLD_HEIGHT / 2, WORLD_HEIGHT / 2)), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(Entity_Manager.getGameObjects(), &GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
		Entity_Manager.addGameObject(object);
	}	

	blur.setup(WORLD_WIDTH, WORLD_HEIGHT, 32, .2, 2);
	//blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));	
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels)
{
	Audio_Manager.audioOut(output, bufferSize, nChannels);
}

void ofApp::update()
{	
	Entity_Manager.updateGameObjects();

	GameController.update(&cam);
	GUI_Manager.update();
	Events.update(&GameController, Entity_Manager.getGameObjects());

	Fluid_Manager.update();
	Audio_Manager.update();

	//blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void ofApp::draw()
{
	cam.begin();

	blur.begin();
	Fluid_Manager.renderFluid();
	blur.end();
	blur.draw();
	
	Fluid_Manager.renderParticles();	

	Audio_Manager.draw();

	ofPushMatrix();

	Entity_Manager.drawGameObjects(ofVec2f(WORLD_WIDTH / 2, WORLD_HEIGHT / 2));	

	Events.draw();

	ofPopMatrix();	
	cam.end();

	drawRequiredGUI();
}

void ofApp::drawRequiredGUI() {
	if (GameController.getGUIVisible() || Events.playerGUIVisible) {
		GUI_Manager.player_gui.draw();
		if (GameController.getActive() != nullptr) {
			if (GameController.getActive()->isSpring) {	// if an object is a spring then it has multiple gui windows to draw
				GUI_Manager.multi_selection_gui_spring.draw();
				if (GUI_Manager.multiNodeSelected == true) {
					GUI_Manager.multi_selection_gui_node.draw();
				}
			}
			else {
				GUI_Manager.selected_gui.draw();
			}
		}
	}
	if (GameController.getGUIVisible()) {
		GUI_Manager.world_gui.draw();
		GUI_Manager.create_node_gui.draw();
	}

	Fluid_Manager.drawGUI(drawParticleGUI);
	Audio_Manager.drawGUI(drawAudioGUI);
}

void ofApp::createNode()
{
	if (GameController.getNewNodeType() == 0) {
		cout << "Mass created" << endl;
		GameObject* object = new Mass(ofVec2f(GameController.getWorldMousePos().x, GameController.getWorldMousePos().y), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(Entity_Manager.getGameObjects(), &GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
		Entity_Manager.addGameObject(object);
	}
	else if (GameController.getNewNodeType() == 1) {
		cout << "Spring created" << endl;
		GameObject* spring = new Spring(ofVec2f(GameController.getWorldMousePos().x, GameController.getWorldMousePos().y), ofRandom(25, 50), ofRandom(25, 75), ofRandom(25, 50), ofRandom(25, 75), 2, 2, 22);
		spring->init(Entity_Manager.getGameObjects(), &GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
		Entity_Manager.addGameObject(spring);
	}
	else if (GameController.getNewNodeType() == 2) {
		cout << "Point created" << endl;
		GameObject* point = new Mass(ofVec2f(GameController.getWorldMousePos().x, GameController.getWorldMousePos().y), 10, 25);
		point->init(Entity_Manager.getGameObjects(), &GameController, &GUI_Manager, &cam, &Fluid_Manager, &Audio_Manager);
		Entity_Manager.addGameObject(point);
	}
}

void ofApp::keyPressed(int key)
{
	Audio_Manager.keyPressed(key);

	cam.keyPressed(key);
	Fluid_Manager.keyPressed(key);
	Events.keyPressed(key);
	Scene_Manager.keyPressed(key);

	if ((Events.fullInput) || (Events.canKeypress)) {
		for (int i = 0; i < Entity_Manager.getGameObjects()->size(); i++) {
			(*Entity_Manager.getGameObjects())[i]->root_keyPressed(key);
		}
		if (key == 'c') {
			createNode();
		}
	}
	if (key == ' ') {
		Fluid_Manager.explosion(12500);
	}
	else if (key == 'f') {
		ofToggleFullscreen();
	}
	else if (key == 57344) { // f1
		 if (GameController.getGUIVisible()) {
			 GameController.setGUIVisible(false);
		 }
		 else {
			 GameController.setGUIVisible(true);
			 drawParticleGUI = false;
			 drawAudioGUI = false;
		 }
	}
	else if (key == 57345) { // f2
		if (drawParticleGUI) {
			drawParticleGUI = false;
		}
		else {
			GameController.setGUIVisible(false);
			drawParticleGUI = true;
			drawAudioGUI = false;
		}
	}
	else if (key == 57346) { // f3
		if (drawAudioGUI) {
			drawAudioGUI = false;
		}
		else {
			GameController.setGUIVisible(false);
			drawParticleGUI = false;
			drawAudioGUI = true;
		}
	}
	
	if ((Events.fullInput) || (Events.canKeypress)) {
		if (key == 57358) {
			if (GameController.getNewNodeType() < 2) {
				GameController.setNewNodeType(GameController.getNewNodeType() + 1); // next
			}
		}
		else if (key == 57356) {
			if (GameController.getNewNodeType() > 0) {
				GameController.setNewNodeType(GameController.getNewNodeType() - 1); // previous
			}
			else {
				GameController.setNewNodeType(0);
			}
		}
	}
}

void ofApp::keyReleased(int key)
{
	Audio_Manager.keyPressed(key);

	cam.keyReleased(key);

	if ((Events.fullInput) || (Events.canKeypress)) {
		for (int i = 0; i < Entity_Manager.getGameObjects()->size(); i++) {
			(*Entity_Manager.getGameObjects())[i]->root_keyReleased(key);
		}
	}
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canDrag)) {
		for (int i = 0; i < Entity_Manager.getGameObjects()->size(); i++) {
			ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam.getPosition().z);
			ofVec3f worldView = cam.screenToWorld(localView);
			(*Entity_Manager.getGameObjects())[i]->mouseDragged(worldView.x, worldView.y, button);
		}
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canSelect)) {
		for (int i = 0; i < Entity_Manager.getGameObjects()->size(); i++) {
			ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam.getPosition().z);
			ofVec3f worldView = cam.screenToWorld(localView);
			(*Entity_Manager.getGameObjects())[i]->mousePressed(worldView.x, worldView.y, button);
		}
	}
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	cam.mouseScrolled(x, y, scrollX, scrollY);
}

void ofApp::mouseReleased(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canSelect)) {
		for (int i = 0; i < Entity_Manager.getGameObjects()->size(); i++) {
			(*Entity_Manager.getGameObjects())[i]->mouseReleased(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), button);
		}
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