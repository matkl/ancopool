#include "gl_object.h"

#include "texture_manager.h"

GL_object::GL_object() : texture(0), cached_texture_id(0) {
}

GL_object::~GL_object() {
	if (texture)
		Texture_manager::get().release_texture(texture);
}

void GL_object::set_texture(const std::string& filename) {
	if (texture)
		Texture_manager::get().release_texture(texture);

	texture = Texture_manager::get().request_texture(filename);
	update_texture();
}

void GL_object::update_texture() {
	if (texture)
		cached_texture_id = texture->get_id();
	else
		cached_texture_id = 0;
}

void GL_object::display() const {
	if (cached_texture_id > 0) {
		glBindTexture(GL_TEXTURE_2D, cached_texture_id);
	}

	vector3 position = get_position();
	glTranslatef(position.x, position.y, position.z);
}

