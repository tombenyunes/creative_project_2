#include "PlayerParticle.h"

PlayerParticle::PlayerParticle(ofVec2f _pos, ofVec2f _vel, float _radius, ofColor _color, int _lifetime)
{
	pos = _pos;
	vel = _vel;
	vel.x += ofRandom(-1.5, 1.5);
	vel.y += ofRandom(-1.5, 1.5);
	radius = _radius;
	starting_radius = radius;
	color = _color;
	starting_alpha = _color.a;
	alpha = starting_alpha;
	lifetime = _lifetime;
	starting_lifetime = lifetime;
}

void PlayerParticle::update()
{
	friction = vel;
	friction *= -1;
	friction *= 0.015;
	accel += (friction * 2);
	accel.limit(0.15);

	vel += accel;
	accel.set(0);

	ofVec2f newPos;
	newPos.x = ofLerp(pos.x, pos.x + vel.x, 0.75);
	newPos.y = ofLerp(pos.y, pos.y + vel.y, 0.75);

	pos = newPos;
	radius = ofMap(lifetime, 0, starting_lifetime, 0, starting_radius, true);
	alpha = ofMap(lifetime, 0, starting_lifetime, 0, starting_alpha, true);

	if (lifetime > 0) {
		lifetime--;
	}
	else {
		needs_to_be_deleted = true;
	}
}

void PlayerParticle::draw()
{
	ofSetColor(color, 50);
	ofFill();
	ofDrawEllipse(pos.x, pos.y, radius, radius);

	ofSetColor(color, alpha);
	ofNoFill();
	ofDrawEllipse(pos.x, pos.y, radius, radius);
}