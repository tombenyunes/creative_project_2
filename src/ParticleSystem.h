#pragma once

#include "Particle.h"

#define MAX_PARTICLES		12500 // 50000

class ParticleSystem
{
public:

	ParticleSystem();

	void update_and_draw(const msa::fluid::Solver& a_solver, ofVec2f window_size, bool drawing_fluid);
	void add_particles(const ofVec2f& pos, int count);
	void add_particle(const ofVec2f& pos);

private:

	float pos_array_[MAX_PARTICLES * 2 * 2];
	float col_array_[MAX_PARTICLES * 3 * 2];
	int cur_index_;

	Particle particles_[MAX_PARTICLES];
};
