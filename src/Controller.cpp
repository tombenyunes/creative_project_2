#include "Controller.h"

Controller::Controller(): active_object_(nullptr),
                          gravity_(false),
                          need_to_delete_all_(false),
                          object_selected_(false),
                          mouse_being_dragged_(false),
                          new_node_id_(0),
                          hard_collisions_(false),
                          gui_visible_(false)
{
}

void Controller::make_active(GameObject* _this)
{
	active_object_ = _this;
}

GameObject* Controller::get_active() const
{
	return active_object_;
}

void Controller::set_gravity(const bool value)
{
	(value == 1) ? gravity_ = true : gravity_ = false;
}

bool Controller::get_gravity() const
{
	return gravity_;
}

void Controller::set_delete_all(const bool value)
{
	(value == 1) ? need_to_delete_all_ = true : need_to_delete_all_ = false;
}

bool Controller::get_delete_all() const
{
	return need_to_delete_all_;
}

void Controller::set_object_selected(const bool value)
{
	(value == 1) ? object_selected_ = true : object_selected_ = false;
}

bool Controller::get_object_selected() const
{
	return object_selected_;
}

void Controller::set_mouse_dragged(const bool value)
{
	(value == 1) ? mouse_being_dragged_ = true : mouse_being_dragged_ = false;
}

bool Controller::get_mouse_dragged() const
{
	return mouse_being_dragged_;
}

void Controller::set_new_node_type(const int value)
{
	new_node_id_ = value;
}

int Controller::get_new_node_type() const
{
	return new_node_id_;
}

void Controller::set_use_hard_collisions(const bool value)
{
	(value == 1) ? hard_collisions_ = true : hard_collisions_ = false;
}

bool Controller::get_use_hard_collisions() const
{
	return hard_collisions_;
}

void Controller::set_gui_visible(const bool value)
{
	(value == 1) ? gui_visible_ = true : gui_visible_ = false;
}

bool Controller::get_gui_visible() const
{
	return gui_visible_;
}