#ifndef PLAYER_H
#define PLAYER_H

#include <string>

class Player {
public:
	Player(const std::string& name);
	
	std::string get_name() const;
	int get_score() const;
	void set_score(int s);
	
private:
	std::string name;
	int score;
};

#endif // PLAYER_H
