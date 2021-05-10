#include "EntityManager.h"

EntityManager::EntityManager(): selected_game_object_(nullptr),
								selected_game_object_index_(-1),
								game_controller_(nullptr),
								gui_manager_(nullptr),
								fluid_manager_(nullptr),
								audio_manager_(nullptr),
								game_mode_manager_(nullptr),
								cam_(nullptr),
								new_node_type_id_(0)
{	
}

void EntityManager::init(Controller* game_controller, GUIManager* gui_manager, Camera* cam, FluidManager* fluid_manager, AudioManager* audio_manager, GamemodeManager* gamemode_manager)
{
	game_controller_ = game_controller;
	gui_manager_ = gui_manager;
	fluid_manager_ = fluid_manager;
	audio_manager_ = audio_manager;
	game_mode_manager_ = gamemode_manager;

	cam_ = cam;
}

vector<GameObject*>* EntityManager::get_game_objects() const
{
	return game_objects_;
}

GameObject* EntityManager::get_selected_game_object() const
{
	return selected_game_object_;
}

void EntityManager::add_game_object(GameObject* _gameobject) const
{
	game_objects_->push_back(_gameobject);
}

void EntityManager::update()
{
	if (gui_manager_->gui_world_calculate_entities)
	{
		delete_game_objects();
		find_selected();

		// update all gameobjects
		for (auto& i : *get_game_objects())
		{
			i->root_update();
		}
	}
}

void EntityManager::delete_game_objects()
{
	// erase objects that need to be deleted and free memory
	for (int i = 0; i < get_game_objects()->size(); i++) {
		if ((*get_game_objects())[i]->get_type() == "Player") {
			set_player_position(ofVec2f((*get_game_objects())[i]->get_position().x + HALF_WORLD_WIDTH, (*get_game_objects())[i]->get_position().y + HALF_WORLD_HEIGHT));
			player_ = (*get_game_objects())[i];
		}
		if ((*get_game_objects())[i]->get_request_to_be_deleted() == true) {
			if ((*get_game_objects())[i] == get_selected_game_object()) {
				selected_game_object_ = nullptr;
			}
			if ((*get_game_objects())[i]->get_type() == "Collectable") {
				if ((*get_game_objects())[i]->get_request_to_be_deleted_event() == "Collected") {
					gui_manager_->inc_points_collected();
				}
			}
			delete (*get_game_objects())[i];
			get_game_objects()->erase(get_game_objects()->begin() + i);
		}
	}

	// delete all if gui requests it
	if (gui_manager_->get_delete_all()) {
		gui_manager_->toggle_delete_all();
		delete_all(true);
	}
}

void EntityManager::find_selected()
{
	// find gameobject/gameobjects that are selected
	for (int i = 0; i < get_game_objects()->size(); i++)
	{
		if ((*get_game_objects())[i]->get_request_to_be_selected() == true)
		{
			
			if (selected_game_object_ != (*get_game_objects())[i])
			{
				// accept request

				// remove previously selected object
				if (selected_game_object_ != nullptr) (*get_game_objects())[selected_game_object_index_]->set_is_selected(false);

				// make new object selected
				selected_game_object_ = (*get_game_objects())[i];
				selected_game_object_index_ = i;
				(*get_game_objects())[i]->set_is_selected(true);
			}
			else
			{
				// deny request
				(*get_game_objects())[i]->set_is_selected(false);
			}

			(*get_game_objects())[i]->set_request_to_be_selected(false);
		}
		else if ((*get_game_objects())[i]->get_request_to_be_deselected() == true)
		{
			// accept request
			selected_game_object_ = nullptr;
			selected_game_object_index_ = -1;
			(*get_game_objects())[i]->set_is_selected(false);
			(*get_game_objects())[i]->set_request_to_be_deselected(false);
		}
	}
}

void EntityManager::delete_all(const bool exclude_player) const
{
	for (auto& i : *get_game_objects())
	{
		if (exclude_player) {
			if (i->get_type() != "Player") {
				i->set_request_to_be_deleted(true);
			}
		}
		else {
			i->set_request_to_be_deleted(true);
		}
	}
}

void EntityManager::set_new_node_type(int id)
{
	new_node_type_id_ = id;
}

int EntityManager::get_new_node_type() const
{
	return new_node_type_id_;
}

void EntityManager::draw_game_objects() const
{
	ofEnableAlphaBlending();

	if (gui_manager_->gui_world_calculate_entities)
	{
		for (auto& i : *get_game_objects())
		{
			i->root_draw();
		}
	}
}


void EntityManager::create_entity() const
{
	// if no args, get new node type
	
	const int type_id = get_new_node_type();
	if (type_id == 0)
	{
		create_entity("Mass");
	}
	else if (type_id == 1)
	{
		create_entity("Spring");
	}
	else if (type_id == 2)
	{
		create_entity("Collectable");
	}
}

void EntityManager::create_entity(const string entity_type) const
{
	// if no pos, create at mouse pos
	
	create_entity(entity_type, ofVec2f(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y));
}

void EntityManager::create_entity(const string entity_type, const ofVec2f pos) const
{	
	if (entity_type == "Player") {
		GameObject* player = new Player;
		player->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(player);
	}
	if (entity_type == "Mass") {
		cout << "Mass created" << endl;
		GameObject* object = new Mass(pos, ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(object);
	}
	else if (entity_type == "Spring") {
		cout << "Spring created" << endl;
		GameObject* spring = new Spring(pos, { ofRandom(25, 50), ofRandom(25, 50) }, { ofRandom(25, 75), ofRandom(25, 75) }, 2, 2, 22);
		spring->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(spring);
	}
	else if (entity_type == "Collectable") {
		cout << "Collectable created" << endl;
		GameObject* point = new Collectable(pos, 15, 25);
		point->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(point);
		gui_manager_->inc_max_point_count();
	}
}


int EntityManager::get_point_count() const
{
	int pointCount = 0;
	for (auto& i : *get_game_objects())
	{
		if (i->get_type() == "Collectable") {
			pointCount++;
		}
	}
	gui_manager_->update_point_count(pointCount);
	return pointCount;
}

GameObject* EntityManager::get_player() const
{
	return player_;
}

ofVec2f EntityManager::get_player_position() const
{
	return player_position_;
}

void EntityManager::set_player_position(const ofVec2f pos)
{
	player_position_ = pos;
}

void EntityManager::key_pressed(const int key)
{
	for (auto& i : *get_game_objects())
	{
		i->root_key_pressed(key);
	}

	if (game_mode_manager_->get_current_mode_id() == 0)
	{
		if (key == 'c')
		{
			create_entity();
		}
		if (key == 'd')
		{
			if (get_new_node_type() < 2)
			{
				set_new_node_type(get_new_node_type() + 1); // next
			}
		}
		else if (key == 'a')
		{
			if (get_new_node_type() > 0)
			{
				set_new_node_type(get_new_node_type() - 1); // previous
			}
			else
			{
				set_new_node_type(0);
			}
		}
	}
}

void EntityManager::key_released(const int key) const
{
	for (auto& i : *get_game_objects())
	{
		i->root_key_released(key);
	}
}

void EntityManager::mouse_moved(const int x, const int y)
{
}

void EntityManager::mouse_dragged(const int x, const int y, const int button) const
{
	for (auto& i : *get_game_objects())
	{																   
		i->mouse_dragged(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y, button);
	}
}

void EntityManager::mouse_pressed(const int x, const int y, const int button)
{
	if (button == 2)
	{
		if (selected_game_object_ != nullptr)
		{
			get_selected_game_object()->set_is_selected(false);
			selected_game_object_ = nullptr;
		}
	}
	for (auto& i : *get_game_objects())
	{																   
		i->mouse_pressed(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y, button);
	}
}

void EntityManager::mouse_scrolled(const int x, const int y, const float scroll_x, const float scroll_y)
{
}

void EntityManager::mouse_released(const int x, const int y, const int button) const
{
	for (auto& i : *get_game_objects())
	{
		i->mouse_released(cam_->get_world_mouse_pos().x, cam_->get_world_mouse_pos().y, button);
	}
}
