/*
 *  font_manager.h
 *  ancopool
 *
 *  Created by Matthias Klein on 25.12.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <string>
#include <map>
#include "OGLFT.h"

typedef OGLFT::TranslucentTexture Font;

class Font_manager {
public:
	~Font_manager();
	static Font_manager& get();
	Font* get_font(const std::string& filename, int size);
	
private:
	Font_manager() {}
	std::map<std::string, Font*> fonts;
};

#endif // FONT_MANAGER_H