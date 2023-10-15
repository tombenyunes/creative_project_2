#include "Player.h"

Player::Player(const ofVec2f pos, const ofColor color, const float radius)
	:	movement_speed(0.20f)
	,	mouse_down_(false)
	,	mouse_button_(-1)
	,	mouse_pos_(0)
	,	aiming_boost_(false)
	,	player_following_mouse_(false)
{
	set_type("Player");
	set_position(pos);
	set_color(color);
	set_velocity(ofVec2f(0));
	set_radius(radius);
	set_mass(500);
	
	add_module("screenBounce");
	add_module("ellipseCollider");
	add_module("gravity");
	add_module("friction");
	add_module("mouseHover");
}

void Player::update()
{
	follow_mouse();
	pull_points();
	draw_fluid_trail();
	update_forces();
	update_gui();
	reset_forces();
}

void Player::update_forces()
{
	apply_all_forces();
	add_forces(true);
}

void Player::apply_all_forces()
{
	if (player_can_move())
	{
		apply_force(accel_, get_movement_vector(), true, movement_speed);
		audio_manager_->event_player_started_moving();
	}
	else
	{
		audio_manager_->event_player_stopped_moving();
	}
}

bool Player::player_can_move() const
{
	if (mouse_down_ && mouse_button_ == 0) {
		return true;
	}
	else {
		return false;
	}
}

ofVec2f Player::get_movement_vector() const
{
	ofVec2f movement_vec = pos_ - mouse_pos_;
	movement_vec.scale(5);
	return movement_vec;	
}

void Player::follow_mouse()
{
	if (player_following_mouse_)
	{
		set_position(ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y));
	}
	// to avoid the player moving after the menu is open
	else if (gamemode_manager_->get_current_mode_string() == "Menu" && mouse_down_)
	{
		mouse_down_ = false;		
	}
}

void Player::pull_points()
{
	for (auto& game_object : *game_objects_)
	{
		if (game_object->can_collide())
		{
			if (game_object != this)
			{
				if (game_object->get_type() == "Collectable")
				{
					if (Collisions::ellipse_compare(pos_, 600, game_object->get_position(), game_object->get_radius()))
					{											
						// move points towards player
						GameObject pull_range;
						pull_range.set_position(pos_);
						pull_range.set_type("PullRange");
						game_object->is_colliding(&pull_range);
					}
				}
			}
		}
	}
}

void Player::update_gui()
{
	if (get_is_selected())
	{
		static bool initiai_values_triggered = false; // initial values are sent to the gui_manager initially, after which it will update the results internally, and the object can receive the values back
		if (!initiai_values_triggered) {
			initiai_values_triggered = true;
			gui_manager_->update_player_values(pos_, vel_, accel_, mass_, infinite_mass_, radius_);
		}
		else {
			gui_manager_->update_player_values(pos_, vel_, accel_, gui_manager_->gui_player_mass, gui_manager_->gui_player_infinite_mass, gui_manager_->gui_player_radius); // receiving and updating the results from the GUI_Manager
			if (infinite_mass_) mass_ = 9999999999999999999.0f; else mass_ = gui_manager_->gui_player_mass;
			radius_ = gui_manager_->gui_player_radius;
			infinite_mass_ = gui_manager_->gui_player_infinite_mass;
		}
	}
}

void Player::reset_forces()
{
	accel_.set(0);
}


// ----- EVENT FUNCTIONS ----- //


void Player::mouse_pressed(const float x, const float y, const int button)
{
	mouse_down_ = true;
	mouse_button_ = button;
	mouse_pos_ = { x, y };

	if (button == 2 && mouse_over_)
	{
		if (get_is_selected() == false)
		{
			// select object
			set_request_to_be_selected(true);

			gui_values_need_to_be_set_ = true;
		}
	}
}

void Player::mouse_dragged(const float x, const float y, const int button)
{
	mouse_down_ = true;
	mouse_button_ = button;
	mouse_pos_ = { x, y };
}

void Player::mouse_released(const float x, const float y, const int button)
{
	mouse_down_ = false;
}

void Player::key_pressed(const int key)
{
	if (gamemode_manager_->get_current_mode_string() == "Sandbox")
	{
		if (key == 't')
		{
			player_following_mouse_ = true;
		}
	}
}

void Player::key_released(const int key)
{
	if (key == 't')
	{
		player_following_mouse_ = false;
	}
}

void Player::boost_player()
{
	aiming_boost_ = false;
	if (vel_.length() < 5) {
		apply_force(accel_, (pos_ - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2)), true, 10);
	}
}


// ----- RENDER LOOP ----- //


void Player::draw()
{
	ofPushStyle();
	
	if (gamemode_manager_->get_current_mode_string() != "Menu")
	{		
		if ((gamemode_manager_->get_current_mode_string() == "Sandbox") && ((get_is_selected() == true) || (mouse_over_ || mouse_drag_)))
		{
			set_color(selected_color_);
		}
		else
		{
			set_color(passive_color_);
		}

		if (aiming_boost_) draw_boost_direction();

		ofSetColor(color_);
		ofFill();
		ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);

		ofNoFill();
	}

	ofPopStyle();
}

void Player::draw_boost_direction() const
// draws dotted line in the direction the player is aiming
{
	ofPushMatrix();

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(8, 0xAAAA);
	glBegin(GL_LINES);
	glVertex3f(pos_.x, pos_.y, 0);
	glVertex3f(draw_vel_path().x, draw_vel_path().y, draw_vel_path().z);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
	glFlush();

	ofPopMatrix();
}

ofVec3f Player::draw_vel_path() const
{
	const ofVec2f vec = pos_ - ofVec2f(ofGetMouseX() - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
	return ofVec3f(vec.x * ofGetWidth(), vec.y * ofGetHeight(), 0);
}

void Player::draw_fluid_trail() const
// draws particle trail following the player when moving
{
	if (mouse_down_ && mouse_button_ == 0)
	{		
		ofVec2f new_pos;
		//new_pos.x = ofMap(pos_.x + ofRandom(-get_radius() / 4, get_radius() / 4), -HALF_WORLD_WIDTH, HALF_WORLD_WIDTH, 0, 1);
		//new_pos.y = ofMap(pos_.y + ofRandom(-get_radius() / 4, get_radius() / 4), -HALF_WORLD_HEIGHT, HALF_WORLD_HEIGHT, 0, 1);
		new_pos.x = ofMap(pos_.x + ofRandom(-get_radius() / 2, get_radius() / 2), -HALF_WORLD_WIDTH, HALF_WORLD_WIDTH, 0, 1);
		new_pos.y = ofMap(pos_.y + ofRandom(-get_radius() / 2, get_radius() / 2), -HALF_WORLD_HEIGHT, HALF_WORLD_HEIGHT, 0, 1);

		ofVec2f new_vel;
		//new_vel.x = ((get_movement_vector().x + ofRandom(-1, 1)) / 6400) * -1;
		//new_vel.y = ((get_movement_vector().y + ofRandom(-1, 1)) / 6400) * -1;
		new_vel.x = ((get_movement_vector().x + ofRandom(-1, 1)) / 6400) * -1;
		new_vel.y = ((get_movement_vector().y + ofRandom(-1, 1)) / 6400) * -1;

		fluid_manager_->add_to_fluid(new_pos, new_vel, true, true);
	}
}
