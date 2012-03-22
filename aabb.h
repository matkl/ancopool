#ifndef AABB_H
#define AABB_H

#include "vector.h"

class Aabb {
	public:
		Aabb(vector3 c, vector3 r);
		Aabb(vector3 c, double r);
		Aabb(vector3 v0, vector3 v1, vector3 v2, vector3 v3);
		vector3 get_min() const;
		vector3 get_max() const;
		void merge(const Aabb& a);
		bool intersect(const Aabb& a) const;

	private:
		vector3 c;	// center point
		vector3 r;	// halfwidths
};

#endif // AABB_H
