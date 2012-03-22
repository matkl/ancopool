#include "gl_phys_sphere.h"

GL_phys_sphere::GL_phys_sphere(double radius, double mass) : GL_sphere(radius), Phys_sphere(radius, mass), Sphere(radius), last_rotation_update(0) {
}

void GL_phys_sphere::display() const {
	if (last_rotation_update != 0) {
		rotate(get_angular_velocity()*((SDL_GetTicks()-last_rotation_update)*0.001));
	}
	last_rotation_update = SDL_GetTicks();

	GL_sphere::display();

	/*
	vector3 a = get_position();
	vector3 b = a + get_velocity();

//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_LIGHTING);

	glColor3f(0.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex3f(a.x, a.y, a.z);
		glVertex3f(b.x, b.y, b.z);
	glEnd();
	glColor3f(1.0, 1.0, 1.0);

//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_LIGHTING);
*/
}

void GL_phys_sphere::spin(double t) {
	vector3 omega = get_angular_velocity();
	rotate(omega*t);
}

