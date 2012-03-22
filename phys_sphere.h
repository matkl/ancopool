#ifndef PHYS_SPHERE_H
#define PHYS_SPHERE_H

#include <SDL/SDL.h>

#include "sphere.h"
#include "phys_quad.h"
#include "phys_object.h"

class Phys_sphere : public virtual Sphere, public Phys_object {
	public:
		Phys_sphere(double radius, double mass);
		
		double get_mass() const;
		vector3 get_velocity() const;
		void set_velocity(vector3 velocity);
		vector3 get_angular_velocity() const;
		void set_angular_velocity(vector3 angular_velocity);
		void move(double t);
		virtual void spin(double t) = 0;
		void accelerate(vector3 a, double t);
		double get_moment_of_inertia() const;
		double get_radius_of_gyration() const;
		virtual void enable() { enabled = true; }
		virtual void disable() { enabled = false; }
		bool is_enabled() const { return enabled; }
		void reset();

	private:
		double m;
		vector3 v;
		vector3 omega;
		double I; // moment of inertia
		double rg; // radius of gyration
		bool enabled;
};


#endif // PHYS_SPHERE_H
