/*
 *  gui_sheet.cpp
 *  ancopool
 *
 *  Created by Matthias Klein on 16.11.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "gui_sheet.h"

GUI_sheet::GUI_sheet(const CEGUI::String& filename) {
	window = CEGUI::WindowManager::getSingleton().loadWindowLayout(filename);

}

void GUI_sheet::show() {
	CEGUI::System::getSingleton().setGUISheet(window);
}

void GUI_sheet::set_window(CEGUI::Window* win) {
	window = win;
}
