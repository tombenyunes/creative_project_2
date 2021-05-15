#include "Camera.h"

Camera::Camera()
	:	view_(Cam_modes_::player_view)
	,	position_(2000, 1500)
	,	scale_(1)
	,	follow_player_(true)
	,	prev_player_view_scale_(1)
	,	zooming_out_(false)
	,	zooming_in_(false)
	,	keyboard_zooming_speed_(0.025f)
	,	scrollwheel_zooming_intervals_(4)
	,	zoom_scale_upper_bound_(3.25f)
	,	zoom_scale_lower_bound_(0.03f)
	,	lerping_scale_(false)
	,	scale_to_lerp_to_(0)
	,	lerping_position_(false)
	,	pos_to_lerp_to_(0)
{
	cam_.disableMouseInput();
	cam_.setVFlip(true); // flipping the axis so it's as if we're in 2D, otherwise all the fluid/movement code will be fucked	
	cam_.enableOrtho();
	cam_.removeAllInteractions();
}

void Camera::update(const ofVec2f player_pos)
{
	calculate_mouse_coords();	
	handle_position(player_pos);
	handle_scale();
}

void Camera::calculate_mouse_coords()
{
	const float mult = ofGetHeight() / 1080.0f;
	
	// calculate local + world mouse positions	
	const ofVec3f local_pos = ofVec3f(ofGetMouseX() - HALF_WORLD_WIDTH, ofGetMouseY() - HALF_WORLD_HEIGHT, 0);
	
	ofVec3f world_pos = screen_to_world(local_pos);
	world_pos.x += WORLD_WIDTH * ofMap(get_scale() / mult, 1.0f, 3.0f, 0.0f, 1.0f);
	world_pos.y += WORLD_HEIGHT * ofMap(get_scale() / mult, 1.0f, 3.0f, 0.0f, 1.0f);

	local_mouse_pos_ = ofVec2f(ofGetMouseX() / 2 - ofGetWidth() / 2, ofGetMouseY() - ofGetHeight() / 2);
	world_mouse_pos_ = world_pos;
}

void Camera::handle_position(const ofVec2f player_pos)
{
	cam_.setPosition(position_.x, position_.y, 1305);

	follow_player(player_pos);
	
	if (lerping_position_)
	{
		lerp_position();
	}	
}

void Camera::follow_player(const ofVec2f player_pos)
{
	// the camera follows the player
	if (view_ != Cam_modes_::free_view)
	{
		if (view_ == Cam_modes_::player_view)
		{
			if (ofGetFrameNum() > 0) { // <-- otherwise the cam briefly stutters on game launch
				pos_to_lerp_to_.set(ofVec2f(player_pos.x, player_pos.y));
				lerping_position_ = true;
			}
		}
		else if (view_ == Cam_modes_::map_view)
		{
			pos_to_lerp_to_.set(ofVec2f(0 + HALF_WORLD_WIDTH, 0 + HALF_WORLD_HEIGHT));
			lerping_position_ = true;
		}
	}
}

void Camera::lerp_position()
{
	ofVec2f lerped_pos;
	lerped_pos.x = ofLerp(get_position().x, pos_to_lerp_to_.x, 0.1f);
	lerped_pos.y = ofLerp(get_position().y, pos_to_lerp_to_.y, 0.1f);

	set_position(lerped_pos);
}

void Camera::handle_scale()
{
	const float mult = ofGetHeight() / 1080.0f;
	cam_.setScale(get_scale() / mult, get_scale() / mult, 1);
	
	handle_bounds();
	
	if (lerping_scale_)
	{
		lerp_scale();
	}
}

void Camera::handle_bounds()
{
	// handle camera zooming in/out
	if (zooming_in_)
	{
		if (get_scale() > zoom_scale_lower_bound_)
		{
			set_scale(get_scale() - keyboard_zooming_speed_);
		}
	}
	else if (zooming_out_)
	{
		if (get_scale() < zoom_scale_upper_bound_)
		{
			set_scale(get_scale() + keyboard_zooming_speed_);
		}
	}
}

void Camera::lerp_scale()
{
	// if difference between lerped value and actual value < 0.001f then lerping will stop, otherwise it will continue indefinitely
	if (abs(get_scale() - scale_to_lerp_to_) < 0.001f)
	{
		set_scale(scale_to_lerp_to_);
		lerping_scale_ = false;
	}
	else if (static_cast<float>(get_scale()) != static_cast<float>(scale_to_lerp_to_))
	{
		const float lerped_scale = ofLerp(get_scale(), scale_to_lerp_to_, 0.2f);

		set_scale(lerped_scale);
	}
}

void Camera::set_zoom_mode(const Cam_modes_ view)
{
	if (view_ == Cam_modes_::player_view)
	{
		prev_player_view_scale_ = get_scale();

		scale_to_lerp_to_ = 2.9f;
		lerping_scale_ = true;

		follow_player_ = false;
	}
	else
	{
		scale_to_lerp_to_ = prev_player_view_scale_;
		lerping_scale_ = true;

		follow_player_ = true;
	}

	view_ = view;
}

void Camera::toggle_zoom_mode()
{
	// toggle between 'player view' and 'map view' camera perspectives
	if (view_ == Cam_modes_::player_view)
	{
		set_zoom_mode(Cam_modes_::map_view);
	}
	else
	{
		set_zoom_mode(Cam_modes_::player_view);
	}
}

void Camera::draw() const
{
	cam_.drawFrustum();
}

void Camera::key_pressed(const int key)
{
	if (key == 'q')
	{
		zooming_out_ = true;
	}
	else if (key == 'e')
	{
		zooming_in_ = true;
	}
	else if (key == 'r')
	{
		if (view_ == Cam_modes_::player_view)
		{
			scale_to_lerp_to_ = 1;
			lerping_scale_ = true;
		}
	}
	else if (key == 3682) // ctrl
	{		
		ctrl_down_ = true;
	}
	else if (key == 9) // tab
	{		
		toggle_zoom_mode();
	}
}

void Camera::key_released(const int key)
{
	if (key == 'q')
	{
		zooming_out_ = false;
	}
	else if (key == 'e')
	{
		zooming_in_ = false;
	}
	else if (key == 3682) // ctrl
	{		
		ctrl_down_ = false;
	}	
}

void Camera::mouse_dragged(const int x, const int y, const int button)
{
	if (button == 1)
	{
		view_ = Cam_modes_::free_view;
		pos_to_lerp_to_ = get_position() + drag_pos_start_ - ofVec2f(x, y);
		lerping_position_ = true;
		set_position(get_position() + drag_pos_start_ - ofVec2f(x, y));
		drag_pos_start_ = ofVec2f(x, y);
	}
}

void Camera::mouse_pressed(const int x, const int y, const int button)
{
	if (button == 1)
	{
		drag_pos_start_ = ofVec2f(x, y);
	}
}

void Camera::mouse_scrolled(int x, int y, const float scroll_x, const float scroll_y)
{
	//if (ctrl_down_)
	{
		// if within bounds
		if (get_scale() - scroll_y / scrollwheel_zooming_intervals_ > zoom_scale_lower_bound_ && get_scale() - scroll_y / scrollwheel_zooming_intervals_ < zoom_scale_upper_bound_)
		{
			scale_to_lerp_to_ = get_scale() - scroll_y / scrollwheel_zooming_intervals_;
			lerping_scale_ = true;

			//cout << "Zoom level: " << scale_to_lerp_to_ << endl;
		}
		else
		{
			if (get_scale() - scroll_y / scrollwheel_zooming_intervals_ < zoom_scale_lower_bound_)
			{
				scale_to_lerp_to_ = zoom_scale_lower_bound_;
				lerping_scale_ = true;
			}
			else if (get_scale() - scroll_y / scrollwheel_zooming_intervals_ > zoom_scale_upper_bound_)
			{
				scale_to_lerp_to_ = zoom_scale_upper_bound_;
				lerping_scale_ = true;
			}
		}
	}
}

void Camera::mouse_released(const int x, const int y, const int button)
{
}



ofVec3f Camera::get_local_mouse_pos() const
{
	return local_mouse_pos_;
}

ofVec3f Camera::get_world_mouse_pos() const
{
	return world_mouse_pos_;
}

glm::mat4 Camera::get_model_view_matrix() const
{
	return cam_.getModelViewMatrix();
}

glm::mat4 Camera::get_local_transform_matrix() const
{
	return cam_.getLocalTransformMatrix();
}

glm::mat4 Camera::get_global_transform_matrix() const
{
	return cam_.getGlobalTransformMatrix();
}

glm::mat4 Camera::get_projection_matrix() const
{
	return cam_.getProjectionMatrix();
}

glm::mat4 Camera::get_model_view_projection_matrix() const
{
	return cam_.getModelViewProjectionMatrix();
}

ofVec2f Camera::get_position() const
{
	return position_;
}

void Camera::set_position(const ofVec2f pos)
{
	position_ = pos;
}

float Camera::get_scale() const
{
	return scale_;
}

void Camera::set_scale(const float scale)
{
	scale_ = scale;
}

void Camera::begin()
{
	cam_.begin();
}

void Camera::end()
{
	cam_.end();
}

ofVec3f Camera::screen_to_world(const ofVec3f view) const
{
	return cam_.screenToWorld(view);
}

ofVec3f Camera::world_to_screen(const ofVec3f view) const
{
	return cam_.worldToScreen(view);
}