#ifndef OBJECT_H
#define OBJECT_H

#include "vector.h"

class Object {
	public:
		Object();
		virtual ~Object();

		void set_position(vector3 destination);
		vector3 get_position() const;
		void translate(vector3 v);

		virtual double get_distance(const Object& obj) const;

	private:
		vector3 position;
};

inline void Object::set_position(vector3 destination) {
	position = destination;
}

inline vector3 Object::get_position() const {
	return position;
}

inline double Object::get_distance(const Object& other) const {
	return position.distance(other.position);
}

#endif // OBJECT_H

