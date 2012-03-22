#include "quad.h"

Quad::Quad() {
}

Quad::Quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3) {
	set_vertices(v0, v1, v2, v3);
}

void Quad::calculate_normal() {
	normal = cross(vertices[0], vertices[1]) + cross(vertices[1], vertices[2]) + cross(vertices[2], vertices[3]) + cross(vertices[3], vertices[0]);
	normal = normal.normalize();
}

vector3 Quad::get_vertex(int i) const {
	return vertices[i];
}

