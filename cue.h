#ifndef CUE_H
#define CUE_H

#include "gl_object_container.h"
#include "gl_phys_sphere.h"

class Scene;

class Cue : public GL_object_container {
	public:
		Cue(Scene& scene, double length, double top_radius, double bottom_radius, double mass);

		virtual void display() const;

		void set_offset(vector3 new_offset);
		vector3 get_offset() const;
		void set_angle(double a);
		double get_angle() const;

		void set_cue_ball(GL_phys_sphere* ball);
		void set_rotation(double r);
		double get_rotation() const;

	private:
		double top_radius;
		double bottom_radius;
		double mass;
		double length;
		vector3 offset;
		double rotation;
		double angle;

		GL_phys_sphere* cue_ball;

		Scene& scene;
};

#endif
