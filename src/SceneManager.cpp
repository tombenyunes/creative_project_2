#include "SceneManager.h"

SceneManager::SceneManager(): game_controller_(nullptr),
							  gui_manager_(nullptr),
							  fluid_manager_(nullptr),
                              audio_manager_(nullptr),
                              entity_manager_(nullptr),
                              gamemode_manager_(nullptr),
                              cam_(nullptr)
{
	cout << "------------SceneManager.cpp------------" << endl;
	cout << " - Press '1-4' to load preset scenes" << endl;
	cout << " - Press '0' to save the current scene" << endl;
	cout << " - Press '9' to load a saved scene" << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::init(Controller* game_controller, GUIManager* gui_manager, Camera* cam, FluidManager* fluid_manager, AudioManager* audio_manager, EntityManager* entity_manager, GamemodeManager* gamemode_manager)
{
	game_controller_ = game_controller;
	gui_manager_ = gui_manager;
	fluid_manager_ = fluid_manager;
	audio_manager_ = audio_manager;
	entity_manager_ = entity_manager;
	gamemode_manager_ = gamemode_manager;
	
	cam_ = cam;
}

void SceneManager::update()
{
	if (gamemode_manager_->get_request_for_procedural_scene()) {
		load_procedural_scene();
		gamemode_manager_->set_request_for_procedural_scene(false);
	}
	else if (gamemode_manager_->get_request_for_blank_scene()) {
		load_blank_scene();
		gamemode_manager_->set_request_for_blank_scene(false);
	}
	else if (gamemode_manager_->get_current_mode_string() == "Procedural")
	{
		if (entity_manager_->get_point_count() == Collectable::get_points_collected())
		{
			static bool trig = false;

			if (enter_pressed_)
			{
				load_procedural_scene();

				// zoom into the player
				cam_->set_zoom_mode(Camera::player_view);

				enter_pressed_ = false;
				trig = false;
			}
			else
			{
				if (!trig)
				{
					// zoom out to view the completed level
					cam_->set_zoom_mode(Camera::map_view);

					// play level complete queue
					audio_manager_->event_level_complete();

					trig = true;
				}
			}
		}
	}

	if (entity_manager_->get_point_count() <= 0) {

		Collectable::reset_ids();
		
		if (gamemode_manager_->get_current_mode_id() == 1) {
			load_procedural_scene();
		}
	}
}

void SceneManager::save_scene(const string scene_name)
{	
	cout << "------------SceneManager.cpp------------" << endl;

	xml1_.popTag();
	xml1_.clear();

	xml1_.addTag("Scene");
	xml1_.pushTag("Scene");

	xml1_.addValue("name", scene_name);

	xml1_.addTag("Fluid");
	xml1_.pushTag("Fluid", 0);
	xml1_.addValue("mode", reinterpret_cast<int&>(fluid_manager_->get_drawer()->drawMode));
	xml1_.popTag();

	for (int i = 0; i < entity_manager_->get_game_objects()->size(); i++)
	{
		// Shared properties
		xml1_.addTag("GameObject");
		xml1_.pushTag("GameObject", i);
		xml1_.addValue("type", (*entity_manager_->get_game_objects())[i]->get_type());
		xml1_.addValue("pos.x", (*entity_manager_->get_game_objects())[i]->get_position().x);
		xml1_.addValue("pos.y", (*entity_manager_->get_game_objects())[i]->get_position().y);		

		// Mass properties
		if ((*entity_manager_->get_game_objects())[i]->get_type() == "Mass")
		{
			xml1_.addValue("mass", (*entity_manager_->get_game_objects())[i]->get_mass());
			xml1_.addValue("radius", (*entity_manager_->get_game_objects())[i]->get_radius());
		}
		
		// Collectable properties
		if ((*entity_manager_->get_game_objects())[i]->get_type() == "Collectable")
		{
			xml1_.addValue("mass", (*entity_manager_->get_game_objects())[i]->get_mass());
			
			xml1_.addValue("radius", (*entity_manager_->get_game_objects())[i]->get_attribute_by_name("starting_radius"));			
			xml1_.addValue("emission_frequency", (*entity_manager_->get_game_objects())[i]->get_attribute_by_name("emission_frequency"));
			xml1_.addValue("emission_force", ((*entity_manager_->get_game_objects())[i]->get_attribute_by_name("emission_force")));
			xml1_.addValue("is_active", (*entity_manager_->get_game_objects())[i]->get_attribute_by_name("is_active"));
		}
		// Spring properties
		else if ((*entity_manager_->get_game_objects())[i]->get_type() == "Spring")
		{
			xml1_.addValue("k", (*entity_manager_->get_game_objects())[i]->get_attribute_by_name("k"));
			xml1_.addValue("damping", (*entity_manager_->get_game_objects())[i]->get_attribute_by_name("damping"));
			xml1_.addValue("springmass", (*entity_manager_->get_game_objects())[i]->get_attribute_by_name("springmass"));
			
			xml1_.addValue("node_count", static_cast<int>((*entity_manager_->get_game_objects())[i]->get_multiple_masses().size()));
			
			for (int j = 0; j < (*entity_manager_->get_game_objects())[i]->get_multiple_masses().size(); j++)
			{
				xml1_.addValue("mass" + to_string(j + 1), (*entity_manager_->get_game_objects())[i]->get_multiple_masses()[j]);
				xml1_.addValue("radius" + to_string(j + 1), (*entity_manager_->get_game_objects())[i]->get_multiple_radiuses()[j]);
			}
		}		
		xml1_.popTag();
	}

	xml1_.save(scene_name + ".xml");

	cout << " [ Current Scene Saved ]" << endl;
	cout << " - Scene Name: " << scene_name << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::get_ready_for_new_scene() const
{
	destroy_current_scene();
	reset_fluid();

	gui_manager_->reset_point_counters();
	Collectable::reset_ids();
}

void SceneManager::load_scene(const string path)
{
	get_ready_for_new_scene();

	if (xml_.loadFile(path)) {
		xml_.pushTag("Scene");

		cout << "------------SceneManager.cpp------------" << endl;
		cout << " [ Scene Loaded ]" << endl;
		cout << " - Scene Name: " << xml_.getValue("name", "N/A") << endl;

		const int fluid_count = xml_.getNumTags("Fluid");
		for (int i = 0; i < fluid_count; i++) {
			xml_.pushTag("Fluid", i);
			reinterpret_cast<int&>(fluid_manager_->get_drawer()->drawMode) = xml_.getValue("mode", -1);
			cout << "- Fluid Mode: " << xml_.getValue("mode", -1) << endl;
			xml_.popTag();
		}
		int count = 0;
		const int game_object_count = xml_.getNumTags("GameObject");
		for (int i = 0; i < game_object_count; i++)
		{
			count++;
			xml_.pushTag("GameObject", i);

			// Shared properties
			string type = xml_.getValue("type", "N/A");
			ofVec2f pos;
			pos.x = xml_.getValue("pos.x", -1);
			pos.y = xml_.getValue("pos.y", -1);

			// Player properties
			if (type == "Player")
			{
				GameObject* player = new Player();
				player->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_, gamemode_manager_);
				entity_manager_->add_game_object(player);
			}
			// Mass properties
			else if (type == "Mass")
			{
				const float mass = xml_.getValue("mass", -1);
				const float radius = xml_.getValue("radius", -1);

				GameObject* object = new Mass(pos, mass, radius);
				object->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_, gamemode_manager_);
				entity_manager_->add_game_object(object);
			}
			// Spring properties
			else if (type == "Spring")
			{
				const float k = xml_.getValue("k", -1.0f);
				const float damping = xml_.getValue("damping", -1.0f);
				const float springmass = xml_.getValue("springmass", -1.0f);
				
				vector<float> masses;
				vector<float> radiuses;

				const float node_count = xml_.getValue("node_count", -1);

				for (int i = 0; i < node_count; i++)
				{
					masses.push_back(xml_.getValue("mass" + to_string(i + 1), -1));
					radiuses.push_back(xml_.getValue("radius" + to_string(i + 1), -1));
				}							
				
				GameObject* spring = new Spring(pos, radiuses, masses, k, damping, springmass);
				spring->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_, gamemode_manager_);
				entity_manager_->add_game_object(spring);
			}
			// Collectable properties
			else if (type == "Collectable")
			{
				const float mass = xml_.getValue("mass", -1);
				const float radius = xml_.getValue("radius", -1);

				const float emission_frequency = xml_.getValue("emission_frequency", -1);
				const double emission_force = xml_.getValue("emission_force", -1.0f);
				const bool is_active = xml_.getValue("is_active", false);
				
				GameObject* point = new Collectable(pos, mass, radius, emission_frequency, emission_force, is_active);
				point->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_, gamemode_manager_);
				gui_manager_->set_max_point_count(gui_manager_->get_max_point_count() + 1);
				entity_manager_->add_game_object(point);
			}

			xml_.popTag();
		}

		cout << " - GameObject count: " << count << endl;
		cout << "----------------------------------------" << endl;

	}
}

void SceneManager::load_procedural_scene() const
{
	get_ready_for_new_scene();

	audio_manager_->event_new_level_loaded();
	
	fluid_manager_->explosion(500);

	entity_manager_->create_entity("Player");
	
	for (int i = 0; i < 5; i++) {
		entity_manager_->create_entity("Collectable", ofVec2f(ofRandom(static_cast<float>(-WORLD_WIDTH) / 2, static_cast<float>(WORLD_WIDTH) / 2), ofRandom(static_cast<float>(-WORLD_HEIGHT) / 2, static_cast<float>(WORLD_HEIGHT) / 2)));
	}	

	cout << "------------SceneManager.cpp------------" << endl;
	cout << " - New Procedural Level Loaded" << endl;
	cout << "----------------------------------------" << endl;
}

void SceneManager::load_blank_scene()
{
	load_scene("Scenes/blank_scene.xml");
	fluid_manager_->explosion(500);
}

void SceneManager::destroy_current_scene() const
{	
	for (auto& i : *entity_manager_->get_game_objects())
	{
		i->set_request_to_be_deleted(true);
	}
}

void SceneManager::reset_fluid() const
{
	fluid_manager_->reset_fluid();
}

void SceneManager::key_pressed(const int key)
{
	if (gamemode_manager_->get_current_mode_string() == "Sandbox") {
		if (key == '1')
		{
			// load scene 1
			load_scene("Scenes/Scene1.xml");

			fluid_manager_->explosion(500);
		}
		else if (key == '2')
		{
			// load scene 2
			load_scene("Scenes/Scene2.xml");

			fluid_manager_->explosion(500);
		}
		else if (key == '3')
		{
			// load scene 2
			load_scene("Scenes/Scene3.xml");

			fluid_manager_->explosion(500);
		}
		else if (key == '4')
		{
			// load scene 2
			load_scene("Scenes/blank_scene.xml");

			fluid_manager_->explosion(500);
		}
		else if (key == '4')
		{
			// load scene 2
			load_scene("Scenes/Scene4.xml");

			fluid_manager_->explosion(500);
		}
		else if (key == '5')
		{
			// load scene 2
			load_scene("Scenes/circle_scene.xml");

			fluid_manager_->explosion(500);
		}
	}
	
	if (key == 13) //enter
	{
		enter_pressed_ = true;
	}
	if (key == 57348) //f5
	{
		// quick-save scene
		save_scene("Scenes/saved_scene");
	}
	else if (key == 57352) //f9
	{
		// load quick-saved scene
		load_scene("Scenes/saved_scene.xml");

		fluid_manager_->explosion(500);
	}
	
	else if (key == 'p')
	{
		load_procedural_scene();
	}
	else if (key == 'o')
	{
		ofFileDialogResult load_result = ofSystemLoadDialog("Load Scene");
		if (load_result.bSuccess) {
			ofDisableDataPath();
			load_scene(load_result.getPath());
			ofEnableDataPath();
		}
	}
}

void SceneManager::key_released(const int key)
{
	if (key == 13)
	{
		enter_pressed_ = false;
	}
}