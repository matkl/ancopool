#ifndef LIGHT_H
#define LIGHT_H

#include <OpenGL/gl.h>

class Light {
	public:
		Light();
		~Light();
		void update() const;
		void enable();
		void disable();
		void display() const;

		void set_ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
			ambient[0] = r;
			ambient[1] = g;
			ambient[2] = b;
			ambient[3] = a;
		}

		void set_diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
			diffuse[0] = r;
			diffuse[1] = g;
			diffuse[2] = b;
			diffuse[3] = a;
		}

		void set_specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
			specular[0] = r;
			specular[1] = g;
			specular[2] = b;
			specular[3] = a;
		}

		void set_position(GLfloat x, GLfloat y, GLfloat z, GLfloat w) {
			position[0] = x;
			position[1] = y;
			position[2] = z;
			position[3] = w;
		}

		void set_spot_direction(GLfloat x, GLfloat y, GLfloat z) {
			spot_direction[0] = x;
			spot_direction[1] = y;
			spot_direction[2] = z;
		}

		void set_spot_exponent(GLfloat a) {
			spot_exponent = a;
		}

		void set_spot_cutoff(GLfloat a) {
			spot_cutoff = a;
		}

		void set_constant_attenuation(GLfloat a) {
			constant_attenuation = a;
		}

		void set_linear_attenuation(GLfloat a) {
			linear_attenuation = a;
		}

		void set_quadratic_attenuation(GLfloat a) {
			quadratic_attenuation = a;
		}

	private:
		GLenum id;
		bool enabled;
		GLfloat ambient[4];
		GLfloat diffuse[4];
		GLfloat specular[4];
		GLfloat position[4];
		GLfloat spot_direction[3];
		GLfloat spot_exponent;
		GLfloat spot_cutoff;
		GLfloat constant_attenuation;
		GLfloat linear_attenuation;
		GLfloat quadratic_attenuation;

		static Light* lights[GL_MAX_LIGHTS];
};

#endif // LIGHT_H
