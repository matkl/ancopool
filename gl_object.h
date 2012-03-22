#ifndef GL_OBJECT_H
#define GL_OBJECT_H

#include "object.h"
#include "texture.h"

#include <OpenGL/gl.h>

class GL_object : public virtual Object {
	public:
		GL_object();
		~GL_object();

		void set_texture(const std::string& filename);
		virtual void update_texture();
		virtual void display() const;

	private:
		Texture* texture;
		GLuint cached_texture_id;
};



#endif // GL_OBJECT_H
