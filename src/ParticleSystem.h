#pragma once

#include "Particle.h"

//#define MAX_PARTICLES		12000
//#define MAX_PARTICLES		24000
//#define MAX_PARTICLES		36000
#define MAX_PARTICLES		48000
//#define MAX_PARTICLES		96000

class ParticleSystem
{
public:

	ParticleSystem();

	void update_and_draw(const msa::fluid::Solver& a_solver, const ofVec2f window_size, const bool drawing_fluid, ofVec2f player_pos);
	void add_particles(const ofVec2f& pos, int count);
	void add_particle(const ofVec2f& pos);

private:

	float pos_array_[MAX_PARTICLES * 2 * 2];
	float col_array_[MAX_PARTICLES * 3 * 2];
	int cur_index_;

	Particle particles_[MAX_PARTICLES];
};
