#include <OpenGL/gl.h>
#include "camera.h"

Camera::Camera() {
}

void Camera::set_center(vector3 point) {
	center = point;
}

void Camera::rotate(vector3 a) {
	rotation += a;
	
	if (rotation.x < 0.0)
		rotation.x = 0.0;
	else if (rotation.x > 90.0)
		rotation.x = 90.0;
}

void Camera::zoom(double a) {
	distance += a;

	if (distance > MAX_DISTANCE)
		distance = MAX_DISTANCE;
	else if (distance < MIN_DISTANCE)
		distance = MIN_DISTANCE;
}

void Camera::look() const {
	glTranslatef(0.0, 0.0, -distance);
	glRotatef(rotation.x, 1.0, 0.0, 0.0);
	glRotatef(rotation.y, 0.0, 1.0, 0.0);
	glRotatef(rotation.z, 0.0, 0.0, 1.0);
	glTranslatef(-center.x, -center.y, -center.z);
}

