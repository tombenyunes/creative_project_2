#include "Spring.h"

Spring::Spring(const ofVec2f anchor_pos, vector<float> node_radiuses, vector<float> node_masses,
										 const float k, const float damping, const float springmass)
	:	k_(k),
		damping_(damping),
		springmass_(springmass),
		time_step_(0.28f),
		fill_ellipses_(false)
{
	set_type("Spring");
	set_color(passive_color_);
	set_position(anchor_pos);
	set_radius(8);	// radius of anchor

	add_module("screenBounce");
	add_module("ellipseCollider");
	add_module("gravity");
	add_module("friction");
	add_module("mouseHover");	

	for (int i = 0; i < node_masses.size(); i++)
	{
		create_node(ofVec2f(pos_.x + ofRandom(-50.0f, 50.0f), pos_.y), ofVec2f(0, 0), ofVec2f(0, 0), node_radiuses[i], node_masses[i]);
	}

	affected_by_gravity_ = true; // by default spring have 'local' gravity (the anchor doesn't) to make springiness noticible
	gravity_mult_ = 400;
	collision_mult_ = 4;
	pixel_buffer_before_drag_ = 0.5f;
}

void Spring::create_node(const ofVec2f node_pos, const ofVec2f node_vel, const ofVec2f node_accel, const float node_radius, const float node_mass)
{
	node_positions_.push_back(node_pos);
	node_velocities_.push_back(node_vel);
	node_accelerations_.push_back(node_accel);
	node_radiuses_.push_back(node_radius);
	node_masses_.push_back(node_mass);
}

void Spring::update()
{	
	update_forces();
	drag_nodes();
	update_gui();
	reset_forces();
}

void Spring::update_forces()
{
	apply_all_forces();
	add_forces();
}

void Spring::apply_all_forces()
{
	for (int i = 0; i < node_positions_.size(); i++)
	{
		apply_force(node_accelerations_[i], update_springs(i) * time_step_, false);
	}
}

ofVec2f Spring::update_springs(const int node)
{
	vector<ofVec2f> mass_spring_forces;
	for (int i = 0; i < node_positions_.size(); i++)
	{
		if (i - 1 < 0)
		{
			ofVec2f force;
			force.x = -k_ * (node_positions_[i].x - pos_.x);
			force.y = -k_ * (node_positions_[i].y - pos_.y);
			mass_spring_forces.push_back(force);
		}
		else
		{
			ofVec2f force;
			force.x = -k_ * (node_positions_[i].x - node_positions_[i - 1].x);
			force.y = -k_ * (node_positions_[i].y - node_positions_[i - 1].y);
			mass_spring_forces.push_back(force);
		}
	}

	vector<ofVec2f> mass_damping_forces;
	for (int i = 0; i < node_positions_.size(); i++)
	{
		ofVec2f force;
		force.x = damping_ * node_velocities_[i].x;
		force.y = damping_ * node_velocities_[i].y;
		mass_damping_forces.push_back(force);
	}

	vector<ofVec2f> mass_forces;
	for (int i = 0; i < node_positions_.size(); i++)
	{
		if (i + 1 < node_positions_.size())
		{
			ofVec2f force;
			force.x = mass_spring_forces[i].x - mass_damping_forces[i].x - mass_spring_forces[i + 1].x + mass_damping_forces[i + 1].x;
			force.y = mass_spring_forces[i].y + springmass_ - mass_damping_forces[i].y - mass_spring_forces[i + 1].y + mass_damping_forces[i + 1].y;
			mass_forces.push_back(force);
		}
		else
		{
			ofVec2f force;
			force.x = mass_spring_forces[i].x - mass_damping_forces[i].x;
			force.y = mass_spring_forces[i].y + springmass_ - mass_damping_forces[i].y;
			mass_forces.push_back(force);
		}
	}

	return mass_forces[node] / springmass_;
};

void Spring::drag_nodes()
{
	local_mouse_pos_before_drag_.set(cam_->get_local_mouse_pos());
	static ofVec2f mouse_pos_before_drag;

	if (mouse_drag_)
	{
		if (mouse_over_index_ != -1) {
			const ofVec2f prev_pos2 = ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y) + mouse_offset_from_center_;

			ofVec2f new_pos; 
			new_pos.x = ofLerp(node_positions_[mouse_over_index_].x, prev_pos2.x, 0.1f);
			new_pos.y = ofLerp(node_positions_[mouse_over_index_].y, prev_pos2.y, 0.1f);

			node_positions_[mouse_over_index_].set(new_pos);
			node_velocities_[mouse_over_index_].set(0);

			started_dragging_ = true;
			mouse_pos_before_drag = ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y);
		}
		else
		{
			pos_.set(ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y));
		}
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

/*void Spring::drag_nodes()
{
	local_mouse_pos_before_drag_.set(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y);

	if (mouse_drag_)
	{
		if (mouse_over_index_ != -1)
		{
			node_positions_[mouse_over_index_].set(ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y) + mouse_offset_from_center_);
			node_velocities_[mouse_over_index_].set(0);
		}
		else
		{
			pos_.set(ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y));
		}
	}
}*/

void Spring::update_gui()
{
	if (get_is_selected() == true)
	{
		if (gui_values_need_to_be_set_)
		{
			if (selected_node_index_ != -1)
			{
				gui_manager_->update_multiple_values(pos_, k_, damping_, springmass_, affected_by_gravity_, node_positions_[selected_node_index_], node_velocities_[selected_node_index_], node_accelerations_[selected_node_index_], node_masses_[selected_node_index_], node_radiuses_[selected_node_index_]);
			}
			else
			{
				gui_manager_->update_multiple_values(pos_, k_, damping_, springmass_, affected_by_gravity_);
			}
			gui_values_need_to_be_set_ = false;
		}
		else
		{
			if (selected_node_index_ != -1)
			{
				gui_manager_->update_multiple_values(pos_, k_, damping_, springmass_, affected_by_gravity_, node_positions_[selected_node_index_], node_velocities_[selected_node_index_], node_accelerations_[selected_node_index_], gui_manager_->node_mass, gui_manager_->node_radius);
				node_masses_[selected_node_index_] = gui_manager_->node_mass;
				node_radiuses_[selected_node_index_] = gui_manager_->node_radius;
			}
			else
			{
				gui_manager_->update_multiple_values(pos_, gui_manager_->k, gui_manager_->damping, gui_manager_->springmass, gui_manager_->spring_affected_by_gravity);
				k_ = gui_manager_->k;
				damping_ = gui_manager_->damping;
				springmass_ = gui_manager_->springmass;
				affected_by_gravity_ = gui_manager_->spring_affected_by_gravity;
			}
		}
	}
}

void Spring::reset_forces()
{
	for (int i = 0; i < node_positions_.size(); i++)
	{
		node_accelerations_[i].set(0);
	}
}

void Spring::add_forces()
{
	for (int i = 0; i < node_positions_.size(); i++)
	{
		node_velocities_[i] += node_accelerations_[i];
		node_positions_[i] += node_velocities_[i] * time_step_;
	}
}

void Spring::ellipse_collider()
{
	for (auto& game_object : *game_objects_)
	{
		if (game_object->can_collide())
		{
			if (game_object != this && game_object->get_type() != "Spring")
			{
				for (int j = 0; j < node_positions_.size(); j++)
				{
					if (Collisions::ellipse_compare(node_positions_[j], node_radiuses_[j], game_object->get_position(), game_object->get_radius()))
					{
						is_colliding(game_object, j);
						game_object->is_colliding(this, node_positions_[j]);
					}
				}
			}
		}
	}
}

void Spring::is_colliding(GameObject* other, const int node_index)
{
	audio_manager_->playRandomSample();

	const ofVec2f force_vec = node_positions_[node_index] - other->get_position();
	ofVec2f accel = force_vec / node_masses_[node_index];
	accel *= collision_mult_;
	apply_force(node_accelerations_[node_index], accel, false);
}


// ----- EVENT FUNCTIONS ----- //


void Spring::mouse_pressed(const float x, const float y, const int button)
{
	if (button == 2 && mouse_over_)
	{		
		if (get_is_selected() == false)
		{
			// select object
			set_request_to_be_selected(true);
			
			selected_node_index_ = mouse_over_index_; // index of selected node
			
			gui_values_need_to_be_set_ = true;
		}
		else if (mouse_over_index_ != selected_node_index_)
		{
			selected_node_index_ = mouse_over_index_; // index of selected node

			gui_values_need_to_be_set_ = true;
		}
		else
		{
			set_request_to_be_deselected(true);
		}					
	}
}

void Spring::mouse_dragged(const float x, const float y, const int button)
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
			}
		}
	}
}

void Spring::mouse_released(const float x, const float y, const int button)
{
	if (button == 2)
	{
		mouse_drag_ = false;
		game_controller_->set_mouse_dragged(false);
	}
}

void Spring::key_pressed(const int key)
{
	if (key == 'a')
	{
		create_node(ofVec2f(pos_.x + ofRandom(-50, 50), pos_.y), ofVec2f(0, 0), ofVec2f(0, 0), node_radiuses_[node_radiuses_.size() - 1], node_masses_[node_radiuses_.size() - 1]);
	}
}


// ----- RENDER LOOP ----- //


void Spring::draw()
{
	ofPushStyle();

	ofNoFill();
	ofSetColor(color_);
	get_node_color(-2);

	draw_connecting_lines();

	(fill_ellipses_) ? ofFill() : ofNoFill();
	get_node_color(-1);
	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);

	ofSetColor(0);
	for (int i = 0; i < node_positions_.size(); i++)
	{
		ofDrawEllipse(node_positions_[i].x, node_positions_[i].y, node_radiuses_[i], node_radiuses_[i]);
	}

	for (int i = 0; i < node_positions_.size(); i++)
	{
		ofSetLineWidth(ofMap(node_masses_[i], MINIMUM_MASS, MAXIMUM_MASS / 2, 0.1f, 10.0f));
		ofNoFill();
		get_node_color(i);
		ofDrawEllipse(node_positions_[i].x, node_positions_[i].y, node_radiuses_[i], node_radiuses_[i]);
	}

	ofPopStyle();
}

void Spring::get_node_color(const int node_index)
{
	if ((get_is_selected() == true) && (selected_node_index_ == -1 || selected_node_index_ == node_index || ((mouse_over_ || mouse_drag_) && mouse_over_index_ == node_index)))
	{
		ofSetColor(selected_color_);
	}
	else
	{
		ofSetColor(passive_color_);
	}
}

void Spring::draw_connecting_lines()
{
	for (int i = 0; i < node_positions_.size(); i++)
	{
		if (i == 0)
		{
			if (fill_ellipses_)
			{
				ofDrawLine(node_positions_[i].x, node_positions_[i].y, pos_.x, pos_.y);
			}
			else
			{
				const float from = angle_between(node_positions_[i], pos_);
				const float to = angle_between(pos_, node_positions_[i]);

				const ofVec2f point_from = get_point_on_circle(node_positions_[i], from, node_radiuses_[i] / 2);
				const ofVec2f point_to = get_point_on_circle(pos_, to, radius_ / 2);

				if (Collisions::ellipse_compare(node_positions_[i], node_radiuses_[i], pos_, radius_) == false)
				{
					ofDrawLine(point_from.x, point_from.y, point_to.x, point_to.y);
				}
			}
		}
		else
		{
			if (fill_ellipses_)
			{
				ofDrawLine(node_positions_[i].x, node_positions_[i].y, node_positions_[i - 1].x, node_positions_[i - 1].y);
			}
			else
			{
				const float from = angle_between(node_positions_[i], node_positions_[i - 1]);
				const float to = angle_between(node_positions_[i - 1], node_positions_[i]);

				const ofVec2f point_from = get_point_on_circle(node_positions_[i], from, node_radiuses_[i] / 2);
				const ofVec2f point_to = get_point_on_circle(node_positions_[i - 1], to, node_radiuses_[i - 1] / 2);

				if (Collisions::ellipse_compare(node_positions_[i], node_radiuses_[i], node_positions_[i - 1], node_radiuses_[i - 1]) == false)
				{
					ofDrawLine(point_from.x, point_from.y, point_to.x, point_to.y);
				}
			}
		}
	}
}

float Spring::angle_between(const ofVec2f from, const ofVec2f to) const
{
	const float x = from.x;
	const float y = from.y;

	const float delta_x = to.x - x;
	const float delta_y = to.y - y;

	float rotation = -atan2(delta_x, delta_y);
	rotation += 3.14159f / 2.0f;

	return rotation;
}

ofVec2f Spring::get_point_on_circle(const ofVec2f center, const float radians, const float radius) const
{

	const float x = center.x;
	const float y = center.y;

	//radians = radians - Math.toRadians(90.0); // 0 becomes the top

	const float x_pos = round(static_cast<float>(x + cos(radians) * radius));
	const float y_pos = round(static_cast<float>(y + sin(radians) * radius));

	const ofVec2f point = ofVec2f(x_pos, y_pos);

	return point;

}
