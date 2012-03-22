#include "text.h"

#include <iostream>
#include "resource_manager.h"

namespace GUI {
	Text::Text(Widget* parent) : Widget(parent), font(0) {
		set_font(Resource_manager::get().get_resource("Arial.ttf"), 12);
	}
	
	void Text::set_text(const std::string& text) {
		this->text = text;
	}
	
	void Text::set_font(const std::string& filename, int size) {
		font = Font_manager::get().get_font(filename, size);
		font->setForegroundColor(1.0, 1.0, 1.0);
		font->setHorizontalJustification( OGLFT::Face::CENTER );
	}
	
	void Text::draw() const {
		// TODO: where to put?
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );

		font->draw(0.0, 0.0, text.c_str());
	}
}