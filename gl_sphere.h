#ifndef GL_SPHERE_H
#define GL_SPHERE_H

#include "sphere.h"
#include "gl_object.h"

class GL_sphere : public virtual Sphere, public GL_object {
	public:
		GL_sphere(double radius);
		virtual void display() const;

		void rotate(vector3 a) const;

		virtual void enable() { enabled = true; }
		virtual void disable() { enabled = false; }

	private:
		// Shame on me for declaring the rotation_matrix as mutable.
		// It is updated by the display() function of GL_phys_sphere, which
		// calculates it from the angular velocity of the sphere.
		mutable GLfloat rotation_matrix[16];
		bool enabled;
};


#endif // GL_SPHERE_H
