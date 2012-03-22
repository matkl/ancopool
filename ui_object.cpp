#include "ui_object.h"

namespace UI {
	Object::Object(Object* parent) : parent(parent) {
		if (parent) {
			parent->add_child(this);
		}
	}

	Object::~Object() {
		for (std::vector<Object*>::iterator p = children.begin(); p != children.end(); p++) {
			delete *p;
		}
	}

	void Object::display() const {
		for (std::vector<Object*>::const_iterator p = children.begin(); p != children.end(); p++) {
			(*p)->display();
		}
	}
	
	void Object::set_position(vector2 point) {
		position = point;
	}

	void Object::add_child(Object* child) {
		children.push_back(child);
	}
}

