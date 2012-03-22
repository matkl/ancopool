#include "phys_quad.h"

// Phys_quad::Phys_quad() {
// }

Phys_quad::Phys_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, enum Quad_type quad_type): 
	Quad(v0, v1, v2, v3),
	aabb(v0, v1, v2, v3),
	coefficient_of_restitution(1.0),
	coefficient_of_rolling_friction(0.00714),
	coefficient_of_sliding_friction(0.2),
	quad_type(quad_type)
{
}

Aabb Phys_quad::get_aabb() const {
	return aabb;
}

double Phys_quad::get_coefficient_of_restitution() const {
	return coefficient_of_restitution;
}

double Phys_quad::get_coefficient_of_rolling_friction() const {
	return coefficient_of_rolling_friction;
}

double Phys_quad::get_coefficient_of_sliding_friction() const {
	return coefficient_of_sliding_friction;
}

enum Quad_type Phys_quad::get_quad_type() const {
	return quad_type;
}

void Phys_quad::set_coefficient_of_restitution(double cor) {
	coefficient_of_restitution = cor;
}

void Phys_quad::set_coefficient_of_rolling_friction(double mu_rolling) {
	coefficient_of_rolling_friction = mu_rolling;
}

void Phys_quad::set_coefficient_of_sliding_friction(double mu_sliding) {
	coefficient_of_sliding_friction = mu_sliding;
}

void Phys_quad::set_quad_type(enum Quad_type qt) {
	quad_type = qt;
}

