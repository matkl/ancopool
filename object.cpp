#include "object.h"

#include <OpenGL/gl.h>
#include <cmath>

Object::Object() {
}

Object::~Object() {
}

void Object::translate(vector3 v) {
	position += v;
}
