#ifndef GL_PHYS_SPHERE_H
#define GL_PHYS_SPHERE_H

#include "gl_sphere.h"
#include "phys_sphere.h"

class GL_phys_sphere : public GL_sphere, public Phys_sphere {
	public:
		GL_phys_sphere(double radius, double mass);

		virtual void display() const;

		virtual void spin(double t);
		virtual void enable() {
			GL_sphere::enable();
			Phys_sphere::enable();
		}
		virtual void disable() {
			GL_sphere::disable();
			Phys_sphere::disable();
		}
	
	private:
		mutable Uint32 last_rotation_update;
};

#endif // GL_PHYS_SPHERE_H
