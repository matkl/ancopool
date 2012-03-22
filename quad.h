#ifndef QUAD_H
#define QUAD_H

#include "object.h"

class Quad : public virtual Object {
	public:
		Quad();
		Quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3);
		void set_vertices(vector3 v0, vector3 v1, vector3 v2, vector3 v3);
		vector3 get_normal() const;
		vector3 get_vertex(int i) const;

	private:
		void calculate_normal();

		vector3 vertices[4];
		vector3 normal;
};

inline void Quad::set_vertices(vector3 v0, vector3 v1, vector3 v2, vector3 v3) {
	vertices[0] = v0;
	vertices[1] = v1;
	vertices[2] = v2;
	vertices[3] = v3;

	calculate_normal();
}

inline vector3 Quad::get_normal() const {
	return normal;
}

#endif // QUAD_H
