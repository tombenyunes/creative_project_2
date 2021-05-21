#include "Collectable.h"

Collectable::Collectable(const ofVec2f pos, const float mass, const float radius, const float emission_frequency, const float emission_force, const bool is_active)
	:	is_active_(is_active)												// controlls if the particle emission is enabled
	,	make_active_on_next_emission_(false)
	,	emission_frequency_(emission_frequency)								// frequency of particle emission
	,	emission_force_(emission_force)										// force of particle emission
	,	starting_radius_(radius)
	,	needs_to_pulse_radius_(false)
	,	player_within_bounds_(false)
	,	alpha_(0)
	,	can_be_collected_(false)
	,	id_(get_cur_id())
{
	set_type("Collectable");
	set_position(pos);
	set_color(ofColor(passive_color_.r, passive_color_.g, passive_color_.b, 100));
	set_mass(mass);
	set_radius(radius);
	
	add_module("screenBounce");
	add_module("ellipseCollider");
	add_module("gravity");
	add_module("friction");
	add_module("mouseHover");

	pixel_buffer_before_drag_ = 2;
	mouse_over_radius_mult_ = 2;
	
	if (id_ == 0)
	{
		can_be_collected_ = true;
		inc_alpha_ = true;
	}
	// incase a point is added in sandbox mode and is enabled by default
	if (is_active_)
	{
		Collectable::points_collected_++;
		last_id_collected_ = id_;
		can_be_collected_ = false;
	}		
}

Collectable::Collectable(const ofVec2f pos, const float mass, const float radius, const bool is_active)
	: Collectable(pos, mass, ofRandom(15, 100), static_cast<int>(ofRandom(75, 100)), 0.1f, is_active)
{}



// Static vars

bool Collectable::first_point_ = true;
int Collectable::last_id_collected_ = -404;
int Collectable::cur_id_ = -1;
int Collectable::collectable_count_ = 0;

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
	collectable_count_++;
	
	cur_id_++;
	return cur_id_;
}

int Collectable::points_collected_ = 0;



void Collectable::update()
{
	check_if_active();
	
	if (can_emit())
	{
		emit_forces();
		pulse_radius();
	}
	
	update_forces();
	drag_nodes();
	update_gui();
	reset_forces();
}

bool Collectable::can_emit() const
{
	if (make_active_on_next_emission_ || is_active_)
	{
		return true;
	}
	return false;
}

// collectables randomly emit 'shock waves' which in effect causes 'streams' of particles to form (this could help the player to locate collectables)
void Collectable::emit_forces()
{
	if (ofGetFrameNum() % static_cast<int>(emission_frequency_) == 0)
	{
		vector<ofVec2f> point_positions;		
		for (auto& game_object : *game_objects_)
		{
			if (game_object->get_type() == "Collectable")
			{
				point_positions.push_back(game_object->get_position());
			}
		}

		ofVec2f vel;
		for (int i = 0; i < point_positions.size(); i++)
		{
			if (point_positions[i] == get_position())
			{
				int i2;
				if (i + 1 == point_positions.size())
					i2 = 0;
				else
					i2 = i + 1;

				vel = (point_positions[i2] - point_positions[i]);
				vel.normalize();
			}
		}		
		
		for (int i = 0; i < 100; i++)
		{
			ofVec2f mapped_pos;
			mapped_pos.x = ofMap(get_position().x + ofRandom(-starting_radius_ * 0.8f, starting_radius_ * 0.8f), -HALF_WORLD_WIDTH, HALF_WORLD_WIDTH, 0, 1);
			mapped_pos.y = ofMap(get_position().y + ofRandom(-starting_radius_ * 0.8f, starting_radius_ * 0.8f), -HALF_WORLD_HEIGHT, HALF_WORLD_HEIGHT, 0, 1);
			
			fluid_manager_->add_to_fluid(mapped_pos, vel * emission_force_ * 0.01f, true, true, 1);
		}

		needs_to_pulse_radius_ = true;		

		if (make_active_on_next_emission_)
		{
			is_active_ = true;
			make_active_on_next_emission_ = false;

			gui_values_need_to_be_set_ = true;

			audio_manager_->event_point_collected();
		}
	}
}

void Collectable::pulse_radius()
{
	if (needs_to_pulse_radius_)
	{
		set_radius(starting_radius_ * 2);
		needs_to_pulse_radius_ = false;

		audio_manager_->event_point_pulsed(get_position());
	}
	else if (get_radius() > starting_radius_)
	{
		set_radius(get_radius() - ((((starting_radius_ * 2) - starting_radius_) / emission_frequency_)));
	}
}

void Collectable::check_if_active()
{	
	if (!is_active_)
	{
		if (gamemode_manager_->get_current_mode_string() == "Procedural" && id_ == 0)
		{
			Collectable::points_collected_++;
			is_active_ = true;
			last_id_collected_ = id_;
			can_be_collected_ = false;
			make_active_on_next_emission_ = false;
		}
		else
		{
			int next_id;
			if (last_id_collected_ == collectable_count_ - 1)
				next_id = 0;
			else
				next_id = last_id_collected_ + 1;

			if (id_ == next_id)
			{
				can_be_collected_ = true;
				inc_alpha_ = true;
			}
		}
	}

	// gui controls
	if (gui_manager_->gui_world_activate_all_points)
	{
		is_active_ = true;
		
		if (id_ == Collectable::collectable_count_ - 1)
			gui_manager_->gui_world_activate_all_points = false;
	}
	else if (gui_manager_->gui_world_deactivate_all_points)
	{
		is_active_ = false;
		if (id_ == Collectable::collectable_count_ - 1)
			gui_manager_->gui_world_deactivate_all_points = false;
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
			gui_manager_->update_collectable_values(pos_, vel_, accel_, starting_radius_, emission_frequency_, emission_force_, is_active_, id_);
			gui_values_need_to_be_set_ = false;
		}
		else
		{
			gui_manager_->update_collectable_values(pos_, vel_, accel_, gui_manager_->gui_collectable_radius, gui_manager_->gui_collectable_emission_frequency, gui_manager_->gui_collectable_emission_force, gui_manager_->gui_collectable_is_active, id_);
			starting_radius_ = gui_manager_->gui_collectable_radius;
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
	if ((gamemode_manager_->get_current_mode_string() == "Sandbox" && gui_manager_->gui_world_enable_points_in_range) || gamemode_manager_->get_current_mode_string() != "Sandbox")
	{
		if (other->get_type() == "PullRange")
		{
			if (can_be_collected_/* || Collectable::first_point()*/)
			{
				first_point_ = false;
				can_be_collected_ = false;

				make_active_on_next_emission_ = true;
				alpha_ = 255;
				Collectable::last_id_collected_ = id_;
				Collectable::points_collected_++;				
			}
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
	
	draw_fill();
	draw_outline();

	ofPopStyle();
}

void Collectable::draw_fill()
{
	if (is_active_)
	{
		ofSetColor(ofColor(0, 0, 0, 50));
		ofFill();
		ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);
	}
}

void Collectable::draw_outline()
{
	// Draw outline
	if (gamemode_manager_->get_current_mode_string() == "Sandbox" || is_active_ || can_be_collected_ || make_active_on_next_emission_ || id_ == 0)
	{
		float r;
		if (is_active_)
		{
			alpha_ = ofMap(radius_, starting_radius_, starting_radius_ * 2, 0, 255);
			r = radius_;
		}
		else
		{
			if (inc_alpha_ && alpha_ < 254)
				alpha_+=2;
			else
				inc_alpha_ = false;

			r = starting_radius_;
		}

		if (gamemode_manager_->get_current_mode_string() == "Sandbox" && !is_active_)
		{
			(can_be_collected_) ? ofSetColor(0, 255, 0) : ofSetColor(255, 0, 0);
			ofSetLineWidth(0.05f);
		}
		else
		{
			(is_active_) ? set_color(ofColor(0, 255, 0)) : set_color(ofColor(255, 0, 0));
			get_color();
			ofSetLineWidth(ofMap(radius_, starting_radius_, starting_radius_ * 2, 0.1f, 2.0f));
		}

		ofNoFill();
		ofDrawEllipse(pos_.x, pos_.y, r, r);
	}
}

void Collectable::get_color() const
{
	if ((gamemode_manager_->get_current_mode_string() == "Sandbox") && ((get_is_selected() == true) || (mouse_over_ || mouse_drag_)))
	{
		ofSetColor(selected_color_);
	}
	else
	{
		ofSetColor(ofColor(color_.r, color_.g, color_.b, alpha_));
	}
}
