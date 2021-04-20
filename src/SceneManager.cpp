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

void SceneManager::update() const
{
	if (gamemode_manager_->request_for_new_scene) {
		load_procedural_scene();
		gamemode_manager_->request_for_new_scene = false;
	}

	if (gamemode_manager_->get_current_mode_id() == 1) {
		if (entity_manager_->get_point_count() <= 0) {			
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
		xml1_.addTag("GameObject");
		xml1_.pushTag("GameObject", i);
		xml1_.addValue("type", (*entity_manager_->get_game_objects())[i]->get_type());
		xml1_.addValue("pos.x", (*entity_manager_->get_game_objects())[i]->get_position().x);
		xml1_.addValue("pos.y", (*entity_manager_->get_game_objects())[i]->get_position().y);
		xml1_.addValue("mass", (*entity_manager_->get_game_objects())[i]->get_mass());
		xml1_.addValue("radius", (*entity_manager_->get_game_objects())[i]->get_radius());
		if ((*entity_manager_->get_game_objects())[i]->get_type() == "Spring")
		{
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

	gui_manager_->prepare_for_new_scene();
}

void SceneManager::load_scene(const string path)
{
	get_ready_for_new_scene();

	if (xml_.loadFile(path + ".xml")) {
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

			string type = xml_.getValue("type", "N/A");
			ofVec2f pos;
			pos.x = xml_.getValue("pos.x", -1);
			pos.y = xml_.getValue("pos.y", -1);

			if (type == "Player")
			{
				float mass = xml_.getValue("mass", -1);
				float radius = xml_.getValue("radius", -1);

				GameObject* player = new Player;
				player->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_,
				             audio_manager_);
				entity_manager_->add_game_object(player);
			}
			else if (type == "Mass")
			{
				const float mass = xml_.getValue("mass", -1);
				const float radius = xml_.getValue("radius", -1);

				GameObject* object = new Mass(pos, mass, radius);
				object->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_,
				             audio_manager_);
				entity_manager_->add_game_object(object);
			}
			else if (type == "Spring")
			{
				vector<float> masses;
				vector<float> radiuses;

				const float node_count = xml_.getValue("node_count", -1);

				for (int i = 0; i < node_count; i++)
				{
					masses.push_back(xml_.getValue("mass" + to_string(i + 1), -1));
					radiuses.push_back(xml_.getValue("radius" + to_string(i + 1), -1));
				}							
				
				GameObject* spring = new Spring(pos, radiuses, masses, 2, 2, 22);
				spring->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_,
				             audio_manager_);
				entity_manager_->add_game_object(spring);
			}
			else if (type == "Point")
			{
				const float mass = xml_.getValue("mass", -1);
				const float radius = xml_.getValue("radius", -1);

				GameObject* point = new Point(pos, mass, radius);
				point->init(entity_manager_->get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_,
				            audio_manager_);
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

	fluid_manager_->explosion(500);

	entity_manager_->create_entity("Player");

	for (int i = 0; i < 5; i++) {
		entity_manager_->create_entity("Point");
	}	

	cout << "------------SceneManager.cpp------------" << endl;
	cout << " - New Procedural Level Loaded" << endl;
	cout << "----------------------------------------" << endl;
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
	if (gamemode_manager_->get_current_mode_id() == 0) {
		if (key == '1') {
			// load scene 1
			load_scene("Scenes/Scene1");

			fluid_manager_->explosion(500);
		}
		else if (key == '2') {
			// load scene 2
			load_scene("Scenes/Scene2");

			fluid_manager_->explosion(500);
		}
		else if (key == '3') {
			// load scene 2
			load_scene("Scenes/Scene3");

			fluid_manager_->explosion(500);
		}
		else if (key == '4') {
			// load scene 2
			load_scene("Scenes/Scene4");

			fluid_manager_->explosion(500);
		}
		else if (key == '9') {
			// load saved scene
			load_scene("Scenes/newScene");

			fluid_manager_->explosion(500);
		}
	}
	if (key == '0') {
		// save scene
		save_scene("Scenes/newScene");
	}
	else if (key == 'p') {
		load_procedural_scene();
	}
}