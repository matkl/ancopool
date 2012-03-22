#include "gl_object_container.h"

GL_object_container::GL_object_container() {
}

GL_object_container::~GL_object_container() {
	clear();
}

void GL_object_container::push_back(GL_object* obj) {
	objs.push_back(obj);
}

void GL_object_container::clear() {
	for (std::vector<GL_object*>::iterator p = objs.begin(); p != objs.end(); p++) {
		delete *p;
	}
}

void GL_object_container::display() const {
	glPushMatrix();
	GL_object::display();
	display_objects();
	glPopMatrix();
}

void GL_object_container::display_objects() const {
	for (std::vector<GL_object*>::const_iterator p = objs.begin(); p != objs.end(); p++) {
		(*p)->display();
	}
}

void GL_object_container::update_texture() {
	GL_object::update_texture();
	
//	for (std::vector<GL_object*>::iterator p = objs.begin(); p != objs.end(); p++) {
//		(*p)->update_texture();
//	}
}

std::vector<GL_object*>::iterator GL_object_container::begin() {
	return objs.begin();
}

std::vector<GL_object*>::iterator GL_object_container::end() {
	return objs.end();
}


