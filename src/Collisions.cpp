#include "Collisions.h"

bool Collisions::EllipseCompare(ofVec2f _pos1, float _r1, ofVec2f _pos2, float _r2)
{
	float dist = _pos2.distance(_pos1);

	if (dist <= (_r1 + _r2)/2) {
		return true;
	}
	else {
		return false;
	}
}
