#include "GameObject.h"

GameObject::GameObject(const ofVec2f pos, const ofColor color)
	: game_objects_(nullptr),
	  game_controller_(nullptr),
	  gui_manager_(nullptr),
	  fluid_manager_(nullptr),
	  audio_manager_(nullptr),
	  cam_(nullptr),
	  pos_(pos),
	  prev_pos_(99999, 99999),
	  mass_(10),
	  radius_(35),
	  vel_(0),
	  accel_(0),
	  color_(color),
	  infinite_mass_(false),
	  affected_by_gravity_(false),
	  gravity_mult_(1),
	  collision_mult_(1),
	  mouse_over_(false),
	  mouse_over_index_(0),
	  selected_node_index_(0),
	  mouse_drag_(false),
	  mouse_offset_from_center_(0),
	  local_mouse_pos_before_drag_(0),
	  pixel_buffer_before_drag_(0),
	  started_dragging_(false),
	  passive_color_(ofColor(255, 255, 255)),
	  selected_color_(ofColor(255, 165, 0)),
	  delete_key_down_(false),
	  request_to_be_deleted_(false),
	  request_to_be_selected_(false),
	  is_selected_(false),
	  gui_values_need_to_be_set_(true),
	  screen_wrap_enabled_(false),
	  screen_bounce_enabled_(false),
	  gravity_enabled_(false),
	  friction_enabled_(false),
	  mouse_hover_enabled_(false),
	  ellipse_collider_enabled_(false)
{
}

void GameObject::init(vector<GameObject*>* gameobjects, Controller* controller, GUIManager* gui_manager, Camera* cam, FluidManager* fluid_manager, AudioManager* audio_manager, GamemodeManager* gamemode_manager)
{
	game_objects_ = gameobjects;
	game_controller_ = controller;
	gui_manager_ = gui_manager;
	fluid_manager_ = fluid_manager;
	audio_manager_ = audio_manager;
	gamemode_manager_ = gamemode_manager;

	cam_ = cam;
}

// root update is called prir to the main update function of a gameobject and is responsible for handling object deletion and updating user-added modules - it automatically updates the main update funcion
void GameObject::root_update()
{
	if (delete_key_down_)
	{
		if (mouse_over_)
		{
			set_request_to_be_deleted(true);
			set_request_to_be_deleted_event("User");
		}
	}
	if (!request_to_be_deleted_)
	{
		if (screen_wrap_enabled_)
		{
			screen_wrap();
		}
		if (screen_bounce_enabled_)
		{
			screen_bounce();
		}
		if (gravity_enabled_)
		{
			gravity();
		}
		if (friction_enabled_)
		{
			friction();
		}
		if (ellipse_collider_enabled_)
		{
			ellipse_collider();
		}
		if (mouse_hover_enabled_)
		{
			mouse_hover();
		}

		prev_pos_ = pos_;

		update(); // <--- user defined update function for every gameobject
	}
	else
	{
		//cout << "Error: 'Dead' GameObject is still being updated" << endl;
	}
}

// ----- MODULES ----- //

// I made the module system to be an easy way to share functions to only the objects that need them

// Simply including `AddModule("module-name")` to the constructor of a new gameobject will cause this module to be updated

void GameObject::add_module(const string id)
{
	if (id == "screenWrap")
	{
		screen_wrap_enabled_ = true;
	}
	else if (id == "screenBounce")
	{
		screen_bounce_enabled_ = true;
	}
	else if (id == "gravity")
	{
		gravity_enabled_ = true;
	}
	else if (id == "friction")
	{
		friction_enabled_ = true;
	}
	else if (id == "ellipseCollider")
	{
		ellipse_collider_enabled_ = true;
	}
	else if (id == "mouseHover")
	{
		mouse_hover_enabled_ = true;
	}
	else
	{
		cout << "Error: Module ID is invalid" << endl;
	}
}

// upon reaching the screen edge, the object is placed at the opposite edge
void GameObject::screen_wrap()
{
	if (pos_.x > 0 + (HALF_WORLD_WIDTH))
	{
		pos_.x = 0 - (HALF_WORLD_WIDTH);
	}
	if (pos_.x < 0 - (HALF_WORLD_WIDTH))
	{
		pos_.x = 0 + (HALF_WORLD_WIDTH);
	}
	if (pos_.y < 0 - (HALF_WORLD_HEIGHT))
	{
		pos_.y = 0 + (HALF_WORLD_HEIGHT);
	}
	if (pos_.y > 0 + (HALF_WORLD_HEIGHT))
	{
		pos_.y = 0 - (HALF_WORLD_HEIGHT);
	}
}

// object 'bounce' when hitting the screen edge
void GameObject::screen_bounce()
{
	if (node_positions_.empty())
	{
		if (pos_.x > 0 + (HALF_WORLD_WIDTH) - (radius_) / 2)
		{
			vel_.x *= -1;
			pos_.x = 0 + (HALF_WORLD_WIDTH) - (radius_) / 2;
		}
		if (pos_.x < 0 - (HALF_WORLD_WIDTH) + (radius_) / 2)
		{
			vel_.x *= -1;
			pos_.x = 0 - (HALF_WORLD_WIDTH) + (radius_) / 2;
		}
		if (pos_.y < 0 - (HALF_WORLD_HEIGHT) + (radius_) / 2)
		{
			vel_.y *= -1;
			pos_.y = 0 - (HALF_WORLD_HEIGHT) + (radius_) / 2;
		}
		if (pos_.y > 0 + (HALF_WORLD_HEIGHT) - (radius_) / 2)
		{
			vel_.y *= -1;
			pos_.y = 0 + (HALF_WORLD_HEIGHT) - (radius_) / 2;
		}
	}
	else
	{
		for (int i = 0; i < node_positions_.size(); i++)
		{
			if (node_positions_[i].x > 0 + (HALF_WORLD_WIDTH) - (node_radiuses_[i]) / 2)
			{
				node_velocities_[i].x *= -1;
				node_positions_[i].x = 0 + (HALF_WORLD_WIDTH) - (node_radiuses_[i]) / 2;
			}
			if (node_positions_[i].x < 0 - (HALF_WORLD_WIDTH) + (node_radiuses_[i]) / 2)
			{
				node_velocities_[i].x *= -1;
				node_positions_[i].x = 0 - (HALF_WORLD_WIDTH) + (node_radiuses_[i]) / 2;
			}
			if (node_positions_[i].y < 0 - (HALF_WORLD_HEIGHT) + (node_radiuses_[i]) / 2)
			{
				node_velocities_[i].y *= -1;
				node_positions_[i].y = 0 - (HALF_WORLD_HEIGHT) + (node_radiuses_[i]) / 2;
			}
			if (node_positions_[i].y > 0 + (HALF_WORLD_HEIGHT) - (node_radiuses_[i]) / 2)
			{
				node_velocities_[i].y *= -1;
				node_positions_[i].y = 0 + (HALF_WORLD_HEIGHT) - (node_radiuses_[i]) / 2;
			}
		}
	}
}

// simple ellipse collision detection
void GameObject::ellipse_collider()
{
	for (auto& game_object : *game_objects_)
	{
		if (game_object->ellipse_collider_enabled_)
		{
			if (game_object != this)
			{
				if (game_object->type_ != "Spring")
				{
					if (Collisions::ellipse_compare(pos_, radius_, game_object->pos_, game_object->radius_))
					{
						is_colliding(game_object);
					}
				}
			}
		}
	}
}
// called when an object is currently colliding
void GameObject::is_colliding(GameObject* other, const ofVec2f node_pos)
{
	ofVec2f other_pos;
	if (other->type_ == "Spring")
	{
		other_pos = node_pos;
	}
	else
	{
		other_pos = other->pos_;
	}

	if (game_controller_->get_use_hard_collisions())
	{
		const ofVec2f force_vec = pos_ - other_pos;
		if (prev_pos_ != ofVec2f(99999, 99999)) pos_ = prev_pos_;
		//vel.set(0);
		apply_force(accel_, (force_vec / mass_), false);
	}
	else
	{
		const ofVec2f force_vec = pos_ - other_pos;
		apply_force(accel_, (force_vec / mass_), true);
	}
}

void GameObject::gravity()
{
	if (node_positions_.empty())
	{
		if (game_controller_->get_gravity() == 1 || affected_by_gravity_)
		{
			const ofVec2f new_force = {0, static_cast<float>(GRAVITY_FORCE) * gravity_mult_ * mass_};
			apply_force(accel_, new_force, false);
		}
	}
	else
	{
		for (int i = 0; i < node_positions_.size(); i++)
		{
			if (game_controller_->get_gravity() == 1 || affected_by_gravity_)
			{
				const ofVec2f new_force = {0, static_cast<float>(GRAVITY_FORCE) * gravity_mult_ * node_masses_[i]};
				apply_force(node_accelerations_[i], new_force, false);
			}
		}
	}
}

void GameObject::friction()
{
	if (node_positions_.empty())
	{
		ofVec2f friction = vel_ * -1;
		friction *= FRICTION_FORCE;
		apply_force(accel_, friction, true);
	}
	else
	{
		for (int i = 0; i < node_positions_.size(); i++)
		{
			ofVec2f friction = node_velocities_[i] * -1;
			friction *= FRICTION_FORCE;
			apply_force(accel_, friction, false);
		}
	}
}

// determines if the mouse is over an object
void GameObject::mouse_hover()
{
	if (gamemode_manager_->get_current_mode_string() == "Sandbox")
	{
		if (node_positions_.empty())
		{
			if (game_controller_->get_mouse_dragged() == false)
			{
				if (Collisions::ellipse_compare(pos_, radius_, ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y), 0))
				{
					mouse_over_ = true;
					mouse_offset_from_center_ = pos_ - cam_->get_world_mouse_pos();
				}
				else {
					mouse_over_ = false;
				}
			}
		}
		else {
			if (game_controller_->get_mouse_dragged() == false)
			{
				for (int i = 0; i < node_positions_.size(); i++)
				{
					if (Collisions::ellipse_compare(node_positions_[i], node_radiuses_[i], cam_->get_world_mouse_pos(), 0))
					{
						mouse_over_ = true;
						mouse_over_index_ = i;
						mouse_offset_from_center_ = node_positions_[i] - cam_->get_world_mouse_pos();
						break;
					}
					else if (Collisions::ellipse_compare(pos_, radius_, cam_->get_world_mouse_pos(), 0))
					{
						mouse_over_ = true;
						mouse_over_index_ = -1;
						mouse_offset_from_center_ = pos_ - cam_->get_world_mouse_pos();
						break;
					}
					else {
						mouse_over_ = false;
					}
				}
			}
		}
	}
	else
	{
		mouse_over_ = false;
	}
}


// Shared physics functions


void GameObject::apply_force(ofVec2f& accel, ofVec2f force, const bool limit, const float limit_amount)
{
	if (limit)
	{
		force.limit(limit_amount);
		accel += force;
	}
	else
	{
		accel += force;
		add_forces(false);
	}
}

void GameObject::add_forces(const bool interp_pos)
{
	if (node_positions_.empty())
	{
		vel_ += accel_;
		vel_.limit(MAXIMUM_VELOCITY);
		if (interp_pos)
		{
			pos_ = get_interpolated_position();
		}
		else
		{
			pos_ += vel_;
		}
	}
	else
	{
		/*for (int i = 0; i < nodePositions.size(); i++) {
			nodeVelocities[i] += nodeAccelerations[i];
			nodePositions[i] += nodeVelocities[i] * 0.28;
		}*/
	}
}

ofVec2f GameObject::get_interpolated_position()
{
	const int progress = (ofGetFrameNum() % 100) / 100;
	ofVec2f pow_interp_in;
	pow_interp_in.x = ofNextPow2(progress);
	pow_interp_in.y = ofNextPow2(progress);

	ofVec2f new_pos;
	new_pos.x = ofLerp(pos_.x, pos_.x + vel_.x, pow_interp_in.x);
	new_pos.y = ofLerp(pos_.y, pos_.y + vel_.y, pow_interp_in.y);
	return new_pos;
}


// ----- EVENT FUNCTIONS ----- //


// functions in the same way as root_update
void GameObject::root_key_pressed(const int key)
{
	if (key == 'x')
	{
		delete_key_down_ = true;
	}
	key_pressed(key);
}

void GameObject::root_key_released(const int key)
{
	if (key == 'x')
	{
		delete_key_down_ = false;
	}
	key_released(key);
}


// ----- RENDER LOOP ----- //


void GameObject::root_draw()
{
	if (!request_to_be_deleted_)
	{
		draw();
	}
}