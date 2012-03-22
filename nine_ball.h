#ifndef NINE_BALL_H
#define NINE_BALL_H

#include "game.h"

class Nine_ball_shot_summary {
public:
	Nine_ball_shot_summary() : lowest_ball_hit(false), foul(false) {}
	void clear() {
		lowest_ball_hit = false;
		balls_hit_rail.clear();
		balls_in_pocket.clear();
		balls_out.clear();
		foul = false;
	}
	bool lowest_ball_hit;
	std::vector<GL_phys_sphere*> balls_hit_rail;
	std::vector<GL_phys_sphere*> balls_in_pocket;
	std::vector<GL_phys_sphere*> balls_out;
	bool foul;
};

class Nine_ball : public Game {
public:
	Nine_ball(Scene& scene, const std::vector<Player*>& players);
	virtual void rack();
	
	virtual void on_collision_event(Phys_collision_type type_id, Phys_object* obj1, Phys_object* obj2);
	
	virtual void on_end_of_shot();
	
private:
	std::map<int, GL_phys_sphere*> balls;
	std::map<int, GL_phys_sphere*> balls_on_table;
	GL_phys_sphere* cue_ball;
	
	bool breaking;	// true if the next shot is the break
	GL_phys_sphere* lowest_ball;
	Nine_ball_shot_summary shot_summary;
	
	vector3 head_spot;
	vector3 foot_spot;
};

#endif // NINE_BALL_H
