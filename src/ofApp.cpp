#include "ofApp.h"

void ofApp::setup()
{
	// ---> Core setup <--- //

	ofSetWindowPosition(3849, 649);
	ofSetWindowTitle("iota");	

	GameController = new Controller;
	GUI_Manager = new GUIManager;

	GameObject* player = new Player;
	player->init(GameObjects, GameController, GUI_Manager, &cam, &Fluid_Manager);
	GameObjects->push_back(player);

	Events.showTutorial(false);	// <----- enable or disable tutorial
	Events.setup();

	Scene_Manager.init(GameObjects, GameController, GUI_Manager, &cam, &Fluid_Manager);

	ofBackground(0);
	ofSetVerticalSync(true);

	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cause I don't think it is called)

	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);

	keyLight.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	keyLight.setSpecularColor(keyLight.getDiffuseColor());
	keyLight.setPosition(vec3(120.0, 100.0, 120.0));

	blur.setup(WORLD_WIDTH, WORLD_HEIGHT, 10, .2, 2);
	blur2.setup(ofGetWidth(), ofGetHeight(), 10, .2, 2);

	//blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
	//blur2.setScale(0);
	//blur2.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));

	/*for (int i = 0; i < 100; i++) {
		GameObject* object = new Mass(ofVec2f(ofRandom(-WORLD_WIDTH / 2, WORLD_WIDTH / 2), ofRandom(-WORLD_HEIGHT / 2, WORLD_HEIGHT / 2)), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(GameObjects, GameController, GUI_Manager, &cam, &Fluid_Manager);
		GameObjects->push_back(object);
	}*/


	Audio_Manager.setup(this);
}

void ofApp::audioOut(float* output, int bufferSize, int nChannels)
{
	Audio_Manager.audioOut(output, bufferSize, nChannels);
}

void ofApp::update()
{
	// erase objects that need to be deleted and free memory
	for (int i = 0; i < GameObjects->size(); i++) {
		if ((*GameObjects)[i]->isPlayer == true) {
			cam.update(WORLD_WIDTH, WORLD_HEIGHT, ofVec2f((*GameObjects)[i]->pos.x + WORLD_WIDTH / 2, (*GameObjects)[i]->pos.y + WORLD_HEIGHT / 2));
		}
		if ((*GameObjects)[i]->needs_to_be_deleted == true) {
			if ((*GameObjects)[i] == GameController->getActive()) {
				GameController->makeActive(nullptr);
			}
			delete (*GameObjects)[i];
			GameObjects->erase(GameObjects->begin() + i);
		}
	}
	// update all gameobjects
	for (int i = 0; i < GameObjects->size(); i++) {
		(*GameObjects)[i]->root_update();
	}

	GameController->update(&cam);
	GUI_Manager->update(GameController);
	Events.update(GameController, GameObjects);

	if (GameController->getDeleteAll()) {
		GameController->setDeleteAll(false);
		for (int i = 0; i < GameObjects->size(); i++) {
			if ((*GameObjects)[i]->isPlayer != true) {
				(*GameObjects)[i]->needs_to_be_deleted = true;
			}
		}
	}

	Fluid_Manager.update();

	//blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
	//blur2.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//blur2.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void ofApp::draw()
{
	//ofEnableLighting();
	//keyLight.enable();
	cam.begin();


	blur.begin();
	Fluid_Manager.renderFluid();
	blur.end();
	blur.draw();

	//blur2.begin();
	Fluid_Manager.renderParticles();
	//blur2.end();
	//blur2.draw();

	ofSetCircleResolution(176); // must be a high resolution as the radius is flexible

	ofPushMatrix();

	ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);
	for (int i = 0; i < GameObjects->size(); i++) (*GameObjects)[i]->root_draw();

	Events.draw();

	ofPopMatrix();	

	cam.end();
	//keyLight.disable();

	drawRequiredGUI();
}

void ofApp::drawRequiredGUI() {
	if (GameController->getGUIVisible() || Events.playerGUIVisible) {
		GUI_Manager->player_gui.draw();
		if (GameController->getActive() != nullptr) {
			if (GameController->getActive()->isSpring) {	// if an object is a spring then it has multiple gui windows to draw
				GUI_Manager->multi_selection_gui_spring.draw();
				if (GUI_Manager->multiNodeSelected == true) {
					GUI_Manager->multi_selection_gui_node.draw();
				}
			}
			else {
				GUI_Manager->selected_gui.draw();
			}
		}
	}
	if (GameController->getGUIVisible()) {
		GUI_Manager->world_gui.draw();
		GUI_Manager->create_node_gui.draw();
	}

	Fluid_Manager.drawGUI(drawParticleGUI);

	Audio_Manager.draw();
}

void ofApp::createNode()
{
	if (GameController->getNewNodeType() == 0) {
		cout << "Mass created" << endl;
		GameObject* object = new Mass(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(GameObjects, GameController, GUI_Manager, &cam, &Fluid_Manager);
		GameObjects->push_back(object);
	}
	else if (GameController->getNewNodeType() == 1) {
		cout << "Spring created" << endl;
		GameObject* spring = new Spring(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y), ofRandom(25, 50), ofRandom(25, 75), ofRandom(25, 50), ofRandom(25, 75), 2, 2, 22);
		spring->init(GameObjects, GameController, GUI_Manager, &cam, &Fluid_Manager);
		GameObjects->push_back(spring);
	}
	else if (GameController->getNewNodeType() == 2) {
		cout << "Point created" << endl;
		GameObject* point = new Mass(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y), 10, 25);
		point->init(GameObjects, GameController, GUI_Manager, &cam, &Fluid_Manager);
		GameObjects->push_back(point);
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
		for (int i = 0; i < GameObjects->size(); i++) {
			(*GameObjects)[i]->root_keyPressed(key);
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
		(drawParticleGUI) ? drawParticleGUI = false : drawParticleGUI = true, GameController->setGUIVisible(false);
	}
	else if (key == 57345) { // f2
		(GameController->getGUIVisible()) ? GameController->setGUIVisible(false) : GameController->setGUIVisible(true), drawParticleGUI = false;
	}
	
	if ((Events.fullInput) || (Events.canKeypress)) {
		if (key == 57358) {
			if (GameController->getNewNodeType() < 2) {
				GameController->setNewNodeType(GameController->getNewNodeType() + 1); // next
			}
		}
		else if (key == 57356) {
			if (GameController->getNewNodeType() > 0) {
				GameController->setNewNodeType(GameController->getNewNodeType() - 1); // previous
			}
			else {
				GameController->setNewNodeType(0);
			}
		}
	}
}

void ofApp::keyReleased(int key)
{
	Audio_Manager.keyPressed(key);

	cam.keyReleased(key);

	if ((Events.fullInput) || (Events.canKeypress)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			(*GameObjects)[i]->root_keyReleased(key);
		}
	}
}

void ofApp::mouseMoved(int x, int y)
{
}

void ofApp::mouseDragged(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canDrag)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam.getPosition().z);
			ofVec3f worldView = cam.screenToWorld(localView);
			(*GameObjects)[i]->mouseDragged(worldView.x, worldView.y, button);
		}
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canSelect)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam.getPosition().z);
			ofVec3f worldView = cam.screenToWorld(localView);
			(*GameObjects)[i]->mousePressed(worldView.x, worldView.y, button);
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
		for (int i = 0; i < GameObjects->size(); i++) {
			(*GameObjects)[i]->mouseReleased(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), button);
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
	GUI_Manager->windowResized(w, h);
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}