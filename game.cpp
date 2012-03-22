#include "game.h"

Game::Game(Scene& scene, const std::vector<Player*>& players) : scene(scene), players(players) {
	active_player = this->players.begin();
}

Table& Game::get_table() const {
	return scene.get_table();
}


int Game::get_active_player_score() const {
	std::map<Player*, int>::const_iterator p = score.find(*active_player);
	if (p != score.end()) {
		return p->second;
	} else {
		return 0;
	}
}

void Game::set_active_player_score(int s) {
	score[*active_player] = s;
	scene.update_player_scores();
}

void Game::next_player() {
	if (++active_player == players.end()) {
		active_player = players.begin();
	}
	
	scene.update_player_names();
}

int Game::get_num_players() const {
	return players.size();
}

std::string Game::get_player_name(int i) const {
	Player* player = players[i];

	return player->get_name();
}

int Game::get_player_score(int i) const {
	Player* player = players[i];

	std::map<Player*, int>::const_iterator p = score.find(player);
	if (p != score.end()) {
		return p->second;
	} else {
		return 0;
	}
}

int Game::get_active_player_num() const {
	for (int i=0; i<players.size(); i++)
		if (players[i] == *active_player)
			return i;
	
	return -1;
}
