#include "gl_sphere.h"

#include <cmath>
#include <cstdlib>
#include <GL/glu.h>

GL_sphere::GL_sphere(double radius) : Sphere(radius), enabled(true) {
	// Set rotation matrix to random rotation.
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glRotatef((double)(rand()) / RAND_MAX * 360.0, rand(), rand(), rand());
	glGetFloatv(GL_MODELVIEW_MATRIX, rotation_matrix);
	glPopMatrix();
}

void GL_sphere::display() const {
	if (!enabled)
		return;

	glPushMatrix();

	GL_object::display();

	glMultMatrixf(rotation_matrix);

	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluSphere(quadric, get_radius(), 50, 50);
	gluDeleteQuadric(quadric);

	glPopMatrix();
}

// TODO: is incorrect
void GL_sphere::rotate(vector3 a) const {
	glPushMatrix();
	glLoadIdentity();
	glRotatef(a.abs()*180.0/M_PI, a.x, a.y, a.z);
	glMultMatrixf(rotation_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, rotation_matrix);
	glPopMatrix();
}
