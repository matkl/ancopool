/*
 *  main_menu.h
 *  ancopool
 *
 *  Created by Matthias Klein on 16.11.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "gui_sheet.h"

class Main_menu : public GUI_sheet {
public:
	Main_menu();
	
	bool handle_quit_button(const CEGUI::EventArgs& e);
	bool handle_local_game_button(const CEGUI::EventArgs& e);
};

#endif // MAIN_MENU_H
