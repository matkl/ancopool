#ifndef GL_QUADS_H
#define GL_QUADS_H

#include <vector>
#include "gl_object.h"

class GL_object_container : public GL_object {
	public:
		GL_object_container();
		virtual ~GL_object_container();

		void push_back(GL_object* obj);
		void clear();
		virtual void display() const;
		virtual void update_texture();

		std::vector<GL_object*>::iterator begin();
		std::vector<GL_object*>::iterator end();

	protected:
		void display_objects() const;

	private:
		std::vector<GL_object*> objs;
};

#endif // GL_QUADS_H
