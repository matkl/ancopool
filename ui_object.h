#ifndef UI_OBJECT_H
#define UI_OBJECT_H

#include <vector>
#include "vector.h"

namespace UI {
	class Object {
		public:
			Object(Object* parent = 0);
			virtual ~Object();
			virtual void set_position(vector2 point);
			virtual void display() const;

		private:
			void add_child(Object* child);

			Object* parent;
			vector2 position;
			std::vector<Object*> children;
	};
}

#endif // UI_OBJECT_H
