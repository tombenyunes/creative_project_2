/*
 *  ParticleSystem.cpp
 *  ofxMSAFluid Demo
 *
 *  Created by Mehmet Akten on 02/05/2009.
 *  Copyright 2009 MSA Visuals Ltd.. All rights reserved.
 *
 */

#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() {
	cur_index_ = 0;
}

void ParticleSystem::update_and_draw(const msa::fluid::Solver &a_solver, const ofVec2f window_size, const bool drawing_fluid, ofVec2f player_pos) {
	const ofVec2f inv_window_size(1.0f / window_size.x, 1.0f / window_size.y);

	glEnable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
    glBlendFunc(GL_ONE,GL_ONE);
//	glEnable(GL_LINE_SMOOTH);
    ofSetLineWidth(1);
	
	for(int i=0; i<MAX_PARTICLES; i++) {
		if(particles_[i].alpha > 0) {
			particles_[i].update(a_solver, window_size, inv_window_size, player_pos);
			particles_[i].update_vertex_arrays(drawing_fluid, inv_window_size, i, pos_array_, col_array_, player_pos);
		}
	}
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, pos_array_);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(3, GL_FLOAT, 0, col_array_);
	
	glDrawArrays(GL_LINES, 0, MAX_PARTICLES * 2);
	
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	
	glDisable(GL_BLEND);
}


void ParticleSystem::add_particles(const ofVec2f &pos, const int count){
	for(int i=0; i<count; i++)
		//add_particle(pos + msa::Rand::randVec2f() * 4);
		add_particle(pos);
}


void ParticleSystem::add_particle(const ofVec2f &pos) {
	particles_[cur_index_].init(pos.x, pos.y);
	cur_index_++;
	if(cur_index_ >= MAX_PARTICLES) cur_index_ = 0;
}
