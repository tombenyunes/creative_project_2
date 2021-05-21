#include "Particle.h"

static const float MOMENTUM = 0.5f;
static const float FLUID_FORCE = 0.6f;

void Particle::init(const float x, const float y)
{
	pos_ = ofVec2f(x, y);
	vel_ = ofVec2f(0, 0);
	alpha = msa::Rand::randFloat(0.3f, 1);
	mass_ = msa::Rand::randFloat(0.1f, 1);
}

void Particle::update(const msa::fluid::Solver& solver, const ofVec2f& window_size, const ofVec2f& inv_window_size, GameObject* player)
{
	// only update if particle is visible
	if (alpha == 0)
		return;

	vel_ = solver.getVelocityAtPos(pos_ * inv_window_size) * (mass_ * FLUID_FORCE) * window_size + vel_ * MOMENTUM;
	pos_ += vel_;

	// bounce of edges
	if (pos_.x < 0)
	{
		pos_.x = 0;
		vel_.set(0);
	}
	else if (pos_.x > window_size.x)
	{
		pos_.x = window_size.x;
		vel_.set(0);
	}
	else if (pos_.y < 0)
	{
		pos_.y = 0;
		vel_.set(0);
	}
	else if (pos_.y > window_size.y)
	{
		pos_.y = window_size.y;
		vel_.set(0);
	}
	
	if (alpha < 0.01f)
		alpha = 0;
}

void Particle::update_vertex_arrays(const bool drawing_fluid, const ofVec2f& inv_window_size, const int i, float* pos_buffer, float* col_buffer, GameObject* player)
{

	{
		if (alpha == 0)
			return;
		
		int vi = i * 4;
		pos_buffer[vi++] = pos_.x - max(vel_.x, vel_.x + 0.25f);		// <--- particles will not get smaller than a certain size (0.25f)
		pos_buffer[vi++] = pos_.y - max(vel_.y, vel_.y + 0.25f);
		pos_buffer[vi++] = pos_.x;
		pos_buffer[vi++] = pos_.y;

		int ci = i * 6;
		if (drawing_fluid)
		{
			// if drawing fluid, draw lines as black & white
			col_buffer[ci++] = alpha;
			col_buffer[ci++] = alpha;
			col_buffer[ci++] = alpha;
			col_buffer[ci++] = alpha;
			col_buffer[ci++] = alpha;
			col_buffer[ci++] = alpha;
		}
		else
		{
			// otherwise, use color
			const float vx_norm = vel_.x * inv_window_size.x;
			const float vy_norm = vel_.y * inv_window_size.y;
			float v2 = vx_norm * vx_norm + vy_norm * vy_norm;
			#define VMAX 0.013f
			if (v2 > VMAX * VMAX) v2 = VMAX * VMAX;
			float sat_inc = mass_ > 0.5 ? mass_ * mass_ * mass_ : 0;
			sat_inc *= sat_inc * sat_inc * sat_inc;
			ofColor color;
			color.setHsb(0, v2 * 255.0f / (VMAX * VMAX) + sat_inc, ofLerp(0.5, 1, mass_) * alpha * 255.0f);

			col_buffer[ci++] = color.r;
			col_buffer[ci++] = color.g;
			col_buffer[ci++] = color.b;
			col_buffer[ci++] = color.r;
			col_buffer[ci++] = color.g;
			col_buffer[ci++] = color.b;
		}		
	}
}
