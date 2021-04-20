#include "PlayerTrail.h"

PlayerTrail::PlayerTrail(const ofVec2f pos, const ofVec2f vel, const float radius, const ofColor color, const int lifetime)
	:	lifetime_(lifetime)
{
	set_position(pos);
	set_velocity(ofVec2f(vel.x + ofRandom(-1.5f, 1.5f), vel.y + ofRandom(-1.5f, 1.5f)));
	set_radius(radius);
	set_color(color);	

	add_module("screenBounce");
	add_module("gravity");
}

void PlayerTrail::update()
{
	friction_ = vel_;
	friction_ *= -1;
	friction_ *= FRICTION_FORCE;
	accel_ += (friction_*2);
	accel_.limit(MAXIMUM_ACCELERATION);

	vel_ += accel_;
	accel_.set(0);

	ofVec2f new_pos;
	new_pos.x = ofLerp(pos_.x, pos_.x + vel_.x, 0.75f);
	new_pos.y = ofLerp(pos_.y, pos_.y + vel_.y, 0.75f);

	pos_ = new_pos;

	if (radius_ > 0) {
		radius_ -= 0.05f;
	}
	else {
		set_request_to_be_deleted(true);
	}
}

void PlayerTrail::draw()
{
	ofSetColor(color_, lifetime_);
	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);
}