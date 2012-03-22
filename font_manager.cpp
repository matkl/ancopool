/*
 *  font_manager.cpp
 *  ancopool
 *
 *  Created by Matthias Klein on 25.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "font_manager.h"

#include <sstream>

Font_manager::~Font_manager() {
	for (std::map<std::string, Font*>::iterator p = fonts.begin(); p != fonts.end(); p++) {
		Font* font = p->second;
		delete font;
	}
}

Font_manager& Font_manager::get() {
	static Font_manager instance;
	return instance;
}

Font* Font_manager::get_font(const std::string& filename, int size) {
//	std::ostringstream key;
//	key << filename << size;
	
//	// Check if font is already loaded.
//	std::map<std::string, Font*>::iterator iter = fonts.find(key.str());
	
	Font* font;
	
//	if (iter == fonts.end()) {
//		// Font is not loaded yet. Load font and put it into our database.
		font = new Font(filename.c_str(), size);
//		fonts[key.str()] = font;
//	} else {
//		// Font is in our database.
//		font = iter->second;
//	}
	
	return font;
}

		
