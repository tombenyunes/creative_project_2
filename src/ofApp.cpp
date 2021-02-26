#include "ofApp.h"

void ofApp::setup()
{
	// ---> Core setup <--- //

	GameController = new Controller;
	gui_Controller = new guiController;

	GameObject* player = new Player;
	player->init(GameObjects, GameController, gui_Controller, &cam, &Fluid_Manager);
	GameObjects->push_back(player);

	Events.showTutorial(false);	// <----- enable or disable tutorial
	Events.setup();

	Scene_Manager.init(GameObjects, GameController, gui_Controller, &cam, &Fluid_Manager);

	ofBackground(0);
	ofSetVerticalSync(true);

	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cause I don't think it is called)

	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);

	keyLight.setDiffuseColor(ofFloatColor(1.0, 1.0, 1.0));
	keyLight.setSpecularColor(keyLight.getDiffuseColor());
	keyLight.setPosition(vec3(120.0, 100.0, 120.0));
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
	gui_Controller->update(GameController);
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
}

void ofApp::draw()
{
	//ofEnableLighting();
	//keyLight.enable();
	cam.begin();

	Fluid_Manager.renderFluid();
	Fluid_Manager.renderParticles();

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
		gui_Controller->player_gui.draw();
		if (GameController->getActive() != nullptr) {
			if (GameController->getActive()->isSpring) {	// if an object is a spring then it has multiple gui windows to draw
				gui_Controller->multi_selection_gui_spring.draw();
				if (gui_Controller->multiNodeSelected == true) {
					gui_Controller->multi_selection_gui_node.draw();
				}
			}
			else {
				gui_Controller->selected_gui.draw();
			}
		}
	}
	if (GameController->getGUIVisible()) {
		gui_Controller->world_gui.draw();
		gui_Controller->create_node_gui.draw();
	}

	Fluid_Manager.drawGUI(drawParticleGUI);
}

void ofApp::createNode()
{
	if (GameController->getNewNodeType() == 0) {
		cout << "Mass created" << endl;
		GameObject* object = new Object(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(GameObjects, GameController, gui_Controller, &cam, &Fluid_Manager);
		GameObjects->push_back(object);
	}
	else if (GameController->getNewNodeType() == 1) {
		cout << "Spring created" << endl;
		GameObject* spring = new Springs(ofVec2f(GameController->getWorldMousePos().x, GameController->getWorldMousePos().y), ofRandom(25, 50), ofRandom(25, 75), ofRandom(25, 50), ofRandom(25, 75), 2, 2, 22);
		spring->init(GameObjects, GameController, gui_Controller, &cam, &Fluid_Manager);
		GameObjects->push_back(spring);
	}
}

void ofApp::keyPressed(int key)
{
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
}

void ofApp::keyReleased(int key)
{
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

	if ((Events.fullInput) || (Events.canKeypress)) {
		if (scrollY == 1) {
			if (GameController->getNewNodeType() < 1) {
				GameController->setNewNodeType(GameController->getNewNodeType() + 1);
			}
			else {
				GameController->setNewNodeType(0);
			}
		}
		else if (scrollY == -1) {
			if (GameController->getNewNodeType() > 0) {
				GameController->setNewNodeType(GameController->getNewNodeType() - 1);
			}
			else {
				GameController->setNewNodeType(1);
			}
		}
	}
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
	gui_Controller->windowResized(w, h);
}

void ofApp::gotMessage(ofMessage msg)
{
}

void ofApp::dragEvent(ofDragInfo dragInfo)
{
}