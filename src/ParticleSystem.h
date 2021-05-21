#pragma once

#include "Particle.h"

#define MAX_PARTICLES		48000

class ParticleSystem
{
public:

	ParticleSystem();

	void update_and_draw(const msa::fluid::Solver& a_solver, const ofVec2f window_size, const bool drawing_fluid, GameObject* player);
	void add_particles(const ofVec2f& pos, int count);
	void add_particle(const ofVec2f& pos);

private:

	float pos_array_[MAX_PARTICLES * 2 * 2];
	float col_array_[MAX_PARTICLES * 3 * 2];
	int cur_index_;

	Particle particles_[MAX_PARTICLES];

	ofVec2f pos{ ofRandom(0, 4000), ofRandom(0, 3000) };
	ofVec2f vel{};
};
