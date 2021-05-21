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
}

void FluidManager::update_from_gui()
{
	velocity_mult_ = gui_manager_->gui_fluid_velocity_mult;
	fluid_solver_.viscocity = gui_manager_->gui_fluid_viscocity;
	fluid_solver_.deltaT = gui_manager_->gui_fluid_delta_t;
	reinterpret_cast<int&>(fluid_drawer_.drawMode) = gui_manager_->gui_fluid_draw_mode;
	fluid_solver_.doVorticityConfinement = gui_manager_->gui_fluid_do_vorticity_confinement;
	fluid_drawer_.brightness = gui_manager_->gui_fluid_brightness;
	fluid_solver_.wrap_x = fluid_solver_.wrap_y = gui_manager_->gui_fluid_wrap_edges;
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
	const float speed = vel.x * vel.x + vel.y * vel.y * msa::getWindowAspectRatio() * msa::getWindowAspectRatio();    // balance the x and y components of speed with the screen aspect ratio
	if (speed > 0)
	{
		pos.x = ofClamp(pos.x, 0.0f, 1.0f);
		pos.y = ofClamp(pos.y, 0.0f, 1.0f);

		const int index = fluid_solver_.getIndexForPos(pos);

		if (add_color)
		{
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
