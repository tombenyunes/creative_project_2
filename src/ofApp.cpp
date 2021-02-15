#include "ofApp.h"


float tuioXScaler = 1;
float tuioYScaler = 1;

void ofApp::setup()
{
	// ---> Core setup <--- //
	
	GameController = new Controller;
	gui_Controller = new guiController;

	GameObject* player = new Player;
	GameObjects->push_back(player);

	Events.showTutorial(false);	// <----- enable or disable tutorial
	Events.setup();

	
	// ---> Fluid Update <--- // 

	fluidSolver.setup(100, 100);
	fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);

	fluidCellsX = 150;

	drawFluid = true;
	drawParticles = true;

	drawParticleGUI = false;

	ofBackground(0);
	ofSetVerticalSync(true);
	
	gui.addSlider("fluidCellsX", fluidCellsX, 20, 400);
	gui.addButton("resizeFluid", resizeFluid);
	gui.addSlider("colorMult", colorMult, 0, 100);
	gui.addSlider("velocityMult", velocityMult, 0, 100);
	gui.addSlider("fs.viscocity", fluidSolver.viscocity, 0.0, 0.01);
	gui.addSlider("fs.colorDiffusion", fluidSolver.colorDiffusion, 0.0, 0.0003);
	gui.addSlider("fs.fadeSpeed", fluidSolver.fadeSpeed, 0.0, 0.1);
	gui.addSlider("fs.solverIterations", fluidSolver.solverIterations, 1, 50);
	gui.addSlider("fs.deltaT", fluidSolver.deltaT, 0.1, 5);
	gui.addComboBox("fd.drawMode", (int&)fluidDrawer.drawMode, msa::fluid::getDrawModeTitles());
	gui.addToggle("fs.doRGB", fluidSolver.doRGB);
	gui.addToggle("fs.doVorticityConfinement", fluidSolver.doVorticityConfinement);
	gui.addToggle("drawFluid", drawFluid);
	gui.addToggle("drawParticles", drawParticles);
	gui.addSlider("velDrawMult", fluidDrawer.velDrawMult, 0.0, 20);
	gui.addSlider("velDrawThreshold", fluidDrawer.velDrawThreshold, 0.0, 1);
	gui.addSlider("brightness", fluidDrawer.brightness, 0.0, 2);
	gui.addToggle("useAdditiveBlending", fluidDrawer.useAdditiveBlending);

	gui.addToggle("fs.wrapX", fluidSolver.wrap_x);
	gui.addToggle("fs.wrapY", fluidSolver.wrap_y);
	gui.addSlider("tuioXScaler", tuioXScaler, 0, 2);
	gui.addSlider("tuioYScaler", tuioYScaler, 0, 2);

	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
	gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(false);
	gui.show();
	
	

	windowResized(ofGetWidth(), ofGetHeight());		// force this at start (cause I don't think it is called)
	resizeFluid = true;

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
			myCam.update(WORLD_WIDTH, WORLD_HEIGHT, ofVec2f((*GameObjects)[i]->pos.x + WORLD_WIDTH / 2, (*GameObjects)[i]->pos.y + WORLD_HEIGHT / 2));
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
		(*GameObjects)[i]->root_update(GameObjects, GameController, gui_Controller, &fluidSolver, &particleSystem, &myCam);
	}

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


	// ---> Fluid Update <--- // 

	if (resizeFluid) {
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}

	fluidSolver.update();
}

void ofApp::draw()
{
	//ofEnableLighting();
	//keyLight.enable();
	myCam.begin();

	if (drawFluid) {
		ofBackground(0);
		ofClear(0);
		glColor3f(1, 1, 1);
		ofDrawBox(ofGetWidth()/2-100, ofGetHeight()/2-100, -100, 100);
		fluidDrawer.draw(0, 0, WORLD_WIDTH, WORLD_HEIGHT);
	}
	if (drawParticles) {
		particleSystem.updateAndDraw(fluidSolver, ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), drawFluid/*, p*/);
	}

	ofSetCircleResolution(176); // must be a high resolution as the radius is flexible

	ofPushMatrix();

	ofTranslate(WORLD_WIDTH / 2, WORLD_HEIGHT / 2);

	for (int i = 0; i < GameObjects->size(); i++) (*GameObjects)[i]->root_draw();
	Events.draw();

	ofPopMatrix();
	
	myCam.end();
	//keyLight.disable();

	drawRequiredGUI();
}

void ofApp::drawRequiredGUI() {
	if (GameController->getGUIVisible() || Events.playerGUIVisible) {
		gui_Controller->player_gui.draw();
		if (GameController->getActive() != nullptr) {
			if (GameController->getActive()->isSpring) {	// if an object is a spring then it has multiple gui windows to draw
				gui_Controller->multi_selection_gui_anchor.draw();
				gui_Controller->multi_selection_gui_node1.draw();
				gui_Controller->multi_selection_gui_node2.draw();
				gui_Controller->multi_selection_gui_spring.draw();
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
	if (drawParticleGUI) {
		gui.show();
	}
	else {
		gui.hide();
	}
	gui.draw();
}

void ofApp::createNode()
{
	if (GameController->getNewNodeType() == 0) {
		cout << "Mass created" << endl;
		GameObject* object = new Object(ofVec2f(GameController->getWorldMousePos(&myCam).x, GameController->getWorldMousePos(&myCam).y), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND), GameController);
		GameObjects->push_back(object);
	}
	else if (GameController->getNewNodeType() == 1) {
		cout << "Spring created" << endl;
		GameObject* spring = new Springs(ofVec2f(GameController->getWorldMousePos(&myCam).x, GameController->getWorldMousePos(&myCam).y), ofRandom(25, 50), ofRandom(25, 75), ofRandom(25, 50), ofRandom(25, 75), 2, 2, 22, GameController);
		GameObjects->push_back(spring);
	}
}



void ofApp::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce, int count) {
	float speed = vel.x * vel.x + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
	if (speed > 0) {
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);

		int index = fluidSolver.getIndexForPos(pos);

		if (addColor) {
			//			Color drawColor(CM_HSV, (getElapsedFrames() % 360) / 360.0f, 1, 1);
			ofColor drawColor;
			drawColor.setHsb((ofGetFrameNum() % 255), 255, 255);

			fluidSolver.addColorAtIndex(index, drawColor * colorMult);

			if (drawParticles)
				particleSystem.addParticles(pos * ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), count);
		}

		if (addForce)
			fluidSolver.addForceAtIndex(index, vel * velocityMult);

	}
}



void ofApp::keyPressed(int key)
{
	myCam.keyPressed(key);
	
	Events.keyPressed(key);
	
	if ((Events.fullInput) || (Events.canKeypress)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			(*GameObjects)[i]->root_keyPressed(key);
		}
		if (key == 'c') {
			createNode();
		}
	}
	if (key == ' ') {
		for (int i = 0; i < 1000; i++) {
			ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
			//ofVec2f pos = ofVec2f(0.5, 0.5);
			ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
			//ofVec2f vel = ofVec2f(0.5, 0.5);
			addToFluid(pos, vel, true, true);
		}
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
	else if (key == 'z') {
		// toggle between following player and world view
		myCam.toggleZoomMode();
	}

	else if (key == '1') {
		// load scene 1
		msa::fluid::Solver* drawer = &fluidSolver;
		msa::fluid::DrawerGl* solver = &fluidDrawer;
		Scene_Manager.loadScene("Scene1", GameObjects, GameController, drawer, solver);
		
		for (int i = 0; i < 500; i++) {
			ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
			ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
			addToFluid(pos, vel, true, true);
		}
	}
	else if (key == '2') {
		// load scene 2
		msa::fluid::Solver* drawer = &fluidSolver;
		msa::fluid::DrawerGl* solver = &fluidDrawer;
		Scene_Manager.loadScene("Scene2", GameObjects, GameController, drawer, solver);
		
		for (int i = 0; i < 500; i++) {
			ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
			ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
			addToFluid(pos, vel, true, true);
		}
	}
	else if (key == '3') {
		// load scene 2
		msa::fluid::Solver* drawer = &fluidSolver;
		msa::fluid::DrawerGl* solver = &fluidDrawer;
		Scene_Manager.loadScene("Scene3", GameObjects, GameController, drawer, solver);

		for (int i = 0; i < 500; i++) {
			ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
			ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
			addToFluid(pos, vel, true, true);
		}
	}
	else if (key == '4') {
		// load scene 2
		msa::fluid::Solver* drawer = &fluidSolver;
		msa::fluid::DrawerGl* solver = &fluidDrawer;
		Scene_Manager.loadScene("Scene4", GameObjects, GameController, drawer, solver);

		for (int i = 0; i < 500; i++) {
			ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
			ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
			addToFluid(pos, vel, true, true);
		}
	}
	else if (key == '9') {
		// load saved scene
		msa::fluid::Solver* drawer = &fluidSolver;
		msa::fluid::DrawerGl* solver = &fluidDrawer;
		Scene_Manager.loadScene("newScene", GameObjects, GameController, drawer, solver);

		for (int i = 0; i < 500; i++) {
			ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
			ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
			addToFluid(pos, vel, true, true);
		}
	}
	else if (key == '0') {
		// save scene
		Scene_Manager.saveScene(GameObjects, (int&)fluidDrawer.drawMode, "newScene");
	}
}

void ofApp::keyReleased(int key)
{
	myCam.keyReleased(key);

	if ((Events.fullInput) || (Events.canKeypress)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			(*GameObjects)[i]->root_keyReleased(key);
		}
	}	
}

void ofApp::mouseMoved(int x, int y )
{

}
 
void ofApp::mouseDragged(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canDrag)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			//float nx = x;
			//float ny = y;
			//nx *= 1080;
			//ny *= 1080;
			//nx /= 1080 / cam.getScale().x;
			//ny /= 1080 / cam.getScale().y;

			//ofVec3f localView = ofVec3f(nx - ((ofGetWidth() * cam.getScale().x) / 2), ny - ((ofGetHeight() * cam.getScale().y) / 2), 0);
			ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), myCam.getPosition().z);
			ofVec3f worldView = myCam.screenToWorld(localView);			

			(*GameObjects)[i]->mouseDragged(worldView.x, worldView.y, button);
		}
	}
}

void ofApp::mousePressed(int x, int y, int button)
{
	if ((Events.fullInput) || (button == 0 && Events.canLMB) || (button == 2 && Events.canSelect)) {
		for (int i = 0; i < GameObjects->size(); i++) {
			//float nx = x;
			//float ny = y;
			//nx *= 1080;
			//ny *= 1080;
			//nx /= 1080 / cam.getScale().x;
			//ny /= 1080 / cam.getScale().y;
			//cout << x << " " << y << endl;
			//cout << ((ofGetWidth() * cam.getScale().x) / 2) << endl;					

			//ofVec3f localView = ofVec3f(nx - ((ofGetWidth() * cam.getScale().x) / 2), ny - ((ofGetHeight() * cam.getScale().y) / 2), 0);
			ofVec3f localView = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), myCam.getPosition().z);
			ofVec3f worldView = myCam.screenToWorld(localView);

			//worldView.x += ofGetWidth() / 2;
			//worldView.y += ofGetHeight() / 2;

			//cout << "localView: " << localView << endl;
			//cout << "worldView: " << worldView << endl;

			(*GameObjects)[i]->mousePressed(worldView.x, worldView.y, button);
		}
	}
}

void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY)
{
	myCam.mouseScrolled(x, y, scrollX, scrollY);

	if ((Events.fullInput) || (Events.canKeypress)) { // use the scroll wheel to set the type of object you create
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