#ifndef GAME_H
#define GAME_H

#include "table.h"
#include "player.h"

class Table;

class Game {
public:
	Game(Scene& scene, const std::vector<Player*>& players);
	virtual void rack() = 0;
	
	virtual void on_collision_event(Phys_collision_type type_id, Phys_object* obj1, Phys_object* obj2) = 0;
	virtual void on_end_of_shot() = 0;
	int get_active_player_score() const;
	void set_active_player_score(int s);
	void next_player();
	int get_num_players() const;
	std::string get_player_name(int i) const;
	int get_player_score(int i) const;
	int get_active_player_num() const;
	
protected:
	Table& get_table() const;
	
	Scene& scene;
	
	std::vector<Player*> players;
	std::map<Player*, int> score;
	std::vector<Player*>::iterator active_player;
};

#endif // GAME_H
