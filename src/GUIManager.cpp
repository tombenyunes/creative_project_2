#include "GUIManager.h"

#include "GameObject.h"

GUIManager::GUIManager()
	:	gui_visible_(false)
	,	panel_pixel_buffer_(10)
	,	draw_audio_gui_(false)   // buffer between all panels and each other + screen edges
	,	multi_node_selected_(false)
	,	points_collected_(0)
	,	max_point_count_(0)
	,	delete_all_(false)
{
	const string error_message = "Error: Updating failed"; // error message will show for all parameters that require but haven't received an update
	const int error_int = 404;

	const ofVec2f k_bounds = { 0.1f, 20.0f };
	const ofVec2f damping_bounds = { 0.1f, 8.0f };
	ofVec2f springmass_bounds = { 0.1f, 50.0f };

	gui_world_delete_all.addListener(this, &GUIManager::toggle_delete_all);

	// Global
	panel_world.setup("World", "", panel_pixel_buffer_, panel_pixel_buffer_);
	panel_world.add(gui_world_delete_all.setup("clear all"));
	panel_world.add(gui_world_gravity.setup("global gravity", false));
	panel_world.add(gui_world_hard_collisions.setup("hard collisions", false));
	panel_world.add(gui_world_calculate_fluid.setup("calculate fluid", true));
	panel_world.add(gui_world_calculate_particles.setup("calculate particles", true));
	panel_world.add(gui_world_calculate_entities.setup("calculate entities", true));
	panel_world.add(gui_world_fps.setup("FPS", error_message));
	panel_world.add(gui_world_frametime.setup("Frametime", error_message));
	
	// Player
	panel_player.setup("Player", "", panel_pixel_buffer_, panel_world.getPosition().y + panel_world.getHeight() + panel_pixel_buffer_);
	panel_player.add(gui_player_pos.setup("pos", error_message));
	panel_player.add(gui_player_vel.setup("vel", error_message));
	panel_player.add(gui_player_accel.setup("acceleration", error_message));
	panel_player.add(gui_player_mass.setup("mass", error_int, MINIMUM_MASS, MAXIMUM_MASS));
	panel_player.add(gui_player_infinite_mass.setup("infinite mass", false));
	panel_player.add(gui_player_radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));

	// Fluid
	panel_fluid.setup("FluidManager", "", panel_pixel_buffer_, panel_player.getPosition().y + panel_player.getHeight() + panel_pixel_buffer_);
	panel_fluid.add(gui_fluid_cells.setup("fluid cells", 100, 20, 400));
	panel_fluid.add(gui_fluid_resize_fluid.setup("resize"));
	panel_fluid.add(gui_fluid_color_mult.setup("color mult", 100.0f, 0.0f, 100.0f));
	panel_fluid.add(gui_fluid_velocity_mult.setup("velocity mult", 7.0f, 0.0f, 100.0f));
	panel_fluid.add(gui_fluid_viscocity.setup("viscocity", 0.00015f, 0.0f, 0.01f));
	panel_fluid.add(gui_fluid_color_diffusion.setup("color diffusion", 0.0f, 0.0f, 0.0003f));
	panel_fluid.add(gui_fluid_fade_speed.setup("fade speed", 0.002f, 0.0f, 0.1f));
	panel_fluid.add(gui_fluid_solver_iterations.setup("solver interations", 10, 0 /*1*/, 50));
	panel_fluid.add(gui_fluid_delta_t.setup("delta t", 0.1f, 0.0f, 5.0f));
	panel_fluid.add(gui_fluid_draw_mode.setup("draw mode", 1, 0, 3));
	panel_fluid.add(gui_fluid_do_rgb.setup("rgb", true));
	panel_fluid.add(gui_fluid_do_vorticity_confinement.setup("vorticity confinement", false));
	panel_fluid.add(gui_fluid_draw_fluid.setup("draw fluid", true));
	panel_fluid.add(gui_fluid_draw_particles.setup("draw particles", true));
	panel_fluid.add(gui_fluid_vel_draw_mult.setup("vel draw mult", 1.0f, 0.0f, 20.0f));
	panel_fluid.add(gui_fluid_vel_draw_threshold.setup("vel draw threshold", 0.0f, 0.0f, 1.0f));
	panel_fluid.add(gui_fluid_brightness.setup("brightness", 1.0f, 0.0f, 2.0f));
	panel_fluid.add(gui_fluid_use_additive_blending.setup("additive blending", false));
	panel_fluid.add(gui_fluid_wrap_x.setup("wrap x", false));
	panel_fluid.add(gui_fluid_wrap_y.setup("wrap y", false));
	panel_fluid.add(gui_fluid_tuio_x_scaler.setup("tuio x scaler", 1.0f, 0.0f, 2.0f));
	panel_fluid.add(gui_fluid_tuio_y_scaler.setup("tuio y scaler", 1.0f, 0.0f, 2.0f));
	
	// Mass
	panel_node.setup("Node", "", ofGetWidth() - panel_node.getWidth() - panel_pixel_buffer_, panel_pixel_buffer_);
	panel_node.add(gui_node_pos.setup("pos", error_message));
	panel_node.add(gui_node_vel.setup("vel", error_message));
	panel_node.add(gui_node_accel.setup("acceleration", error_message));
	panel_node.add(gui_node_mass.setup("mass", error_int, MINIMUM_MASS, MAXIMUM_MASS));
	panel_node.add(gui_node_infinite_mass.setup("infinite mass", false));
	panel_node.add(gui_node_radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));

	// Mass
	panel_collectable.setup("Collectable", "", ofGetWidth() - panel_node.getWidth() - panel_pixel_buffer_, panel_pixel_buffer_);
	panel_collectable.add(gui_collectable_pos.setup("pos", error_message));
	panel_collectable.add(gui_collectable_radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));
	panel_collectable.add(gui_collectable_emission_frequency.setup("emission Frequency", error_int, 15, 150));
	panel_collectable.add(gui_collectable_emission_force.setup("emission Force", error_int, 0.1f, 150));	
	panel_collectable.add(gui_collectable_is_active.setup("active", true));
	panel_collectable.add(gui_collectable_id.setup("id", error_int));

	// Spring Settings
	panel_spring_settings.setup("Spring Settings", "", ofGetWidth() - panel_spring_settings.getWidth() - panel_pixel_buffer_, panel_pixel_buffer_);
	panel_spring_settings.add(gui_spring_anchor_pos.setup("anchor pos", error_message));
	panel_spring_settings.add(gui_spring_k.setup("springiness", error_int, k_bounds.x, k_bounds.y));
	panel_spring_settings.add(gui_spring_damping.setup("damping", error_int, damping_bounds.x, damping_bounds.y));
	panel_spring_settings.add(gui_spring_springmass.setup("springmass", error_int, MINIMUM_MASS, MAXIMUM_MASS));
	panel_spring_settings.add(gui_spring_add_node.setup("add node"));
	
	// Spring Node
	panel_spring_node.setup("Selected Object", "", ofGetWidth() - panel_spring_settings.getWidth() - panel_pixel_buffer_, panel_spring_settings.getPosition().y + panel_spring_settings.getHeight() + panel_pixel_buffer_);
	panel_spring_node.add(gui_spring_node_pos.setup("pos", error_message));
	panel_spring_node.add(gui_spring_node_vel.setup("vel", error_message));
	panel_spring_node.add(gui_spring_node_accel.setup("acceleration", error_message));
	panel_spring_node.add(gui_spring_node_mass.setup("mass", error_int, MINIMUM_MASS, MAXIMUM_MASS / 10));
	panel_spring_node.add(gui_spring_node_radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));



	potta_one_title_.load("Fonts/PottaOne-Regular.ttf", 96, true, true);
	potta_one_main_.load("Fonts/PottaOne-Regular.ttf", 24, true, true);
	potta_one_sub_.load("Fonts/PottaOne-Regular.ttf", 16, true, true);
	window_resized();
}

void GUIManager::init(Controller* controller, AudioManager* audio_manager, Camera* cam)
{
	game_controller_ = controller;
	audio_manager_ = audio_manager;
	cam_ = cam;
}

void GUIManager::update()
{
	update_world();
}

void GUIManager::update_world()
{
	game_controller_->set_gravity(gui_world_gravity);
	game_controller_->set_use_hard_collisions(gui_world_hard_collisions);
	
	gui_world_fps = ofToString(ofGetFrameRate());
	gui_world_frametime = ofToString(ofGetLastFrameTime());
}


int GUIManager::get_max_point_count()
{
	return max_point_count_;
}

void GUIManager::set_max_point_count(const int count)
{
	max_point_count_ = count;
}

int GUIManager::get_point_count()
{
	return points_collected_;
}

void GUIManager::set_point_count(const int count)
{
	points_collected_ = count;
}


void GUIManager::reset_point_counters()
{
	max_point_count_ = 0;
	points_collected_ = 0;
}

int GUIManager::get_delete_all() const
{
	return delete_all_;
}

void GUIManager::toggle_delete_all()
{
	(delete_all_ == 0) ? delete_all_ = true : delete_all_ = false;
}

void GUIManager::set_gui_visible(const bool value)
{
	(value == 1) ? gui_visible_ = true : gui_visible_ = false;
}

bool GUIManager::get_gui_visible() const
{
	return gui_visible_;
}



void GUIManager::update_player_values(const ofVec2f pos, const ofVec2f vel, const ofVec2f accel, const float mass, const bool infmass, const float radius)
{
	gui_player_pos = ofToString(roundf(pos.x)) + ", " + ofToString(roundf(pos.y));
	gui_player_vel = ofToString(roundf(vel.x * 100) / 100) + ", " + ofToString(roundf(vel.y * 100) / 100);
	gui_player_accel = ofToString(roundf(accel.x * 10000) / 10000) + ", " + ofToString(roundf(accel.y * 10000) / 10000);
	if (infmass)
	{
		gui_player_mass.setTextColor(0);
		gui_player_infinite_mass = true;
	}
	else
	{
		gui_player_mass.setTextColor(255);
		gui_player_mass = mass;
		gui_player_infinite_mass = false;
	}
	gui_player_radius = radius;
}

void GUIManager::update_mass_values(const ofVec2f pos, const ofVec2f vel, const ofVec2f accel, const float mass, const bool infmass, const float radius)
{
	panel_node.setName("Mass");
	panel_node.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + pos.x + 8, HALF_WORLD_HEIGHT + pos.y + 8)));

	gui_node_pos = ofToString(roundf(pos.x)) + ", " + ofToString(roundf(pos.y));
	gui_node_vel = ofToString(roundf(vel.x * 100) / 100) + ", " + ofToString(roundf(vel.y * 100) / 100);
	gui_node_accel = ofToString(roundf(accel.x * 10000) / 10000) + ", " + ofToString(roundf(accel.y * 10000) / 10000);
	if (infmass)
	{
		gui_node_mass.setTextColor(0);
		gui_node_infinite_mass = true;
	}
	else
	{
		gui_node_mass.setTextColor(255);
		gui_node_mass = mass;
		gui_node_infinite_mass = false;
	}
	gui_node_radius = radius;
}

void GUIManager::update_collectable_values(const ofVec2f pos, const ofVec2f vel, const ofVec2f accel, const float radius, const float emission_frequency, const float emission_force, const bool is_active, const int id)
{
	panel_collectable.setName("Collectable");
	panel_collectable.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + pos.x + 8, HALF_WORLD_HEIGHT + pos.y + 8)));

	gui_collectable_pos = ofToString(roundf(pos.x)) + ", " + ofToString(roundf(pos.y));
	gui_collectable_radius = radius;
	gui_collectable_emission_frequency = emission_frequency;
	gui_collectable_emission_force = emission_force;
	gui_collectable_is_active = is_active;
	gui_collectable_id = id;
}



void GUIManager::update_spring_values(const ofVec2f anchor_position, const float k, const float damping, const float springmass, const ofVec2f selected_node_pos, const ofVec2f selected_node_vel, const ofVec2f selected_node_accel, const float selected_node_mass, const float selected_node_radius)
{
	panel_spring_settings.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + anchor_position.x + 8, HALF_WORLD_HEIGHT + anchor_position.y + 8)));
	panel_spring_node.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + selected_node_pos.x + 8, HALF_WORLD_HEIGHT + selected_node_pos.y + 8)));
	
	gui_spring_anchor_pos = ofToString(roundf(anchor_position.x)) + ", " + ofToString(roundf(anchor_position.y));

	gui_spring_k = k;
	gui_spring_damping = damping;
	gui_spring_springmass = springmass;
	
	gui_spring_node_pos = ofToString(roundf(selected_node_pos.x)) + ", " + ofToString(roundf(selected_node_pos.y));
	gui_spring_node_vel = ofToString(roundf(selected_node_vel.x * 100) / 100) + ", " + ofToString(roundf(selected_node_vel.y * 100) / 100);
	gui_spring_node_accel = ofToString(roundf(selected_node_accel.x * 10000) / 10000) + ", " + ofToString(roundf(selected_node_accel.y * 10000) / 10000);
	gui_spring_node_mass = selected_node_mass;
	gui_spring_node_radius = selected_node_radius;

	multi_node_selected_ = true;
}

void GUIManager::update_spring_values(const ofVec2f anchor_position, const float k, const float damping, const float springmass)
{
	panel_spring_settings.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + anchor_position.x + 8, HALF_WORLD_HEIGHT + anchor_position.y + 8)));

	gui_spring_anchor_pos = ofToString(roundf(anchor_position.x)) + ", " + ofToString(roundf(anchor_position.y));

	gui_spring_k = k;
	gui_spring_damping = damping;
	gui_spring_springmass = springmass;
	
	multi_node_selected_ = false;
}



void GUIManager::draw_required_gui(GameObject* selected_object, const int new_node_id, const string current_gamemode)
{
	if (current_gamemode == "Menu")
	{
		draw_menu();
	}
	else
	{
		draw_text(new_node_id, current_gamemode);
		
		if (current_gamemode == "Sandbox")
		{			
			draw_border();
		}

		

		if (get_gui_visible() /*|| Event_Manager->playerGUIVisible*/)
		{	
			if (selected_object != nullptr)
			{
				if (selected_object->get_type() == "Mass")
				{
					panel_node.draw();
				}
				else if (selected_object->get_type() == "Spring")
				{
					// if an object is a spring then it has multiple gui windows to draw					
					if (multi_node_selected_ == true)
					{
						panel_spring_node.draw();
					}
					else
					{
						panel_spring_settings.draw();
					}
				}
				else if (selected_object->get_type() == "Collectable")
				{
					panel_collectable.draw();
				}
			}
			panel_world.draw();
			panel_player.draw();
			panel_fluid.draw();
		}
		else
		{
			audio_manager_->drawGUI(draw_audio_gui_);
		}
	}
}

void GUIManager::draw_text(const int new_node_id, const string current_gamemode) const
{
	string entity_type;
	switch (new_node_id)
	{
	case 0:
		entity_type = "Mass";
		break;
	case 1:
		entity_type = "Spring";
		break;
	case 2:
		entity_type = "Collectable";
		break;
	default:
		cout << "Error -> GUIManager.cpp::update_create_node_id -> New Node Type Not Specified" << endl;
		break;
	}

	if (current_gamemode == "Sandbox")
	{
		ofDrawBitmapString("Entity Type: " + entity_type, glm::vec2((ofGetWidth() / 2) - 100, ofGetHeight() - 150));
	}
	else if (current_gamemode == "Procedural")
	{
		if (points_collected_ == max_point_count_)
		{
			potta_one_sub_.drawString("Press 'Enter' to move to next level", (ofGetWidth() / 2) - potta_one_sub_.stringWidth("Press 'Enter' to move to next level") / 2, ofGetHeight() - 150);
			static bool init = false;
			if (!init)
			{
				init = true;
				cam_->toggle_zoom_mode();
			}
		}
	}
	
	//ofDrawBitmapString("GameMode: " + current_gamemode, glm::vec2((ofGetWidth() / 2) - 100, ofGetHeight() - 100));
	//ofDrawBitmapString("Collectables Found: " + to_string(points_collected_) + " / " + to_string(max_point_count_), glm::vec2((ofGetWidth() / 2) - 100, ofGetHeight() - 50));
}

void GUIManager::draw_border() const
{
	ofPushStyle();
	ofPushMatrix();

	ofSetColor(255);
	ofNoFill();
	ofSetLineWidth(4);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofPopMatrix();
	ofPopStyle();
}

void GUIManager::draw_menu() const
{
	ofPushStyle();
	ofPushMatrix();

	ofSetColor(0, 0, 0, 125);
	ofFill();
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());	
	
	ofSetColor(255);
	const float w = ofGetWidth() / 2;
	const float h = ofGetHeight() / 2;
	const float v_buf = 48;
	/*ofDrawBitmapString("Menu", w, h + 16 * 1);
	ofDrawBitmapString("'Left Click' to exert a force in a direction", w, h + 16 * 2);
	ofDrawBitmapString("'Space' to toggle sandbox mode", w, h + 16 * 3);
	ofDrawBitmapString("'Right Click' to select and move entities", w, h + 16 * 4);
	ofDrawBitmapString("'F5' to quick-save a scene", w, h + 16 * 5);
	ofDrawBitmapString("'F9' to quick-load a scene", w, h + 16 * 6);
	ofDrawBitmapString("'c' to create an entity", w, h + 16 * 7);
	ofDrawBitmapString("'x' to delete an entity", w, h + 16 * 8);
	ofDrawBitmapString("'Scroll Wheel' to zoom", w, h + 16 * 9);
	ofDrawBitmapString("'Tab' to toggle perspectives", w, h + 16 * 10);*/

	potta_one_title_.drawString("Iota", w - potta_one_title_.stringWidth("Iota") / 2, h - (v_buf * 1));
	
	if (main_mode_bounds.intersects(ofRectangle(ofGetMouseX(), ofGetMouseY(), 0, 0)))
		ofSetColor(155);
	else
		ofSetColor(255);
	potta_one_main_.drawString("Start", w - potta_one_main_.stringWidth("Start") / 2, h + (v_buf * 1));
	
	if (sandbox_mode_bounds.intersects(ofRectangle(ofGetMouseX(), ofGetMouseY(), 0, 0)))
		ofSetColor(155);
	else
		ofSetColor(255);
	potta_one_main_.drawString("Sandbox Mode", w - potta_one_main_.stringWidth("Sandbox Mode") / 2, h + (v_buf * 2));
	
	ofPopMatrix();
	ofPopStyle();
}

void GUIManager::window_resized()
{
	const float w = ofGetWidth() / 2;
	const float h = ofGetHeight() / 2;
	const float v_buf = 48;
	main_mode_bounds = potta_one_main_.getStringBoundingBox("Start", w - potta_one_main_.stringWidth("Start") / 2, h + (v_buf * 1));
	sandbox_mode_bounds = potta_one_main_.getStringBoundingBox("Sandbox Mode", w - potta_one_main_.stringWidth("Sandbox Mode") / 2, h + (v_buf * 2));
}

void GUIManager::key_pressed(const int key)
{
	if (key == 57344) // f1
	{		
		if (get_gui_visible())
		{
			set_gui_visible(false);
		}
		else
		{
			set_gui_visible(true);
			draw_audio_gui_ = false;
		}
	}
	else if (key == 57346) // f3
	{		
		if (draw_audio_gui_)
		{
			draw_audio_gui_ = false;
		}
		else
		{
			set_gui_visible(false);
			draw_audio_gui_ = true;
		}
	}
	else if (key == 'g')
	{
		gui_fluid_viscocity = 0;
	}
	else
	{
		gui_fluid_viscocity = 0.00015f;
	}
}
