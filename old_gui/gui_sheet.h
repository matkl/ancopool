/*
 *  gui_sheet.h
 *  ancopool
 *
 *  Created by Matthias Klein on 16.11.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef GUI_SHEET_H
#define GUI_SHEET_H

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <CEGUIBase/CEGUI.h>

class GUI_sheet {

public:
	GUI_sheet(const CEGUI::String& filename);
	void show();
	
protected:
	void set_window(CEGUI::Window* win);

private:
	CEGUI::Window* window;
};

#endif // GUI_SHEET_H