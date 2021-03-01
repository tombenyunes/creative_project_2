#include "FluidManager.h"

FluidManager::FluidManager()
{
	fluidSolver.setup(100, 100);
	fluidSolver.enableRGB(true).setFadeSpeed(0.002).setDeltaT(0.5).setVisc(0.00015).setColorDiffusion(0);
	fluidDrawer.setup(&fluidSolver);

	fluidCellsX = 150;

	drawFluid = true;
	drawParticles = true;

	drawParticleGUI = false;


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

	resizeFluid = true;
}

void FluidManager::update()
{
	if (resizeFluid) {
		fluidSolver.setSize(fluidCellsX, fluidCellsX / msa::getWindowAspectRatio());
		fluidDrawer.setup(&fluidSolver);
		resizeFluid = false;
	}

	fluidSolver.update();

	//randomForces();
}

void FluidManager::renderFluid()
{
	if (drawFluid) {
		ofBackground(0);
		ofClear(0);
		glColor3f(1, 1, 1);
		ofDrawBox(ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 100, -100, 100);
		fluidDrawer.draw(0, 0, WORLD_WIDTH, WORLD_HEIGHT);
	}
}

void FluidManager::renderParticles()
{
	if (drawParticles) {
		particleSystem.updateAndDraw(fluidSolver, ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), drawFluid/*, p*/);
	}
}

void FluidManager::drawGUI(bool enable)
{
	if (enable) {
		gui.show();
		gui.draw();
	}
	else {
		gui.hide();
	}
}

void FluidManager::addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce, int count)
{
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

void FluidManager::explosion(int count)
{
	for (int i = 0; i < count; i++) {
		ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
		ofVec2f vel = ofVec2f(ofRandom(-0.01, 0.01), ofRandom(-0.01, 0.01));
		addToFluid(pos, vel, true, true);
	}
}

void FluidManager::randomForces()
{
	if (ofGetFrameNum() % (int)ofRandom(25, 100) == 0) {
		ofVec2f pos = ofVec2f(ofRandom(0.25, 0.75), ofRandom(0.25, 0.75));
		//for (int i = 0; i < 100; i++) {
		//	//ofVec2f vel = ofVec2f(ofRandom(-0.001, 0.001), ofRandom(-0.001, 0.001));
		//	ofVec2f vel = ofVec2f(ofMap(i, 0, 99, -0.001, 0.001), ofMap(i, 0, 99, -0.001, 0.001));
		//	addToFluid(pos, vel, true, true, 1);
		//}
		//ofVec2f vel = ofVec2f(ofRandom(-0.001, 0.001), ofRandom(-0.001, 0.001));
		//addToFluid(pos + 0.01, vel, true, true, 1);
		//cout << "placed" << endl;
		
		float pi = 3.14159;
		float radius = 0.01;
		for (double angle = 0; angle <= 2 * pi; angle += 0.1) {
			//ofVec2f vel = ofVec2f(ofRandom(-0.001, 0.001), ofRandom(-0.001, 0.001));
			//ofVec2f vel = ofVec2f(-0.0001, 0.0001);
			ofVec2f vel = ofVec2f(ofMap(radius * cos(angle), -0.01, 0.01, -0.001, 0.001), ofMap(radius * sin(angle), -0.01, 0.01, -0.001, 0.001));
			//cout << vel << endl;
			addToFluid(ofVec2f(pos.x + radius * cos(angle), pos.y + radius * sin(angle)), vel, false, true);
		}
	}
}

void FluidManager::resetFluid()
{
	fluidSolver.reset();
}

msa::fluid::Solver* FluidManager::getSolver()
{
	return &fluidSolver;
}

msa::fluid::DrawerGl* FluidManager::getDrawer()
{
	return &fluidDrawer;
}

ParticleSystem* FluidManager::getParticleSystem()
{
	return &particleSystem;
}

void FluidManager::keyPressed(int key)
{
	//if (key == 57344) { // f1
	//	(drawParticleGUI) ? drawParticleGUI = false : drawParticleGUI = true;
	//}
	//else if (key == 57345) { // f2
	//	//(GameController->getGUIVisible()) ? GameController->setGUIVisible(false) : GameController->setGUIVisible(true), drawParticleGUI = false;
	//}
}