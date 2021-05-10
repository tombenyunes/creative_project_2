#pragma once

#include "MSACore.h"
#include "MSAFluidSolver.h"

class GameObject;

class Particle
{
public:
	
	void init(float x, float y);
	void update(const msa::fluid::Solver& solver, const ofVec2f& window_size, const ofVec2f& inv_window_size, GameObject* player);
	void update_vertex_arrays(bool drawing_fluid, const ofVec2f& inv_window_size, int i, float* pos_buffer, float* col_buffer, GameObject* player);
	
	float alpha{};

private:
	
	ofVec2f pos_;
	ofVec2f vel_;
	float mass_{};
	
};
