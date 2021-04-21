#pragma once

#include "ofMain.h"
#include "Controller.h"

class Camera {

public:
	
	Camera();
	
	void update(ofVec2f player_pos);

	ofVec3f get_local_mouse_pos() const;
	ofVec3f get_world_mouse_pos() const;
	
	void toggle_zoom_mode();

	glm::mat4 get_local_transform_matrix() const;
	glm::mat4 get_global_transform_matrix() const;
	glm::mat4 get_projection_matrix() const;
	glm::mat4 get_model_view_matrix() const;
	glm::mat4 get_model_view_projection_matrix() const;
	ofVec3f get_position() const;
	glm::vec3 get_scale() const;
	void begin();
	void end();
	ofVec3f screen_to_world(ofVec3f view) const;

	void draw() const;
	
	void key_pressed(int key);
	void key_released(int key);
	void mouse_dragged(int x, int y, int button);
	void mouse_pressed(int x, int y, int button);
	void mouse_scrolled(int x, int y, float scroll_x, float scroll_y);

private:

	ofEasyCam cam_;
	bool ctrl_down_ = false;
	bool follow_player_;
	ofVec3f local_mouse_pos_;
	ofVec3f world_mouse_pos_;
	
};