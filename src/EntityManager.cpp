#include "EntityManager.h"

EntityManager::EntityManager(): selected_game_object_(nullptr),
								selected_game_object_index_(-1),
								game_controller_(nullptr),
								gui_manager_(nullptr),
								fluid_manager_(nullptr),
								audio_manager_(nullptr),
								game_mode_manager_(nullptr),
								cam_(nullptr)
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

void EntityManager::update_game_objects()
{
	delete_game_objects();
	find_selected();

	// update all gameobjects
	for (auto& i : *get_game_objects())
	{
		i->root_update();
	}
}

void EntityManager::delete_game_objects() const
{
	// erase objects that need to be deleted and free memory
	for (int i = 0; i < get_game_objects()->size(); i++) {
		if ((*get_game_objects())[i]->get_type() == "Player") {
			cam_->update(WORLD_WIDTH, WORLD_HEIGHT, ofVec2f((*get_game_objects())[i]->get_position().x + WORLD_WIDTH / 2, (*get_game_objects())[i]->get_position().y + WORLD_HEIGHT / 2));
		}
		if ((*get_game_objects())[i]->get_request_to_be_deleted() == true) {
			if ((*get_game_objects())[i] == game_controller_->get_active()) {
				game_controller_->make_active(nullptr);
			}
			if ((*get_game_objects())[i]->get_type() == "Point") {
				if ((*get_game_objects())[i]->get_request_to_be_deleted_event() == "Collected") {
					gui_manager_->inc_points_collected();
				}
			}
			delete (*get_game_objects())[i];
			get_game_objects()->erase(get_game_objects()->begin() + i);
		}
	}

	// delete all if gui requests it
	if (game_controller_->get_delete_all()) {
		game_controller_->set_delete_all(false);
		delete_all(true);
	}
}

void EntityManager::find_selected()
{
	// find gameobject/gameobjects that are selected
	for (int i = 0; i < get_game_objects()->size(); i++) {
		if ((*get_game_objects())[i]->get_request_to_be_selected() == true) {
			
			if (selected_game_object_ != (*get_game_objects())[i]) {
				// accept request

				// remove previously selected object
				if (selected_game_object_ != nullptr) (*get_game_objects())[selected_game_object_index_]->set_is_selected(false);

				// make new object selected
				selected_game_object_ = (*get_game_objects())[i];
				selected_game_object_index_ = i;
				(*get_game_objects())[i]->set_is_selected(true);
			}
			else {
				// deny request
				(*get_game_objects())[i]->set_is_selected(false);
			}

			(*get_game_objects())[i]->set_request_to_be_selected(false);
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

void EntityManager::draw_game_objects() const
{
	for (auto& i : *get_game_objects())
	{
		i->root_draw();
	}
}

void EntityManager::create_entity(const string entity_type) const
{
	int type_id;
	if (entity_type.empty()) {
		type_id = game_controller_->get_new_node_type();
	}
	else {
		type_id = -1;
	}
	
	if (entity_type == "Player") {
		GameObject* player = new Player;
		player->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(player);
	}
	if (type_id == 0 || entity_type == "Mass") {
		cout << "Mass created" << endl;
		GameObject* object = new Mass(ofVec2f(game_controller_->get_world_mouse_pos().x, game_controller_->get_world_mouse_pos().y), ofRandom(MASS_LOWER_BOUND, MASS_UPPER_BOUND), ofRandom(RADIUS_LOWER_BOUND, RADIUS_UPPER_BOUND));
		object->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(object);
	}
	else if (type_id == 1 || entity_type == "Spring") {
		cout << "Spring created" << endl;
		GameObject* spring = new Spring(ofVec2f(game_controller_->get_world_mouse_pos().x, game_controller_->get_world_mouse_pos().y), { ofRandom(25, 50), ofRandom(25, 50) }, { ofRandom(25, 75), ofRandom(25, 75) }, 2, 2, 22);
		spring->init(get_game_objects(), game_controller_, gui_manager_, cam_, fluid_manager_, audio_manager_);
		add_game_object(spring);
	}
	else if (type_id == 2 || entity_type == "Point") {
		cout << "Point created" << endl;
		GameObject* point = new Point(ofVec2f(ofRandom(static_cast<float>(-WORLD_WIDTH) / 2, static_cast<float>(WORLD_WIDTH) / 2), ofRandom(static_cast<float>(-WORLD_HEIGHT) / 2, static_cast<float>(WORLD_HEIGHT) / 2)), 15, 25);
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
		if (i->get_type() == "Point") {
			pointCount++;
		}
	}
	gui_manager_->update_point_count(pointCount);
	return pointCount;
}

void EntityManager::key_pressed(const int key) const
{
	for (auto& i : *get_game_objects())
	{
		i->root_key_pressed(key);
	}

	if (game_mode_manager_->get_current_mode_id() == 0) {
		if (key == 'c') {
			create_entity();
		}
		if (key == 57358) {
			if (game_controller_->get_new_node_type() < 2) {
				game_controller_->set_new_node_type(game_controller_->get_new_node_type() + 1); // next
			}
		}
		else if (key == 57356) {
			if (game_controller_->get_new_node_type() > 0) {
				game_controller_->set_new_node_type(game_controller_->get_new_node_type() - 1); // previous
			}
			else {
				game_controller_->set_new_node_type(0);
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
		const ofVec3f local_view = ofVec3f(x - (static_cast<float>(WORLD_WIDTH) / 2), y - (static_cast<float>(WORLD_HEIGHT) / 2), cam_->get_position().z);
		const ofVec3f world_view = cam_->screen_to_world(local_view);
		i->mouse_dragged(world_view.x, world_view.y, button);
	}
}

void EntityManager::mouse_pressed(const int x, const int y, const int button) const
{
	for (auto& i : *get_game_objects())
	{
		const ofVec3f local_view = ofVec3f(x - (WORLD_WIDTH / 2), y - (WORLD_HEIGHT / 2), cam_->get_position().z);
		const ofVec3f world_view = cam_->screen_to_world(local_view);
		i->mouse_pressed(world_view.x, world_view.y, button);
	}
}

void EntityManager::mouse_scrolled(const int x, const int y, const float scroll_x, const float scroll_y)
{
}

void EntityManager::mouse_released(const int x, const int y, const int button) const
{
	for (auto& i : *get_game_objects())
	{
		i->mouse_released(x - (static_cast<float>(WORLD_WIDTH) / 2), y - (static_cast<float>(WORLD_HEIGHT) / 2), button);
	}
}
