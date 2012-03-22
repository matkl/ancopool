#include "nine_ball.h"

#include <cmath>
#include <sstream>
#include "scene.h"
#include "resource_manager.h"

using std::cerr;
using std::endl;

Nine_ball::Nine_ball(Scene& scene, const std::vector<Player*>& players) : Game(scene, players), breaking(false) {
	Table& table = get_table();
	
	Resource_manager& rmgr = Resource_manager::get();
	
	// Create cue ball.
	cue_ball = scene.get_table().create_ball(rmgr.get_resource("poolball0.png"));
	balls[0] = cue_ball;
	
	for (int i=1; i<=9; i++) {
		std::ostringstream texture;
		texture << "poolball" << i << ".png";
		balls[i] = table.create_ball(rmgr.get_resource(texture.str()));
	}

	rack();

}

void Nine_ball::rack() {
	Table& table = get_table();
	double ball_radius = table.get_ball_radius();

	// Get head and foot spot positions from table.
	head_spot = table.get_head_spot() + vector3(0.0, ball_radius, 0.0);
	foot_spot = table.get_foot_spot() + vector3(0.0, ball_radius, 0.0);

	// Place cue ball on the head spot.
	cue_ball->set_position(head_spot);

	// Place other balls.
	balls[1]->set_position(foot_spot + vector3(0.0, 0.0, 2 * sqrt(3.0) * ball_radius));
	balls[2]->set_position(foot_spot + vector3(ball_radius, 0.0, sqrt(3.0) * ball_radius));
	balls[3]->set_position(foot_spot + vector3(-ball_radius, 0.0, sqrt(3.0) * ball_radius));
	balls[4]->set_position(foot_spot + vector3(2 * ball_radius, 0.0, 0.0));
	balls[5]->set_position(foot_spot + vector3(-2 * ball_radius, 0.0, 0.0));
	balls[6]->set_position(foot_spot + vector3(ball_radius, 0.0, -sqrt(3.0) * ball_radius));
	balls[7]->set_position(foot_spot + vector3(-ball_radius, 0.0, -sqrt(3.0) * ball_radius));
	balls[8]->set_position(foot_spot + vector3(0.0, 0.0, -2 * sqrt(3.0) * ball_radius));
	balls[9]->set_position(foot_spot);

	for (int i=0; i<10; i++) {
		balls[i]->enable();
		balls_on_table[i] = balls[i];
	}

	lowest_ball = balls[1];

	breaking = true;
	
	scene.set_camera_center(cue_ball->get_position());
	scene.set_cue_ball(cue_ball);
	scene.set_cue_offset(vector3(0.0, 0.0, cue_ball->get_radius() * 2));

//	cue_ball->set_velocity(vector3(0.0, 0.50, -10.0));
}

void Nine_ball::on_collision_event(Phys_collision_type type_id, Phys_object* obj1, Phys_object* obj2) {
	if (type_id == PHYS_COLLISION_SPHERE_SPHERE) {
		GL_phys_sphere* ball1 = static_cast<GL_phys_sphere*>(obj1);
		GL_phys_sphere* ball2 = static_cast<GL_phys_sphere*>(obj2);

		GL_phys_sphere* ball;

		if (ball1 == cue_ball)
			ball = ball2;
		else if (ball2 == cue_ball)
			ball = ball1;
		else
			return;

		if (!shot_summary.lowest_ball_hit) {
			// The lowest ball has to be hit first.
			if (ball == lowest_ball) {
				shot_summary.lowest_ball_hit = true;
			} else {
				std::cerr << "foul: did not hit lowest ball first." << std::endl;
				shot_summary.foul = true;
			}
		}
	} else if (type_id == PHYS_COLLISION_SPHERE_QUAD) {
		GL_phys_sphere* ball = static_cast<GL_phys_sphere*>(obj1);
		GL_phys_quad* quad = static_cast<GL_phys_quad*>(obj2);
		Quad_type quad_type = quad->get_quad_type();
		switch (quad_type) {
			case QUAD_TYPE_NONE:
				return;
			case QUAD_TYPE_RAIL:
				if (shot_summary.lowest_ball_hit && find(shot_summary.balls_hit_rail.begin(), shot_summary.balls_hit_rail.end(), ball) == shot_summary.balls_hit_rail.end()) {
					shot_summary.balls_hit_rail.push_back(ball);
				}
				break;
			case QUAD_TYPE_POCKET_FOOT_LEFT:
			case QUAD_TYPE_POCKET_FOOT_RIGHT:
			case QUAD_TYPE_POCKET_MIDDLE_LEFT:
			case QUAD_TYPE_POCKET_MIDDLE_RIGHT:
			case QUAD_TYPE_POCKET_HEAD_LEFT:
			case QUAD_TYPE_POCKET_HEAD_RIGHT:
				std::cerr << "versenkt!" << std::endl;
				
				shot_summary.balls_in_pocket.push_back(ball);
				ball->reset();
				ball->disable();

				if (ball == cue_ball) {
					shot_summary.foul = true;
					cerr << "foul: cue ball pocketed." << endl;
				}

				break;
			case QUAD_TYPE_OUT:
				std::cerr << "out!" << std::endl;
				shot_summary.balls_out.push_back(ball);
				ball->reset();
				ball->disable();
				shot_summary.foul = true;
				break;
		}
	}
}

void Nine_ball::on_end_of_shot() {
	// Evaluate shot.
	
	bool& lowest_ball_hit = shot_summary.lowest_ball_hit;
	std::vector<GL_phys_sphere*>& balls_hit_rail = shot_summary.balls_hit_rail;
	std::vector<GL_phys_sphere*>& balls_in_pocket = shot_summary.balls_in_pocket;
	std::vector<GL_phys_sphere*>& balls_out = shot_summary.balls_out; 
	bool& foul = shot_summary.foul;

	if (!foul) {
		if (!lowest_ball_hit) {
			std::cerr << "foul: did not hit any ball" << std::endl;
			foul = true;
		} else {
			if (breaking) {
				// Evaluate break shot.
				
				if (balls_in_pocket.empty()) {
					// No ball pocketed at the break, so 4 balls must hit the rail.
					// The cue ball doesn't count.
					remove(balls_hit_rail.begin(), balls_hit_rail.end(), cue_ball);
					if (balls_hit_rail.size() < 4) {
						std::cerr << "foul: less than 4 balls on the rail" << std::endl;
						foul = true;
					}
				}
			} else {
				// Evaluate regular shot.
				
				if (balls_in_pocket.empty()) {
					// No balls pocketed, so either the cue ball or the object ball must hit a rail.
					if (balls_hit_rail.empty()) {
						std::cerr << "foul: must hit rail after collision" << std::endl;
						foul = true;
					}
				}
			}
		}
	}
	
	if (foul) {
		std::cerr << "FOUL!" << std::endl;
		// give ball in hand
		cue_ball->set_position(head_spot);
		cue_ball->enable();
		
		for (std::vector<GL_phys_sphere*>::iterator p = balls_in_pocket.begin(); p != balls_in_pocket.end(); p++) {
			if (*p == cue_ball) {
				balls_in_pocket.erase(p);
				break;
			}
		}
		
		next_player();
	} else {
		if (balls_in_pocket.empty()) {
			next_player();
		} else {
			// Check if the 9 ball was pocketed.
			GL_phys_sphere* nine_ball = balls[9];
			
			for (std::vector<GL_phys_sphere*>::iterator p = balls_in_pocket.begin(); p != balls_in_pocket.end(); p++) {
				if (*p == nine_ball) {
					std::cerr << "WIN!" << std::endl;
					set_active_player_score(get_active_player_score() + 1);
					rack();
					return;
				}
			}
			
			// continue
		}
	}

	for (std::vector<GL_phys_sphere*>::iterator p = balls_in_pocket.begin(); p != balls_in_pocket.end(); p++) {
		GL_phys_sphere* ball = *p;
		for (std::map<int, GL_phys_sphere*>::iterator q = balls_on_table.begin(); q != balls_on_table.end(); q++) {
			if (q->second == ball) {
				std::cerr << "removing " << q->first << std::endl;
				balls_on_table.erase(q);
				break;
			}
		}
	}
	
	// Set new lowest ball.
	std::map<int, GL_phys_sphere*>:: iterator q = balls_on_table.begin();
	// Ignore cue ball.
	q++;
	if (q == balls_on_table.end()) {
		// No balls on table.
	} else {
		std::cerr << "Lowest ball: " << q->first << std::endl;
		lowest_ball = q->second;
	}
						
	// Setup next shot.
	shot_summary.clear();
	breaking = false;

	// Center camera on cue ball.
	scene.set_camera_center(cue_ball->get_position());
	scene.set_cue_ball(cue_ball);
	scene.set_cue_offset(vector3(0.0, 0.0, cue_ball->get_radius() * 2));
}
