#include "GUIManager.h"

GUIManager::GUIManager()
	:	multi_node_selected(false),
		point_count_(0),
		points_collected_(0),
		max_point_count_(0),   // buffer between all panels and each other + screen edges
		buffer_(5),
		draw_particle_gui_(false),
		draw_audio_gui_(false)
{
	const string error_message = "Error: Updating failed"; // error message will show for all parameters that require but haven't received an update
	const int error_int = 404;

	const ofVec2f k_bounds = { 0.1f, 20.0f };
	const ofVec2f damping_bounds = { 0.1f, 8.0f };
	ofVec2f springmass_bounds = { 0.1f, 50.0f };

	new_scene_.addListener(this, &GUIManager::set_clear_all);

	world_gui_.setup("World", "", buffer_, buffer_);
	world_gui_.add(new_scene_.setup("clear all"));
	world_gui_.add(gravity_.setup("global gravity", false));
	world_gui_.add(hard_collisions_.setup("hard collisions", false));

	player_gui_.setup("Player", "", world_gui_.getPosition().x + world_gui_.getWidth() + buffer_, buffer_);
	player_gui_.add(position.setup("pos", error_message));
	player_gui_.add(velocity.setup("vel", error_message));
	player_gui_.add(acceleration.setup("acceleration", error_message));
	player_gui_.add(mass.setup("mass", error_int, MINIMUM_MASS, MAXIMUM_MASS));
	player_gui_.add(infinite_mass.setup("infinite mass", false));
	player_gui_.add(radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));
	player_gui_.add(affected_by_gravity.setup("gravity", false));

	selected_gui_.setup("Selected Object", "", ofGetWidth() - selected_gui_.getWidth() - buffer_, buffer_);
	selected_gui_.add(selected_position.setup("pos", error_message));
	selected_gui_.add(selected_velocity.setup("vel", error_message));
	selected_gui_.add(selected_accel.setup("acceleration", error_message));
	selected_gui_.add(selected_mass.setup("mass", error_int, MINIMUM_MASS, MAXIMUM_MASS));
	selected_gui_.add(selected_infinite_mass.setup("infinite mass", false));
	selected_gui_.add(selected_radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));
	selected_gui_.add(selected_affected_by_gravity.setup("gravity", false));
	selected_gui_.add(selected_emission_frequency.setup("Emission Frequency", error_int, 15, 150));

	multi_selection_gui_spring_.setup("Spring Settings", "", ofGetWidth() - multi_selection_gui_spring_.getWidth() - buffer_, buffer_);
	multi_selection_gui_spring_.add(anchor_pos.setup("anchor pos", error_message));
	multi_selection_gui_spring_.add(k.setup("springiness", error_int, k_bounds.x, k_bounds.y));
	multi_selection_gui_spring_.add(damping.setup("damping", error_int, damping_bounds.x, damping_bounds.y));
	multi_selection_gui_spring_.add(springmass.setup("springmass", error_int, MINIMUM_MASS, MAXIMUM_MASS));
	multi_selection_gui_spring_.add(spring_affected_by_gravity.setup("gravity", false));

	multi_selection_gui_node_.setup("Selected Object", "", ofGetWidth() - multi_selection_gui_spring_.getWidth() - buffer_, multi_selection_gui_spring_.getPosition().y + multi_selection_gui_spring_.getHeight() + buffer_);
	multi_selection_gui_node_.add(node_pos.setup("pos", error_message));
	multi_selection_gui_node_.add(node_vel.setup("vel", error_message));
	multi_selection_gui_node_.add(node_accel.setup("acceleration", error_message));
	multi_selection_gui_node_.add(node_mass.setup("mass", error_int, MINIMUM_MASS, MAXIMUM_MASS / 10));
	multi_selection_gui_node_.add(node_radius.setup("radius", error_int, RADIUS_MINIMUM, RADIUS_MAXIMUM));

	create_node_gui_.setup("Create", "", ofGetWidth() / 2 - create_node_gui_.getWidth() / 2, buffer_);
	create_node_gui_.add(how_to_move_.setup("", "Hold M1 to Move Player"));
	create_node_gui_.add(how_to_drag_.setup("", "Hold M2 to Drag Node"));
	create_node_gui_.add(how_to_select_.setup("", "Press M2 to Select Node"));
	create_node_gui_.add(how_to_delete_.setup("", "Press 'x' to Delete Node"));
	create_node_gui_.add(how_to_create_.setup("", "Press 'c' to Create Node"));
	create_node_gui_.add(how_to_change_type_.setup("", "MouseWheel Changes Type:"));
	create_node_gui_.add(name_.setup("Type", error_message));
}

void GUIManager::init(Controller* controller, FluidManager* fluid_manager, AudioManager* audio_manager, GamemodeManager* game_mode_manager, Camera* cam)
{
	game_controller_ = controller;
	fluid_manager_ = fluid_manager;
	audio_manager_ = audio_manager;
	game_mode_manager_ = game_mode_manager;

	cam_ = cam;
}

void GUIManager::update()
{	
	update_world();
	update_create_node_id();
}

void GUIManager::update_world()
{
	game_controller_->set_gravity(gravity_);
	game_controller_->set_use_hard_collisions(hard_collisions_);
}

void GUIManager::update_create_node_id()
{
	switch (game_controller_->get_new_node_type())
	{
	case 0:
		name_ = "Mass";
		break;
	case 1:
		name_ = "Spring";
		break;
	case 2:
		name_ = "Collectable";
		break;
	default:
		cout << "Error -> GUIManager.cpp::update_create_node_id -> New Node Type Not Specified" << endl;
		break;
	}
}

void GUIManager::inc_points_collected()
{
	points_collected_++;
}

void GUIManager::inc_max_point_count()
{
	max_point_count_++;
}

void GUIManager::update_point_count(const int count)
{
	point_count_ = count;
}

void GUIManager::update_values(const string entity_type, const ofVec2f node_position, const ofVec2f node_velocity, const ofVec2f node_acceleration, const float _node_mass, const bool infmass, const float _node_radius, const bool is_affected_by_gravity, const int emission_frequency)
{
	if (entity_type == "Player")
	{
		position = ofToString(roundf(node_position.x)) + ", " + ofToString(roundf(node_position.y));
		velocity = ofToString(roundf(node_velocity.x * 100) / 100) + ", " + ofToString(roundf(node_velocity.y * 100) / 100);
		acceleration = ofToString(roundf(node_acceleration.x * 10000) / 10000) + ", " + ofToString(roundf(node_acceleration.y * 10000) / 10000);
		if (infmass)
		{
			mass.setTextColor(0);
			infinite_mass = true;
		}
		else
		{
			mass.setTextColor(255);
			mass = _node_mass;
			infinite_mass = false;
		}
		radius = _node_radius;
		affected_by_gravity = is_affected_by_gravity;

		player_gui_position_ = node_position;
	}
	else if (entity_type == "Mass")
	{
		selected_gui_.setName(entity_type);
		selected_gui_.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + node_position.x + 8, HALF_WORLD_HEIGHT + node_position.y + 8)));
		
		selected_position = ofToString(roundf(node_position.x)) + ", " + ofToString(roundf(node_position.y));
		selected_velocity = ofToString(roundf(node_velocity.x * 100) / 100) + ", " + ofToString(roundf(node_velocity.y * 100) / 100);
		selected_accel = ofToString(roundf(node_acceleration.x * 10000) / 10000) + ", " + ofToString(roundf(node_acceleration.y * 10000) / 10000);
		if (infmass)
		{
			selected_mass.setTextColor(0);
			selected_infinite_mass = true;
		}
		else
		{
			selected_mass.setTextColor(255);
			selected_mass = _node_mass;
			selected_infinite_mass = false;
		}
		selected_radius = _node_radius;
		selected_affected_by_gravity = is_affected_by_gravity;
	}
	else if (entity_type == "Collectable")
	{
		selected_gui_.setName(entity_type);
		selected_gui_.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + node_position.x + 8, HALF_WORLD_HEIGHT + node_position.y + 8)));

		selected_position = ofToString(roundf(node_position.x)) + ", " + ofToString(roundf(node_position.y));
		selected_velocity = ofToString(roundf(node_velocity.x * 100) / 100) + ", " + ofToString(roundf(node_velocity.y * 100) / 100);
		selected_accel = ofToString(roundf(node_acceleration.x * 10000) / 10000) + ", " + ofToString(roundf(node_acceleration.y * 10000) / 10000);
		if (infmass)
		{
			selected_mass.setTextColor(0);
			selected_infinite_mass = true;
		}
		else
		{
			selected_mass.setTextColor(255);
			selected_mass = _node_mass;
			selected_infinite_mass = false;
		}
		selected_radius = _node_radius;
		selected_affected_by_gravity = is_affected_by_gravity;
		selected_emission_frequency = emission_frequency;
	}
}

void GUIManager::update_multiple_values(const ofVec2f spring_anchor_position, const float spring_k, const float spring_damping, const float spring_mass, const bool is_affected_by_gravity, const ofVec2f selected_node_pos, const ofVec2f selected_node_vel, const ofVec2f selected_node_accel, const float selected_node_mass, const float selected_node_radius)
{
	multi_selection_gui_spring_.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + spring_anchor_position.x + 8, HALF_WORLD_HEIGHT + spring_anchor_position.y + 8)));
	multi_selection_gui_node_.setPosition(cam_->world_to_screen(ofVec2f(HALF_WORLD_WIDTH + selected_node_pos.x + 8, HALF_WORLD_HEIGHT + selected_node_pos.y + 8)));
	
	anchor_pos = ofToString(roundf(spring_anchor_position.x)) + ", " + ofToString(roundf(spring_anchor_position.y));

	k = spring_k;
	damping = spring_damping;
	springmass = spring_mass;
	spring_affected_by_gravity = is_affected_by_gravity;

	if (selected_node_pos == ofVec2f(-1, -1) && selected_node_vel == ofVec2f(-1, -1) && selected_node_accel == ofVec2f(-1, -1) && selected_node_mass == -1 && selected_node_radius == -1)
	{
		// can't draw
		multi_node_selected = false;
	}
	else
	{
		multi_node_selected = true;
		node_pos = ofToString(roundf(selected_node_pos.x)) + ", " + ofToString(roundf(selected_node_pos.y));
		node_vel = ofToString(roundf(selected_node_vel.x * 100) / 100) + ", " + ofToString(roundf(selected_node_vel.y * 100) / 100);
		node_accel = ofToString(roundf(selected_node_accel.x * 10000) / 10000) + ", " + ofToString(roundf(selected_node_accel.y * 10000) / 10000);
		node_mass = selected_node_mass;
		node_radius = selected_node_radius;
	}
}

void GUIManager::prepare_for_new_scene()
{
	max_point_count_ = 0;
	points_collected_ = 0;
}

void GUIManager::window_resized(int w, int h)
{
	create_node_gui_.setPosition(ofGetWidth() / 2 - create_node_gui_.getWidth() / 2, buffer_);
	selected_gui_.setPosition(ofGetWidth() - selected_gui_.getWidth() - buffer_, buffer_);
	multi_selection_gui_spring_.setPosition(ofGetWidth() - multi_selection_gui_spring_.getWidth() - buffer_, buffer_);
	multi_selection_gui_node_.setPosition(ofGetWidth() - multi_selection_gui_spring_.getWidth() - buffer_, multi_selection_gui_spring_.getPosition().y + multi_selection_gui_spring_.getHeight() + buffer_);
}

void GUIManager::set_clear_all()
{
	game_controller_->set_delete_all(true);
}

void GUIManager::draw_required_gui(GameObject* selected_object, const bool is_spring)
{
	if (game_controller_->get_gui_visible() /*|| Event_Manager->playerGUIVisible*/)
	{
		player_gui_.draw();

		if (selected_object != nullptr)
		{
			if (is_spring)
			{
				// if an object is a spring then it has multiple gui windows to draw
				multi_selection_gui_spring_.draw();
				if (multi_node_selected == true)
				{
					multi_selection_gui_node_.draw();
				}
			}
			else
			{
				selected_gui_.draw();
			}
		}
	}
	if (game_controller_->get_gui_visible())
	{
		world_gui_.draw();
		//create_node_gui_.draw();
	}

	FluidManager::draw_gui(draw_particle_gui_);
	audio_manager_->drawGUI(draw_audio_gui_);

	draw_text();
	if (game_mode_manager_->get_current_mode_id() == 0) draw_border();
}

void GUIManager::draw_text() const
{
	ofDrawBitmapString("GameMode: " + game_mode_manager_->get_current_mode_string(), glm::vec2((ofGetWidth() / 2) - 100, ofGetHeight() - 100));
	ofDrawBitmapString("Collectables Found: " + to_string(points_collected_) + " / " + to_string(max_point_count_), glm::vec2((ofGetWidth() / 2) - 100, ofGetHeight() - 50));
}

void GUIManager::draw_border() const
{
	ofPushStyle();
	ofPushMatrix();

	ofSetColor(255, 0, 0);
	ofNoFill();
	ofSetLineWidth(10);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());

	ofPopMatrix();
	ofPopStyle();
}

void GUIManager::key_pressed(const int key)
{
	if (key == 57344) // f1
	{		
		if (game_controller_->get_gui_visible())
		{
			game_controller_->set_gui_visible(false);
		}
		else
		{
			game_controller_->set_gui_visible(true);
			draw_particle_gui_ = false;
			draw_audio_gui_ = false;
		}
	}
	else if (key == 57345) // f2
	{		
		if (draw_particle_gui_)
		{
			draw_particle_gui_ = false;
		}
		else
		{
			game_controller_->set_gui_visible(false);
			draw_particle_gui_ = true;
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
			game_controller_->set_gui_visible(false);
			draw_particle_gui_ = false;
			draw_audio_gui_ = true;
		}
	}
}
