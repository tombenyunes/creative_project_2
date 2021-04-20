#include "Player.h"

Player::Player(const ofVec2f pos, const ofColor color)
	:	mouse_down_(false),
		mouse_button_(-1),
		mouse_pos_(0),
		aiming_boost_(false)
{
	set_type("Player");
	set_position(pos);
	set_color(color);
	set_velocity(ofVec2f(0));
	//set_accel(0);
	set_radius(14); // 35
	set_mass(500);
	
	add_module("screenBounce");
	add_module("ellipseCollider");
	add_module("gravity");
	add_module("friction");
}

void Player::update()
{
	pull_points();
	draw_particle_trail();
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
	if (player_can_move()) apply_force(accel_, get_movement_vector(), true, 0.15);	
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
	ofVec2f movementVec = pos_ - mouse_pos_;
	movementVec.scale(5);
	return movementVec;
}

void Player::pull_points()
{
	for (auto& game_object : *game_objects_)
	{
		if (game_object->can_collide())
		{
			if (game_object != this)
			{
				if (game_object->get_type() == "Point")
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
	static bool initiai_values_triggered = false; // initial values are sent to the gui_manager initially, after which it will update the results internally, and the object can receive the values back
	if (!initiai_values_triggered) {
		initiai_values_triggered = true;
		gui_manager_->update_values(pos_, vel_, accel_, mass_, infinite_mass_, radius_, affected_by_gravity_, 1);
	}
	else {
		gui_manager_->update_values(pos_, vel_, accel_, gui_manager_->mass, gui_manager_->infinite_mass, gui_manager_->radius, gui_manager_->affected_by_gravity, 1); // receiving and updating the results from the GUI_Manager
		if (infinite_mass_) mass_ = 9999999999999999999; else mass_ = gui_manager_->mass;
		radius_ = gui_manager_->radius;
		infinite_mass_ = gui_manager_->infinite_mass;
		affected_by_gravity_ = gui_manager_->affected_by_gravity;
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
	mouse_pos_ = { static_cast<float>(x), static_cast<float>(y) };
}

void Player::mouse_dragged(const float x, const float y, const int button)
{
	mouse_down_ = true;
	mouse_button_ = button;
	mouse_pos_ = { static_cast<float>(x), static_cast<float>(y) };
}

void Player::mouse_released(const float x, const float y, const int button)
{
	mouse_down_ = false;
}

void Player::key_pressed(const int key)
{
	if (key == 114) // r
	{
		accel_ = { 0, 0 };
		vel_ = { 0, 0 };
		pos_ = { 0, 0 };
	}
	if (key == 32)
	{
		//aimingBoost = true;
	}
}

void Player::key_released(const int key)
{
	if (key == 32)
	{
		//boostPlayer();
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
	ofSetColor(255);

	if (aiming_boost_) draw_boost_direction();
	//drawParticleTrail();

	ofSetColor(color_);
	ofFill();
	ofEllipse(pos_.x, pos_.y, radius_, radius_);
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

void Player::draw_particle_trail() const
// draws particle trail following the player when moving
{
	if (mouse_down_ && mouse_button_ == 0) {
		// draw new particles
		//float posX = ofMap(pos.x, -ofGetWidth()/2, ofGetWidth()/2, 0, 1);
		//float posY = ofMap(pos.y, -ofGetHeight() / 2, ofGetHeight() / 2, 0, 1);
		//ofVec2f mouseNorm = ofVec2f(eventPos) / ofGetWindowSize();
		//ofVec2f mouseVel = ofVec2f(eventPos - pMouse) / ofGetWindowSize();
		//addToFluid(ofVec2f(posX, posY), vel / 100 * -1, true, true, 1);
		
		ofVec2f new_pos;
		new_pos.x = ofMap(pos_.x/* + ofRandom(-radius / 4, radius / 4)*/, -WORLD_WIDTH / 2, WORLD_WIDTH / 2, 0, 1);
		new_pos.y = ofMap(pos_.y/* + ofRandom(-radius / 4, radius / 4)*/, -WORLD_HEIGHT / 2, WORLD_HEIGHT / 2, 0, 1);
		
		ofVec2f new_vel;
		//newVel.x = ((vel.x + ofRandom(-1, 1)) / 600) * -1;
		//newVel.y = ((vel.y + ofRandom(-1, 1)) / 600) * -1;
		new_vel.x = ((get_movement_vector().x + ofRandom(-1, 1)) / 6400) * -1;
		new_vel.y = ((get_movement_vector().y + ofRandom(-1, 1)) / 6400) * -1;

		fluid_manager_->add_to_fluid(new_pos, new_vel, true, true);
	}
}