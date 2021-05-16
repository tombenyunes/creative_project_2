#include "FluidManager.h"

FluidManager::FluidManager(): fluid_cells_x_(150),
                              resize_fluid_(false),
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
}

void FluidManager::init(GUIManager* gui_manager)
{
	gui_manager_ = gui_manager;

	fluid_solver_.setup(100, 100);
	fluid_solver_.enableRGB(true).setFadeSpeed(0.002f).setDeltaT(0.5f).setVisc(0.00015f).setColorDiffusion(0);
	fluid_drawer_.setup(&fluid_solver_);

	update_from_gui();

	fluid_blur_.setup(WORLD_WIDTH, WORLD_HEIGHT, 32, 0.2f, 2);
}

void FluidManager::update()
{
	update_from_gui();
	
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

void FluidManager::update_from_gui()
{
	//fluid_cells_x_ = gui_manager_->gui_fluid_cells;
	//resize_fluid_ = gui_manager_->gui_fluid_resize_fluid;
	//color_mult_ = gui_manager_->gui_fluid_color_mult;
	velocity_mult_ = gui_manager_->gui_fluid_velocity_mult;
	fluid_solver_.viscocity = gui_manager_->gui_fluid_viscocity;
	//fluid_solver_.colorDiffusion = gui_manager_->gui_fluid_color_diffusion;
	//fluid_solver_.fadeSpeed = gui_manager_->gui_fluid_fade_speed;
	//fluid_solver_.solverIterations = gui_manager_->gui_fluid_solver_iterations;
	fluid_solver_.deltaT = gui_manager_->gui_fluid_delta_t;
	reinterpret_cast<int&>(fluid_drawer_.drawMode) = gui_manager_->gui_fluid_draw_mode;
	//fluid_solver_.doRGB = gui_manager_->gui_fluid_do_rgb;
	fluid_solver_.doVorticityConfinement = gui_manager_->gui_fluid_do_vorticity_confinement;
	//draw_fluid_ = gui_manager_->gui_fluid_draw_fluid;
	//draw_particles_ = gui_manager_->gui_fluid_draw_particles;
	//fluid_drawer_.velDrawMult = gui_manager_->gui_fluid_vel_draw_mult;
	//fluid_drawer_.velDrawThreshold = gui_manager_->gui_fluid_vel_draw_threshold;
	fluid_drawer_.brightness = gui_manager_->gui_fluid_brightness;
	//fluid_drawer_.useAdditiveBlending = gui_manager_->gui_fluid_use_additive_blending;
	fluid_solver_.wrap_x = fluid_solver_.wrap_y = gui_manager_->gui_fluid_wrap_edges;
	//tuio_x_scaler_ = gui_manager_->gui_fluid_tuio_x_scaler;
	//tuio_y_scaler_ = gui_manager_->gui_fluid_tuio_y_scaler;
}

void FluidManager::draw(GameObject* player)
{
	render_fluid();
	render_particles(player);
}

void FluidManager::render_fluid()
{
	if (gui_manager_->gui_fluid_calculate_fluid)
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
}

void FluidManager::render_particles(GameObject* player)
{
	if (gui_manager_->gui_fluid_calculate_particles)
	{
		if (draw_particles_)
		{
			particle_system_.update_and_draw(fluid_solver_, ofVec2f(WORLD_WIDTH, WORLD_HEIGHT), draw_fluid_, player);
		}
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
