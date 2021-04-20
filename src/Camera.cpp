#include "Camera.h"

Camera::Camera(): follow_player_(true)
{
	cam_.disableMouseInput();
	cam_.setVFlip(true);
	// flipping the axis so it's as if we're in 2D, otherwise all the fluid/movement code will be fucked
	cam_.enableOrtho();
	//cam.setScale(1);
	cam_.setPosition(0 + (ofGetWidth() / 2), 0 + (ofGetHeight() / 2), 1305);

	//zoomDistance = 1080.0f; 

	cam_.removeAllInteractions();
	//cam.addInteraction(ofEasyCam::TRANSFORM_SCALE, OF_MOUSE_BUTTON_RIGHT);
	cam_.addInteraction(ofEasyCam::TRANSFORM_TRANSLATE_Z, OF_MOUSE_BUTTON_RIGHT);

	//cam.setNearClip(-1000000);
	//cam.setFarClip(1000000);
}

void Camera::update(const int world_width, const int world_height, const ofVec2f player_pos)
{
	if (follow_player_) {
		cam_.setPosition(player_pos.x, player_pos.y, cam_.getPosition().z); // camera follows player
	}
	else {
		cam_.setPosition(0 + (world_width / 2), 0 + (world_height / 2), cam_.getPosition().z); // camera follows player
	}
}

void Camera::toggle_zoom_mode()
{
	// reset scale/zoom
	cout << "Zoom reset" << endl;
	if ((cam_.getScale().x == 1) && (cam_.getScale().y == 1) && (cam_.getScale().z == 1)) {
		cam_.setScale(2.9f, 2.9f, 1);
		follow_player_ = false;
	}
	else {
		cam_.setScale(1);
		follow_player_ = true;
	}
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

ofVec3f Camera::get_position() const
{
	return cam_.getPosition();
}

glm::vec3 Camera::get_scale() const
{
	return cam_.getScale();
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

void Camera::draw() const
{
	cam_.drawFrustum();
}

void Camera::key_pressed(const int key)
{
	if (key == 'z') {
		toggle_zoom_mode();
	}
	else if (key == 3682) {
		ctrl_down_ = true;
	}
}

void Camera::key_released(const int key)
{
	if (key == 3682) {
		ctrl_down_ = false;
	}
}

void Camera::mouse_dragged(int x, int y, int button)
{

}

void Camera::mouse_pressed(int x, int y, int button)
{

}

void Camera::mouse_scrolled(int x, int y, const float scroll_x, const float scroll_y)
{
	if (ctrl_down_) {
		cam_.setScale(cam_.getScale().x + scroll_y / 10, cam_.getScale().y + scroll_y / 10, 1);
		cout << "Zoom level: " << cam_.getScale() << endl;
	}
}