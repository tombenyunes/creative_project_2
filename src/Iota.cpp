#include "Iota.h"

void Iota::setup(ofBaseApp* app_ptr)
{
	ofSetWindowTitle("iota");
	ofSetCircleResolution(176);
	ofBackground(0);
	ofSetVerticalSync(true);
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);
	ofSetEscapeQuitsApp(false);
	window_resized(ofGetWidth(), ofGetHeight());

	event_manager.init(&entity_manager, &gui_manager, &gamemode_manager);
	gamemode_manager.init(&gui_manager);
	scene_manager.init(&game_controller, &gui_manager, &cam, &fluid_manager, &audio_manager, &entity_manager, &gamemode_manager);
	entity_manager.init(&game_controller, &gui_manager, &cam, &fluid_manager, &audio_manager, &gamemode_manager);
	fluid_manager.init(&gui_manager);	
	audio_manager.setup(app_ptr);
	gui_manager.init(&game_controller, &audio_manager, &cam);
	
	scene_manager.load_scene("Scenes/menu_scene.xml");
}

void Iota::audio_out(float* output, const int buffer_size, const int n_channels)
{
	audio_manager.audioOut(output, buffer_size, n_channels);
}

void Iota::update()
{
	event_manager.update();
	gamemode_manager.update();
	scene_manager.update();
	entity_manager.update();
	fluid_manager.update();
	audio_manager.update(entity_manager.get_player()->get_position());
	gui_manager.update();
	cam.update(entity_manager.get_player_position());
}

void Iota::draw()
{	
	cam.begin();

	// draw fluid and particle systemS
	fluid_manager.draw(entity_manager.get_player());
	
	// draw all entities
	ofPushMatrix();
	ofTranslate(HALF_WORLD_WIDTH, HALF_WORLD_HEIGHT);
	entity_manager.draw_game_objects();
	ofPopMatrix();

	// gamemode menu + transitions
	gamemode_manager.draw();

	cam.end();

	// gui
	gui_manager.draw_required_gui(entity_manager.get_selected_game_object(), entity_manager.get_new_node_type(), gamemode_manager.get_current_mode_string(), gamemode_manager.get_main_mode_started(), gamemode_manager.get_prev_gamemode());
}

void Iota::key_pressed(const int key)
{
	gamemode_manager.key_pressed(key);
	
	if (event_manager.is_event_allowed("key_pressed")) {
		entity_manager.key_pressed(key);
		audio_manager.keyPressed(key);
		cam.key_pressed(key);
		fluid_manager.key_pressed(key);
		scene_manager.key_pressed(key);		
		fluid_manager.key_pressed(key);
		gui_manager.key_pressed(key);
	}
	
	if (key == 57354) // f11
	{
		ofToggleFullscreen();
	}
}

void Iota::key_released(const int key)
{
	if (event_manager.is_event_allowed("key_released")) {
		entity_manager.key_released(key);
		audio_manager.keyReleased(key);
		cam.key_released(key);
		scene_manager.key_released(key);
	}
}

void Iota::mouse_moved(const int x, const int y)
{
}

void Iota::mouse_dragged(const int x, const int y, const int button)
{
	if (event_manager.is_event_allowed("mouse_dragged", button)) {
		entity_manager.mouse_dragged(x, y, button);
		cam.mouse_dragged(x, y, button);
	}
}

void Iota::mouse_pressed(const int x, const int y, const int button)
{
	gamemode_manager.mouse_pressed(x, y, button);
	
	if (event_manager.is_event_allowed("mouse_pressed", button)) {
		entity_manager.mouse_pressed(x, y, button);
		cam.mouse_pressed(x, y, button);
		audio_manager.mousePressed(x, y, button);
	}
}

void Iota::mouse_scrolled(const int x, const int y, const float scroll_x, const float scroll_y)
{
	if (event_manager.is_event_allowed("mouse_scrolled")) {
		cam.mouse_scrolled(x, y, scroll_x, scroll_y);
	}
}

void Iota::mouse_released(const int x, const int y, const int button)
{
	if (event_manager.is_event_allowed("mouse_released", button)) {
		entity_manager.mouse_released(x, y, button);
		cam.mouse_released(x, y, button);
		audio_manager.mouseReleased(x, y, button);
	}
}

void Iota::mouse_entered(int x, int y)
{
}

void Iota::mouse_exited(int x, int y)
{
}

void Iota::window_resized(int w, int h)
{
	gui_manager.window_resized();
}

void Iota::drag_event(ofDragInfo drag_info)
{
}

void Iota::got_message(ofMessage msg)
{
}
