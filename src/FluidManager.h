#pragma once

#include "MSAFluid.h"
#include "ofxBlur.h"
#include "ofxSimpleGuiToo.h"
#include "GUIManager.h"
#include "Controller.h"
#include "ParticleSystem.h"

class FluidManager
{
public:

	FluidManager();

	void init(GUIManager* gui_manager);	

	void update();
	void update_from_gui();
	
	void draw(GameObject* player);
	void render_fluid();
	void render_particles(GameObject* player);

	void add_to_fluid(ofVec2f pos, ofVec2f vel, bool add_color, bool add_force, int count = 10);
	void explosion(int count = 500);
	void increment_brightness();

	void set_velocity_mult(const float vel) { velocity_mult_ = vel; }

	void reset_fluid();

	msa::fluid::Solver* get_solver();
	msa::fluid::DrawerGl* get_drawer();
	ParticleSystem* get_particle_system();

	void key_pressed(int key);

private:

	int fluid_cells_x_;
	bool resize_fluid_;
	float color_mult_;
	float velocity_mult_;
	bool draw_fluid_;
	bool draw_particles_;
	float tuio_x_scaler_;
	float tuio_y_scaler_;

	msa::fluid::Solver fluid_solver_;
	msa::fluid::DrawerGl fluid_drawer_;

	ParticleSystem particle_system_;

	ofxBlur fluid_blur_;

	bool do_increment_brightness_;
	float prev_brightness_;
	bool do_increment_delta_t_;
	float prev_delta_t_;
	bool do_increment_viscocity_;
	float prev_viscocity_;
	bool do_increment_velocity_;
	float prev_velocity_;

	GUIManager* gui_manager_;
	
};
