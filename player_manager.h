/*
 *  player_manager.h
 *  ancopool
 *
 *  Created by Matthias Klein on 28.10.10.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include <string>
#include <map>
#include "player.h"

class Player_manager {
public:
	~Player_manager();
	static Player_manager& get();
	
	Player* new_player(const std::string& name);
	Player* get_player(const std::string& name);
	
private:
	Player_manager();
	Player_manager(const Player_manager&);
	
	std::map<std::string, Player*> players;
};
