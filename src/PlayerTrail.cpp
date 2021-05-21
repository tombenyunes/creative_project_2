#include "PlayerTrail.h"

PlayerTrail::PlayerTrail(const ofVec2f pos, const ofVec2f vel, const float radius, const ofColor color, const int lifetime)
	:	pos_(pos),
		vel_(ofVec2f(vel.x + ofRandom(-1.5f, 1.5f), vel.y + ofRandom(-1.5f, 1.5f))),
		radius_(radius),
		color_(color),
		lifetime_(lifetime)
{
}

void PlayerTrail::update()
{
	friction_ = vel_;
	friction_ *= -1;
	friction_ *= 0.015f;
	accel_ += (friction_*2);
	accel_.limit(0.15f);

	vel_ += accel_;
	accel_.set(0);

	ofVec2f new_pos;
	new_pos.x = ofLerp(pos_.x, pos_.x + vel_.x, 0.75f);
	new_pos.y = ofLerp(pos_.y, pos_.y + vel_.y, 0.75f);

	pos_ = new_pos;

	if (radius_ > 0)
	{
		radius_ -= 0.05f;
	}
}

void PlayerTrail::draw()
{
	ofSetColor(color_, lifetime_);
	ofDrawEllipse(pos_.x, pos_.y, radius_, radius_);
}