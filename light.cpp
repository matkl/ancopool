#include "light.h"

Light* Light::lights[GL_MAX_LIGHTS] = { 0 };

Light::Light():
	id(0),
	enabled(false),
	spot_exponent(0.0),
	spot_cutoff(180.0),
	constant_attenuation(1.0),
	linear_attenuation(0.0),
	quadratic_attenuation(0.0)
{
	set_ambient(0.0, 0.0, 0.0, 1.0);
	set_diffuse(1.0, 1.0, 1.0, 1.0);
	set_specular(1.0, 1.0, 1.0, 1.0);
	set_position(0.0, 0.0, 1.0, 0.0);
	set_spot_direction(0.0, 0.0, -1.0);

	for (int i=0; i<GL_MAX_LIGHTS; i++) {
		if (!lights[i]) {
			id = GL_LIGHT0 + i;
			lights[i] = this;
			break;
		}
	}
}

Light::~Light() {
	if (id >= GL_LIGHT0 && id < GL_LIGHT0 + GL_MAX_LIGHTS) {
		lights[id - GL_LIGHT0] = 0;
		disable();
	}
}

void Light::display() const {
	glLightfv(id, GL_POSITION, position);
}

void Light::update() const {
	glLightfv(id, GL_AMBIENT, ambient);
	glLightfv(id, GL_DIFFUSE, diffuse);
	glLightfv(id, GL_SPECULAR, specular);
	glLightfv(id, GL_POSITION, position);
	glLightfv(id, GL_SPOT_DIRECTION, spot_direction);
	glLightf(id, GL_SPOT_EXPONENT, spot_exponent);
	glLightf(id, GL_SPOT_CUTOFF, spot_cutoff);
	glLightf(id, GL_CONSTANT_ATTENUATION, constant_attenuation);
	glLightf(id, GL_LINEAR_ATTENUATION, linear_attenuation);
	glLightf(id, GL_QUADRATIC_ATTENUATION, quadratic_attenuation);

	if (enabled) {
		glEnable(id);
	}
}

void Light::enable() {
	glEnable(id);
	enabled = true;
}

void Light::disable() {
	glDisable(id);
	enabled = false;
}



