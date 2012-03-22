/*
 *  main_menu.cpp
 *  ancopool
 *
 *  Created by Matthias Klein on 16.11.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "main_menu.h"

#include "anco_pool.h"

Main_menu::Main_menu() : GUI_sheet("mainmenu.layout") {
	CEGUI::WindowManager& wmgr = CEGUI::WindowManager::getSingleton();

	CEGUI::PushButton* local_game_button = (CEGUI::PushButton*)wmgr.getWindow("mainMenu/localGameButton");	
	local_game_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Main_menu::handle_local_game_button, this));
	
	CEGUI::PushButton* quit_button = (CEGUI::PushButton*)wmgr.getWindow("mainMenu/quitButton");
	quit_button->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Main_menu::handle_quit_button, this));
}

bool Main_menu::handle_quit_button(const CEGUI::EventArgs& e) {
	Anco_pool::get().quit();

	return true;
}

bool Main_menu::handle_local_game_button(const CEGUI::EventArgs& e) {
	Anco_pool::get().start_local_game();
	
	return true;
}
