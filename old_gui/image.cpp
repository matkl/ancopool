#include "image.h"

#include "texture_manager.h"

GUI::Image::Image(GUI::Widget* parent, const std::string& image_str) : GUI::Widget(parent), texture(0) {
	if (!image_str.empty())
		set_image(image_str);
}

GUI::Image::~Image() {
	if (texture)
		delete texture;
}

void GUI::Image::set_image(const std::string& image_str) {
	if (texture)
		Texture_manager::get().release_texture(texture);

	texture = Texture_manager::get().request_texture(image_str);
}

void GUI::Image::draw() const {
	glBindTexture(GL_TEXTURE_2D, texture->get_id());
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0); glVertex2d(0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex2d(128.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex2d(128.0, 128.0);
	glTexCoord2f(0.0, 1.0); glVertex2d(0.0, 128.0);
	glEnd();
}

