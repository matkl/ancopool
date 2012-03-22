#include "phys_sphere.h"

#include <cmath>

static const double sqrt_10_div_5 = sqrt(10.0)/5.0;

Phys_sphere::Phys_sphere(double radius, double mass) : Sphere(radius), m(mass), I(0.4*mass*radius*radius), rg(sqrt_10_div_5*radius), enabled(true) {
}

double Phys_sphere::get_mass() const {
	return m;
}

vector3 Phys_sphere::get_velocity() const {
	return v;
}

void Phys_sphere::set_velocity(vector3 velocity) {
	v = velocity;
}

vector3 Phys_sphere::get_angular_velocity() const {
	return omega;
}

void Phys_sphere::set_angular_velocity(vector3 angular_velocity) {
	omega = angular_velocity;
}

void Phys_sphere::move(double t) {
	translate(v*t);
}

void Phys_sphere::accelerate(vector3 a, double t) {
	v += a*t;
}

double Phys_sphere::get_moment_of_inertia() const {
	return I;
}

double Phys_sphere::get_radius_of_gyration() const {
	return rg;
}

void Phys_sphere::reset() {
	v = vector3(0.0, 0.0, 0.0);
	omega = vector3(0.0, 0.0, 0.0);
}

