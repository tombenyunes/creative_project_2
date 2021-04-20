#include "Collisions.h"

bool Collisions::ellipse_compare(const ofVec2f pos1, const float r1, const ofVec2f pos2, const float r2)
{
	const float dist = pos2.distance(pos1);

	if (dist <= (r1 + r2)/2) {
		return true;
	}
	else {
		return false;
	}
}