#include <algorithm>
#include <cmath>

#include "aabb.h"

using std::min;
using std::max;

Aabb::Aabb(vector3 c, vector3 r) : c(c), r(r) {
}

Aabb::Aabb(vector3 c, double r) : c(c), r(r, r, r) {
}

Aabb::Aabb(vector3 v0, vector3 v1, vector3 v2, vector3 v3) {
	vector3 vmin, vmax;

	vmin.x = min(min(v0.x, v1.x), min(v2.x, v3.x));
	vmin.y = min(min(v0.y, v1.y), min(v2.y, v3.y));
	vmin.z = min(min(v0.z, v1.z), min(v2.z, v3.z));

	vmax.x = max(max(v0.x, v1.x), max(v2.x, v3.x));
	vmax.y = max(max(v0.y, v1.y), max(v2.y, v3.y));
	vmax.z = max(max(v0.z, v1.z), max(v2.z, v3.z));

	r = (vmax - vmin) / 2.0;
	c = vmin + r;
}

vector3 Aabb::get_min() const {
	return c - r;
}

vector3 Aabb::get_max() const {
	return c + r;
}

bool Aabb::intersect(const Aabb& a) const {
	if (fabs(c.x - a.c.x) > (r.x + a.r.x)) return false;
	if (fabs(c.y - a.c.y) > (r.y + a.r.y)) return false;
	if (fabs(c.z - a.c.z) > (r.z + a.r.z)) return false;

	// We have an overlap.
	return true;
}

void Aabb::merge(const Aabb& a) {
	vector3 min = get_min();
	vector3 max = get_max();
	const vector3 amin = a.get_min();
	const vector3 amax = a.get_max();

	if (amin.x < min.x)
		min.x = amin.x;
	if (amin.y < min.y)
		min.y = amin.y;
	if (amin.z < min.z)
		min.z = amin.z;

	if (amax.x > max.x)
		max.x = amax.x;
	if (amax.y > max.y)
		max.y = amax.y;
	if (amax.z > max.z)
		max.z = amax.z;

	r = (max - min) / 2.0;
	c = min + r;
}

