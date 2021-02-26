#pragma once

#include "MSAFluid.h"
#include "ofxSimpleGuiToo.h"
#include "ParticleSystem.h"
#include "Controller.h"

class FluidManager {

public:

	FluidManager();
	void update();
	void renderFluid();
	void renderParticles();
	void drawGUI(bool enable);

	void addToFluid(ofVec2f pos, ofVec2f vel, bool addColor, bool addForce, int count = 10);
	void explosion(int count = 500);

	void resetFluid();

	msa::fluid::Solver* getSolver();
	msa::fluid::DrawerGl* getDrawer();
	ParticleSystem* getParticleSystem();

	void keyPressed(int key);

private:

	float colorMult;
	float velocityMult;
	int fluidCellsX;
	bool resizeFluid;
	bool drawFluid;
	bool drawParticles;

	float tuioXScaler = 1;
	float tuioYScaler = 1;

	bool drawParticleGUI;

	msa::fluid::Solver fluidSolver;
	msa::fluid::DrawerGl fluidDrawer;

	ParticleSystem particleSystem;

};