#include "Collectable.h"

Collectable::Collectable(const ofVec2f pos, const float mass, const float radius, const float emission_frequency, const float emission_force, const bool is_active)
	:	is_active_(is_active)												// controlls if the particle emission is enabled
	,	emission_frequency_(emission_frequency)								// frequency of particle emission
	,	emission_force_(emission_force)										// force of particle emission
	,	starting_radius_(get_radius())
	,	needs_to_pulse_radius_(false)
	,	alpha_(0)
	,	can_be_collected_(false)
	,	id_(get_cur_id())
{
	set_type("Collectable");
	set_position(pos);
	//set_color(ofColor(passive_color_));
	set_color(ofColor(passive_color_.r, passive_color_.g, passive_color_.b, 100));
	set_mass(mass);
	set_radius(radius);

	add_module("screenBounce");
	add_module("ellipseCollider");
	add_module("gravity");
	add_module("friction");
	add_module("mouseHover");

	pixel_buffer_before_drag_ = 2;
}



// Static vars

bool Collectable::first_point_ = true;
int Collectable::last_id_collected_ = -404;
int Collectable::cur_id_ = -1;

bool Collectable::first_point()
{
	if (Collectable::first_point_)
	{
		return true;
	}
	else
	{
		return false;
	}
}
int Collectable::get_cur_id()
{
	cur_id_++;
	return cur_id_;
}



void Collectable::update()
{
	if (is_active_)
	{
		random_forces();
		pulse_radius();
	}
	else
	{
		check_if_active();
	}
	
	update_forces();
	drag_nodes();
	update_gui();
	reset_forces();	
}

// collectables randomly emit 'shock waves' which in effect causes 'streams' of particles to form (this could help the player to locate collectables)
void Collectable::random_forces()
{
	if (ofGetFrameNum() % static_cast<int>(emission_frequency_) == 0)
	{
		ofVec2f mapped_pos;
		mapped_pos.x = ofMap(get_position().x, -HALF_WORLD_WIDTH, HALF_WORLD_WIDTH, 0, 1);
		mapped_pos.y = ofMap(get_position().y, -HALF_WORLD_HEIGHT, HALF_WORLD_HEIGHT, 0, 1);
		ofVec2f vel/* = ofVec2f(ofRandom(-0.001f, 0.1f), ofRandom(-0.001f, 0.1f))*/;
		/*vel.x = ofRandom(-0.001f, 0.1f);
		vel.y = ofRandom(-0.001f, 0.1f);*/


		// all collectables direct velocities somewhat towards the centre
		/*if (get_position().x > 0)
		{
			vel.x = ofRandom(-0.1f, 0.001f);
		}
		else
		{
			vel.x = ofRandom(-0.001f, 0.1f);
		}
		if (get_position().y > 0)
		{
			vel.y = ofRandom(-0.1f, 0.001f);
		}
		else
		{
			vel.y = ofRandom(-0.001f, 0.1f);
		}*/


		// randomly select a collectables and target all velocities towards that point's position
		vector<ofVec2f> point_positions;
		
		for (auto& game_object : *game_objects_)
		{
			if (game_object->get_type() == "Collectable")
			{
				point_positions.push_back(game_object->get_position());				
			}
		}

		for (int i = 0; i < point_positions.size(); i++)
		{
			if (point_positions[i] == get_position())
			{
				int i2;
				if (i + 1 > point_positions.size() - 1)
					i2 = 0;
				else
					i2 = i + 1;

				vel = (point_positions[i2] - point_positions[i]);
				//vel = (point_positions[0] - point_positions[i])/* / 1000*/;
				vel.normalize();
			}
		}

		fluid_manager_->add_to_fluid(mapped_pos, vel * emission_force_, true, true, 100);
		needs_to_pulse_radius_ = true;

		// if collectable is within screen bounds increment brightness
		/*for (auto& game_object : *game_objects_)
		{
			if (game_object->get_type() == "Player")
			{
				if (game_object->get_position().x < get_position().x + 1000 && game_object->get_position().x > get_position().x - 1000 &&
					game_object->get_position().y < get_position().y + 500 && game_object->get_position().y > get_position().y - 500)
				{
					//fluid_manager_->increment_brightness();
					player_within_bounds_ = true;
				}
				else
				{
					player_within_bounds_ = false;
				}
			}
		}*/

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

void Collectable::pulse_radius()
{
	if (needs_to_pulse_radius_)
	{
		set_radius(starting_radius_ * 2);
		draw_particle_burst();
		needs_to_pulse_radius_ = false;
	}
	else if (get_radius() > starting_radius_)
	{
		set_radius(get_radius() - ((((starting_radius_ * 2) - starting_radius_) / emission_frequency_)));
	}
}

void Collectable::check_if_active()
{
	int next_id;
	if (last_id_collected_ == 4)
		next_id = 0;
	else
		next_id = last_id_collected_ + 1;

	if (id_ == next_id)
	{
		can_be_collected_ = true;
	}
}



void Collectable::update_forces()
{
	add_forces(false);
}

void Collectable::drag_nodes()
{
	local_mouse_pos_before_drag_.set(cam_->get_local_mouse_pos());
	static ofVec2f mouse_pos_before_drag;
	
	if (mouse_drag_)
	{
		if (!started_dragging_)
		{
			started_dragging_ = true;		
		}
		
		const ofVec2f prev_pos2 = ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y)/* + mouse_offset_from_center_*/;

		ofVec2f new_pos;
		new_pos.x = ofLerp(pos_.x, prev_pos2.x, 0.1f);
		new_pos.y = ofLerp(pos_.y, prev_pos2.y, 0.1f);

		set_position(new_pos);
		set_velocity(ofVec2f(0));
		
		mouse_pos_before_drag = ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y);
	}
	else
	{
		if (started_dragging_ == true)
		{
			started_dragging_ = false;
			const ofVec2f mouse_speed = (ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y) - mouse_pos_before_drag) / 3;
			apply_force(accel_, mouse_speed, false);
		}
	}
}

void Collectable::update_gui()
{
	if (get_is_selected() == true)
	{
		if (gui_values_need_to_be_set_)
		{
			gui_manager_->update_collectable_values(pos_, vel_, accel_, mass_, infinite_mass_, starting_radius_, affected_by_gravity_, emission_frequency_, emission_force_, is_active_, id_);
			gui_values_need_to_be_set_ = false;
		}
		else
		{
			gui_manager_->update_collectable_values(pos_, vel_, accel_, gui_manager_->gui_collectable_mass, gui_manager_->gui_collectable_infinite_mass, gui_manager_->gui_collectable_radius, gui_manager_->gui_collectable_affected_by_gravity, gui_manager_->gui_collectable_emission_frequency, gui_manager_->gui_collectable_emission_force, gui_manager_->gui_collectable_is_active, id_);
			if (infinite_mass_)
			{
				mass_ = 999999999999.0f;
			}
			else
			{
				mass_ = gui_manager_->gui_collectable_mass;
			}
			starting_radius_ = gui_manager_->gui_collectable_radius;
			infinite_mass_ = gui_manager_->gui_collectable_infinite_mass;
			affected_by_gravity_ = gui_manager_->gui_collectable_affected_by_gravity;
			emission_frequency_ = gui_manager_->gui_collectable_emission_frequency;
			emission_force_ = gui_manager_->gui_collectable_emission_force;
			is_active_ = gui_manager_->gui_collectable_is_active;
		}
	}
}

void Collectable::reset_forces()
{
	accel_.set(0);
}

void Collectable::is_colliding(GameObject* other, ofVec2f node_pos)
{
	if (other->get_type() == "PullRange")
	{
		if (can_be_collected_ || Collectable::first_point())
		{
			if (Collectable::first_point())
			{
				first_point_ = false;				
			}
			
			is_active_ = true;
			alpha_ = 255;
			Collectable::last_id_collected_ = id_;
		}
	}
	
	/*if (other->get_type() == "PullRange")
	{
		const ofVec2f direction_to_player = other->get_position() - pos_;
		apply_force(accel_, direction_to_player, true, 2.75f);
		other->set_request_to_be_deleted(true);
	}
	if (other->get_type() == "Player")
	{
		fluid_manager_->increment_brightness();
		set_request_to_be_deleted_event("Collected");
		set_request_to_be_deleted(true);
	}*/
}

void Collectable::draw_particle_burst() const
{
	if (player_within_bounds_)
	{
		for (int i = 0; i < 1; i++)
		{
			//PlayerTrail* particle = nullptr;
			//particle = new PlayerTrail(get_position() + ofRandom(-get_radius() / 3, get_radius() / 3), ofVec2f(ofRandom(-1, 1), ofRandom(-1, 1)), ofRandom(1, 4), ofColor(255), 255);
			//particles_->push_back(particle);

			//GameObject* particle = new Mass(get_position() + ofVec2f(ofRandom(-get_radius() / 3, get_radius() / 3), ofRandom(-get_radius() / 3, get_radius() / 3)), 5, 5);
			//particle->init(game_objects_, game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
			//game_objects_->push_back(particle);
		}
	}
}


// ----- EVENT FUNCTIONS ----- //


void Collectable::mouse_pressed(const float x, const float y, const int button)
{
	if (button == 2 && mouse_over_)
	{
		if (get_is_selected() == false)
		{
			set_request_to_be_selected(true);
			
			gui_values_need_to_be_set_ = true;
		}
		else
		{
			// deselect object
			//set_request_to_be_deselected(true);
		}
	}
}

void Collectable::mouse_dragged(const float x, const float y, const int button)
{
	if (button == 2)
	{
		if (mouse_over_ && game_controller_->get_mouse_dragged() == false)
		{
			if (local_mouse_pos_before_drag_.distance(ofVec2f(ofGetMouseX() / 2 - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2)) > pixel_buffer_before_drag_)
			{
				// the node will only be moved by the mouse if it has been moved by more than 1 pixel - this prevents accidentally stopping something by selecting it
				mouse_drag_ = true;
				game_controller_->set_mouse_dragged(true);

				// select object, if unselected
				mouse_pressed(x, y, button);
			}
		}
	}
}

void Collectable::mouse_released(const float x, const float y, const int button)
{
	if (button == 2)
	{
		mouse_drag_ = false;
		game_controller_->set_mouse_dragged(false);
	}
}


// ----- RENDER LOOP ----- //


void Collectable::draw()
{
	ofPushStyle();	

	ofSetColor(ofColor(0, 0, 0, 50));
	//get_color();
	ofFill();
	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);

	
	static int r = get_radius();
	
	if (is_active_)
		alpha_ = ofMap(radius_, r, r * 4, 0, 255);
	else if (can_be_collected_)
		alpha_ = 255;
	
	ofSetLineWidth(ofMap(radius_, r, r * 2, 0.1f, 2.0f));
	ofNoFill();
	get_color();
	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);

	ofPopStyle();
}

void Collectable::get_color() const
{
	if (infinite_mass_)
	{
		ofSetColor(255, 0, 0);
	}
	else if ((get_is_selected() == true) || (mouse_over_ || mouse_drag_))
	{
		ofSetColor(selected_color_);
	}
	else
	{
		ofSetColor(ofColor(color_.r, color_.g, color_.b, alpha_));
	}
}
