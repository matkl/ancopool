#include "player.h"

Player::Player(const std::string& name) : name(name), score(0) {
}

std::string Player::get_name() const {
	return name;
}

int Player::get_score() const {
	return score;
}

void Player::set_score(int s) {
	score = s;
}