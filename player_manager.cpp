/*
 *  player_manager.cpp
 *  ancopool
 *
 *  Created by Matthias Klein on 28.10.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "player_manager.h"

Player_manager::Player_manager() {
}

Player_manager::Player_manager(const Player_manager&) {
}

Player_manager::~Player_manager() {
	for (std::map<std::string, Player*>::iterator p = players.begin(); p != players.end(); p++) {
		delete p->second;
	}
	
	players.clear();
}

Player_manager& Player_manager::get() {
	static Player_manager instance;
	return instance;
}

// Create a new player in the database and return a pointer.
Player* Player_manager::new_player(const std::string& name) {
	if (get_player(name)) {
		// A player with this name already exists.
		return NULL;
	}

	// Create new player.
	Player* new_player = new Player(name);
	
	// Put player into database.
	players[name] = new_player;
	
	return new_player;
}

Player* Player_manager::get_player(const std::string& name) {
	std::map<std::string, Player*>::iterator p = players.find(name);

	if (p != players.end()) {
		return p->second;
	} else {
		return NULL;
	}
}
