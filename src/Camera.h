#pragma once

#include "ofMain.h"
#include "Controller.h"

class Camera {

public:
	
	Camera();

	// Updated
	void update(ofVec2f player_pos);
	void calculate_mouse_coords();
	void follow_player(ofVec2f player_pos);

	void handle_position(ofVec2f player_pos);

	void handle_scale();
	void handle_bounds();
	void lerp_scale();
	void lerp_position();

	// Helpers
	void toggle_zoom_mode();

	void begin();
	void end();
	
	ofVec3f screen_to_world(ofVec3f view) const;
	ofVec3f world_to_screen(ofVec3f view) const;

	ofVec3f get_local_mouse_pos() const;
	ofVec3f get_world_mouse_pos() const;
	
	glm::mat4 get_local_transform_matrix() const;
	glm::mat4 get_global_transform_matrix() const;
	glm::mat4 get_projection_matrix() const;
	glm::mat4 get_model_view_matrix() const;
	glm::mat4 get_model_view_projection_matrix() const;
	
	ofVec2f get_position() const;
	void set_position(ofVec2f pos);
	float get_scale() const;
	void set_scale(float scale);

	// Draw Frustram
	void draw() const;

	// Events
	void key_pressed(int key);
	void key_released(int key);
	void mouse_dragged(int x, int y, int button);
	void mouse_pressed(int x, int y, int button);
	void mouse_scrolled(int x, int y, float scroll_x, float scroll_y);
	void mouse_released(int x, int y, int button);

private:

	ofEasyCam cam_;	

	enum Cam_modes_ { player_view, map_view };
	Cam_modes_ view_;

	ofVec2f position_;
	float scale_;
	
	bool ctrl_down_ = false;
	
	bool follow_player_;
	float prev_player_view_scale_;
	
	ofVec3f local_mouse_pos_;
	ofVec3f world_mouse_pos_;
	
	bool zooming_out_;
	bool zooming_in_;
	
	float keyboard_zooming_speed_;
	float scrollwheel_zooming_intervals_;
	
	float zoom_scale_upper_bound_;
	float zoom_scale_lower_bound_;
	
	bool lerping_scale_;
	float scale_to_lerp_to_;

	bool lerping_position_;
	ofVec2f pos_to_lerp_to_;

	bool drag_started_{ false };
	ofVec2f drag_pos_start_{};
	ofVec2f drag_pos_end_{};
	
};