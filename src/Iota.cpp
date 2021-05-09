#include "Iota.h"

void Iota::setup(ofBaseApp* app_ptr)
{
	//ofSetWindowPosition(3849, 649);
	ofSetWindowShape(1920, 1080);
	ofSetWindowPosition((ofGetScreenWidth() - ofGetWindowWidth()) / 2, (ofGetScreenHeight() - ofGetWindowHeight()) / 2);
	ofSetWindowTitle("iota");
	ofSetCircleResolution(176);
	ofBackground(0);
	ofSetVerticalSync(true);
	window_resized(ofGetWidth(), ofGetHeight());
	ofEnableAlphaBlending();
	ofSetBackgroundAuto(true);

	entity_manager.init(&game_controller, &gui_manager, &cam, &fluid_manager, &audio_manager, &gamemode_manager);
	scene_manager.init(&game_controller, &gui_manager, &cam, &fluid_manager, &audio_manager, &entity_manager, &gamemode_manager);
	gui_manager.init(&game_controller, &fluid_manager, &audio_manager, &cam);

	event_manager.init(&entity_manager, &gui_manager);
	event_manager.show_tutorial(false);
	event_manager.setup();

	audio_manager.setup(app_ptr);

	gamemode_manager.init(&gui_manager);
	
	//scene_manager.loadScene("Scenes/StartingScene");
	scene_manager.load_procedural_scene();
}

void Iota::audio_out(float* output, const int buffer_size, const int n_channels)
{
	//audio_manager.audioOut(output, buffer_size, n_channels);
}

void Iota::update()
{
	cam.update(entity_manager.get_player_position());
	entity_manager.update();
	gui_manager.update();
	event_manager.update();
	fluid_manager.update();
	audio_manager.update();
	scene_manager.update();
}

void Iota::draw()
{	
	cam.begin();

	fluid_manager.render_fluid();
	fluid_manager.render_particles(entity_manager.get_player_position());

	//audio_manager.draw(); // background animation effect

	ofPushMatrix();

	ofTranslate(HALF_WORLD_WIDTH, HALF_WORLD_HEIGHT);
	entity_manager.draw_game_objects();
	event_manager.draw_tutorial();

	ofPopMatrix();

	cam.end();

	if (entity_manager.get_selected_game_object() != nullptr && entity_manager.get_selected_game_object()->get_type() == "Spring") {
		gui_manager.draw_required_gui(entity_manager.get_selected_game_object(), entity_manager.get_new_node_type(), gamemode_manager.get_current_mode_string());
	}
	else {
		gui_manager.draw_required_gui(entity_manager.get_selected_game_object(), entity_manager.get_new_node_type(), gamemode_manager.get_current_mode_string());
	}
}

void Iota::key_pressed(const int key)
{
	audio_manager.keyPressed(key);

	cam.key_pressed(key);
	fluid_manager.key_pressed(key);
	event_manager.key_pressed(key);
	scene_manager.key_pressed(key);
	gamemode_manager.key_pressed(key);
	fluid_manager.key_pressed(key);
	gui_manager.key_pressed(key);

	if (event_manager.is_event_allowed("key_pressed")) {
		entity_manager.key_pressed(key);
	}

	if (key == 'f') {
		ofToggleFullscreen();
	}
}

void Iota::key_released(const int key)
{
	audio_manager.keyPressed(key);
	cam.key_released(key);

	if (event_manager.is_event_allowed("key_released")) {
		entity_manager.key_released(key);
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
	if (event_manager.is_event_allowed("mouse_pressed", button)) {
		entity_manager.mouse_pressed(x, y, button);
		cam.mouse_pressed(x, y, button);
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
}

void Iota::drag_event(ofDragInfo drag_info)
{
}

void Iota::got_message(ofMessage msg)
{
}
