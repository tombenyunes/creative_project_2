#pragma once

#include "AudioManager.h"
#include "Camera.h"
#include "Controller.h"
#include "EntityManager.h"
#include "EventManager.h"
#include "FluidManager.h"
#include "GamemodeManager.h"
#include "GUIManager.h"
#include "SceneManager.h"

class Iota
{
public:

	void setup(ofBaseApp* app_ptr);

	void audio_out(float* output, int buffer_size, int n_channels);

	void update();

	void draw();

	void key_pressed(int key);
	void key_released(int key);
	void mouse_moved(int x, int y);
	void mouse_dragged(int x, int y, int button);
	void mouse_pressed(int x, int y, int button);
	void mouse_scrolled(int x, int y, float scroll_x, float scroll_y);
	void mouse_released(int x, int y, int button);
	void mouse_entered(int x, int y);
	void mouse_exited(int x, int y);
	void window_resized(int w, int h);
	void drag_event(ofDragInfo drag_info);
	void got_message(ofMessage msg);

	EntityManager entity_manager;
	Controller game_controller;
	GUIManager gui_manager;
	SceneManager scene_manager;
	GamemodeManager gamemode_manager{ 1 };
	EventManager event_manager;
	AudioManager audio_manager;
	FluidManager fluid_manager;
	Camera cam;
};
