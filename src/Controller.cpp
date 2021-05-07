#include "Controller.h"

Controller::Controller()
	:	gravity_(false)
	,	mouse_being_dragged_(false)
	,	hard_collisions_(false)
{
}

void Controller::set_gravity(const bool value)
{
	(value == 1) ? gravity_ = true : gravity_ = false;
}

bool Controller::get_gravity() const
{
	return gravity_;
}

void Controller::set_mouse_dragged(const bool value)
{
	(value == 1) ? mouse_being_dragged_ = true : mouse_being_dragged_ = false;
}

bool Controller::get_mouse_dragged() const
{
	return mouse_being_dragged_;
}

void Controller::set_use_hard_collisions(const bool value)
{
	(value == 1) ? hard_collisions_ = true : hard_collisions_ = false;
}

bool Controller::get_use_hard_collisions() const
{
	return hard_collisions_;
}