#include "font_manager.h"

#include <sstream>
#include <iostream>

using std::cerr;
using std::endl;

Font_manager& Font_manager::get() {
	static Font_manager instance;
	return instance;
}

Font_manager::~Font_manager() {
}

void Font_manager::shutdown() {
	for (Font_iter p = fonts.begin(); p != fonts.end(); p++) {
		delete p->second;
	}

	fonts.clear();
}

FTFont* Font_manager::get_font(const std::string& filename, int size) {
	std::ostringstream ss;
	ss << filename << size;
	std::string font_key = ss.str();

	Font_iter p = fonts.find(font_key);
	if (p != fonts.end()) {
		return p->second;
	}

	FTFont* font = new FTTextureFont(filename.c_str());

	if (font->Error()) {
		cerr << "Could not load font " << filename << " (size " << size << ")." << endl;
		delete font;
	}

	if (!font->FaceSize(size)) {
		cerr << "Font " << filename << " failed to set size " << size << "." << endl;
		delete font;
	}

	fonts[font_key] = font;

	return font;
}

Font_manager::Font_manager() {
}

Font_manager::Font_manager(const Font_manager&) {
}

Font_manager& Font_manager::operator= (const Font_manager&) {
	return *this;
}
