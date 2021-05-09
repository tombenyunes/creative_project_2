#include "FluidManager.h"

FluidManager::FluidManager(): fluid_cells_x_(150),
                              resize_fluid_(true),
                              color_mult_(0),
                              velocity_mult_(0),
                              draw_fluid_(true),
                              draw_particles_(true),
                              tuio_x_scaler_(1),
                              tuio_y_scaler_(1),
                              do_increment_brightness_(false),
                              prev_brightness_(-1),
                              do_increment_delta_t_(false),
                              prev_delta_t_(-1),
                              do_increment_viscocity_(false),
                              prev_viscocity_(-1),
                              do_increment_velocity_(false),
                              prev_velocity_(-1)

{
	fluid_solver_.setup(100, 100);
	fluid_solver_.enableRGB(true).setFadeSpeed(0.002f).setDeltaT(0.5f).setVisc(0.00015f).setColorDiffusion(0);
	fluid_drawer_.setup(&fluid_solver_);

	gui.addSlider("fluidCellsX", fluid_cells_x_, 20, 400);
	gui.addButton("resizeFluid", resize_fluid_);
	gui.addSlider("colorMult", color_mult_, 0.0f, 100.0f);
	gui.addSlider("velocityMult", velocity_mult_, 0.0f, 100.0f);
	gui.addSlider("fs.viscocity", fluid_solver_.viscocity, 0.0f, 0.01f);
	gui.addSlider("fs.colorDiffusion", fluid_solver_.colorDiffusion, 0.0f, 0.0003f);
	gui.addSlider("fs.fadeSpeed", fluid_solver_.fadeSpeed, 0.0f, 0.1f);
	gui.addSlider("fs.solverIterations", fluid_solver_.solverIterations, 1, 50);
	gui.addSlider("fs.deltaT", fluid_solver_.deltaT, 0.1f, 5.0f);
	gui.addComboBox("fd.drawMode", reinterpret_cast<int&>(fluid_drawer_.drawMode), msa::fluid::getDrawModeTitles());
	gui.addToggle("fs.doRGB", fluid_solver_.doRGB);
	gui.addToggle("fs.doVorticityConfinement", fluid_solver_.doVorticityConfinement);
	gui.addToggle("drawFluid", draw_fluid_);
	gui.addToggle("drawParticles", draw_particles_);
	gui.addSlider("velDrawMult", fluid_drawer_.velDrawMult, 0.0, 20.0f);
	gui.addSlider("velDrawThreshold", fluid_drawer_.velDrawThreshold, 0.0, 1.0f);
	gui.addSlider("brightness", fluid_drawer_.brightness, 0.0, 2.0f);
	gui.addToggle("useAdditiveBlending", fluid_drawer_.useAdditiveBlending);

	gui.addToggle("fs.wrapX", fluid_solver_.wrap_x);
	gui.addToggle("fs.wrapY", fluid_solver_.wrap_y);
	gui.addSlider("tuioXScaler", tuio_x_scaler_, 0, 2.0f);
	gui.addSlider("tuioYScaler", tuio_y_scaler_, 0, 2.0f);

	gui.currentPage().setXMLName("ofxMSAFluidSettings.xml");
	gui.loadFromXML();
	gui.setDefaultKeys(true);
	gui.setAutoSave(false);
	gui.show();

	fluid_blur_.setup(WORLD_WIDTH, WORLD_HEIGHT, 32, 0.2f, 2);
	//fluid_blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//fluid_blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void FluidManager::update()
{
	if (resize_fluid_)
	{
		fluid_solver_.setSize(fluid_cells_x_, fluid_cells_x_ / msa::getWindowAspectRatio());
		fluid_drawer_.setup(&fluid_solver_);
		resize_fluid_ = false;
	}

	fluid_solver_.update();

	if (do_increment_brightness_)
	{
		if (fluid_drawer_.brightness > prev_brightness_)
		{
			fluid_drawer_.brightness -= 0.1f;
		}
		else
		{
			do_increment_brightness_ = false;
			fluid_drawer_.brightness = prev_brightness_;
		}
	}
	if (do_increment_delta_t_)
	{
		if (fluid_solver_.deltaT < prev_delta_t_)
		{
			fluid_solver_.deltaT += 0.005f;
		}
		else
		{
			do_increment_delta_t_ = false;
			fluid_solver_.deltaT = prev_delta_t_;
		}
	}
	if (do_increment_viscocity_)
	{
		if (fluid_solver_.viscocity < prev_viscocity_)
		{
			fluid_solver_.viscocity += 0.000001f;
		}
		else
		{
			do_increment_viscocity_ = false;
			fluid_solver_.viscocity = prev_viscocity_;
		}
	}
	if (do_increment_velocity_)
	{
		if (velocity_mult_ > prev_velocity_)
		{
			velocity_mult_ -= 1;
		}
		else
		{
			do_increment_velocity_ = false;
			velocity_mult_ = prev_velocity_;
		}
	}

	random_forces();

	//fluid_blur.setScale(ofMap(mouseX, 0, ofGetWidth(), 0, 10));
	//fluid_blur.setRotation(ofMap(mouseY, 0, ofGetHeight(), -PI, PI));
}

void FluidManager::render_fluid()
{
	fluid_blur_.begin();
	if (draw_fluid_)
	{
		ofBackground(0);
		ofClear(0);
		glColor3f(1, 1, 1);
		ofDrawBox(ofGetWidth() / 2 - 100, ofGetHeight() / 2 - 100, -100, 100);
		fluid_drawer_.draw(0, 0, WORLD_WIDTH, WORLD_HEIGHT);
	}
	fluid_blur_.end();
	fluid_blur_.draw(); // blur only applies to background fluid
}

void FluidManager::render_particles(const ofVec2f player_pos)
{
	if (draw_particles_)
	{
		particle_system_.update_and_draw(fluid_solver_, ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), draw_fluid_, player_pos);
	}
}

void FluidManager::draw_gui(const bool enable)
{
	if (enable)
	{
		gui.show();
		gui.draw();
	}
	else
	{
		gui.hide();
	}
}

void FluidManager::add_to_fluid(ofVec2f pos, const ofVec2f vel, const bool add_color, const bool add_force, const int count)
{
	//vel /= 2;
	const float speed = vel.x * vel.x + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
	if (speed > 0)
	{
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);

		const int index = fluid_solver_.getIndexForPos(pos);

		if (add_color)
		{
			//			Color drawColor(CM_HSV, (getElapsedFrames() % 360) / 360.0f, 1, 1);
			ofColor draw_color;
			draw_color.setHsb(ofGetFrameNum() % 255, 255, 255);

			fluid_solver_.addColorAtIndex(index, draw_color * color_mult_);

			if (draw_particles_)
				particle_system_.add_particles(pos * ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), count);
		}

		if (add_force)
			fluid_solver_.addForceAtIndex(index, vel * velocity_mult_);

	}
}

void FluidManager::explosion(const int count)
{
	for (int i = 0; i < count; i++)
	{
		const ofVec2f pos = ofVec2f(ofRandom(0, 1), ofRandom(0, 1));
		const ofVec2f vel = ofVec2f(ofRandom(-0.01f, 0.01f), ofRandom(-0.01f, 0.01f));
		add_to_fluid(pos, vel, true, true);
	}
}

void FluidManager::increment_brightness()
{
	if (!do_increment_brightness_) prev_brightness_ = fluid_drawer_.brightness;
	fluid_drawer_.brightness = 6;
	do_increment_brightness_ = true;

	//if (!doIncrementDeltaT) prevDeltaT = fluidSolver.deltaT;
	//fluidSolver.deltaT = 0;
	//doIncrementDeltaT = true;

	//if (!doIncrementViscocity) prevViscocity = fluidSolver.viscocity;
	//fluidSolver.viscocity = 0;
	//doIncrementViscocity = true;

	/*if (!do_increment_velocity_) prev_velocity_ = velocity_mult_;
	velocity_mult_ = 100;
	do_increment_velocity_ = true;*/
}

void FluidManager::random_forces()
{
	if (ofGetFrameNum() % static_cast<int>(ofRandom(25, 100)) == 0)
	{
		const ofVec2f pos = ofVec2f(ofRandom(0.25, 0.75), ofRandom(0.25, 0.75));
		/*for (int i = 0; i < 100; i++) {
			//ofVec2f vel = ofVec2f(ofRandom(-0.001, 0.001), ofRandom(-0.001, 0.001));
			ofVec2f vel = ofVec2f(ofMap(i, 0, 99, -0.001, 0.001), ofMap(i, 0, 99, -0.001, 0.001));
			addToFluid(pos, vel, true, true, 1);
		}*/
		/*const ofVec2f vel = ofVec2f(ofRandom(-0.001, 0.1), ofRandom(-0.001, 0.1));
		add_to_fluid(pos + 0.01, vel, false,  true, 1);*/

		/*const float pi = 3.14159f;
		const float radius = 0.01f;
		for (double angle = 0; angle <= 2 * pi; angle += 0.1)
		{
			//ofVec2f vel = ofVec2f(ofRandom(-0.001, 0.001), ofRandom(-0.001, 0.001));
			//ofVec2f vel = ofVec2f(-0.0001, 0.0001);
			const ofVec2f vel = ofVec2f(ofMap(radius * cos(angle), -0.01f, 0.01f, -0.001f, 0.001f),
			                            ofMap(radius * sin(angle), -0.01f, 0.01f, -0.001f, 0.001f));
			//cout << vel << endl;
			add_to_fluid(ofVec2f(pos.x + radius * cos(angle), pos.y + radius * sin(angle)), vel, false, true);
		}*/
	}
}

void FluidManager::reset_fluid()
{
	fluid_solver_.reset();
}

msa::fluid::Solver* FluidManager::get_solver()
{
	return &fluid_solver_;
}

msa::fluid::DrawerGl* FluidManager::get_drawer()
{
	return &fluid_drawer_;
}

ParticleSystem* FluidManager::get_particle_system()
{
	return &particle_system_;
}

void FluidManager::key_pressed(const int key)
{
	if (key == 'f')
	{
		explosion(12500);
	}
}
