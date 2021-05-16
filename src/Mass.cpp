#include "Mass.h"

Mass::Mass(const ofVec2f pos, const float mass, const float radius)
{
	set_type("Mass");

	set_position(pos);
	set_mass(mass);
	set_radius(radius);
	set_color(ofColor(passive_color_));

	add_module("screenBounce");
	add_module("ellipseCollider");
	add_module("gravity");
	add_module("friction");
	add_module("mouseHover");

	pixel_buffer_before_drag_ = 2;
}

void Mass::update()
{	
	update_forces();
	drag_nodes();
	update_gui();
	reset_forces();
}

void Mass::update_forces()
{
	apply_force(accel_, get_fluid_force(), false);
	add_forces(false);
}

ofVec2f Mass::get_fluid_force()
{
	const ofVec2f inv_window_size(1.0f / WORLD_WIDTH, 1.0f / WORLD_HEIGHT);
	const ofVec2f window_size(WORLD_WIDTH, WORLD_HEIGHT);	
	force_ = fluid_manager_->get_solver()->getVelocityAtPos(ofVec2f(pos_.x + HALF_WORLD_WIDTH, pos_.y + HALF_WORLD_HEIGHT) * inv_window_size) * ofMap(get_mass(), 0, 5000, 0.003f, 0.00006f) * window_size + force_ * 0.5f;
	return force_;
}

void Mass::drag_nodes()
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

void Mass::update_gui()
{
	if (get_is_selected() == true)
	{
		if (gui_values_need_to_be_set_)
		{
			gui_manager_->update_mass_values(pos_, vel_, accel_, mass_, radius_);
			gui_values_need_to_be_set_ = false;
		}
		else {
			gui_manager_->update_mass_values(pos_, vel_, accel_, gui_manager_->gui_node_mass, gui_manager_->gui_node_radius);
			radius_ = gui_manager_->gui_node_radius;
			mass_ = gui_manager_->gui_node_mass;
		}
	}
}

void Mass::reset_forces()
{
	accel_.set(0);
}


// ----- EVENT FUNCTIONS ----- //


void Mass::mouse_pressed(const float x, const float y, const int button)
{
	if (button == 2 && mouse_over_)
	{
		if (get_is_selected() == false)
		{
			// select object
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

void Mass::mouse_dragged(const float x, const float y, const int button)
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

				mouse_pressed(x, y, button);
			}
		}
	}
}

void Mass::mouse_released(const float x, const float y, const int button)
{
	if (button == 2)
	{
		mouse_drag_ = false;
		game_controller_->set_mouse_dragged(false);
	}
}


// ----- RENDER LOOP ----- //


void Mass::draw()
{
	ofPushStyle();

	ofSetColor(ofColor(0, 0, 0, 50));
	ofFill();
	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);

	get_color();
	ofNoFill();
	ofSetLineWidth(ofMap(mass_, MINIMUM_MASS, MAXIMUM_MASS, 0.1f, 10.0f));

	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);

	ofPopStyle();
}

void Mass::get_color() const
{
	if ((gamemode_manager_->get_current_mode_string() == "Sandbox") && ((get_is_selected() == true) || (mouse_over_ || mouse_drag_)))
	{
		ofSetColor(selected_color_);
	}
	else
	{
		ofSetColor(ofColor(color_.r, color_.g, color_.b));
	}
}
