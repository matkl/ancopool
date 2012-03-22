#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "table.h"
#include "light.h"
#include "cue.h"
#include "player.h"
#include "table.h"

class Object;
class Camera;
class Table;
class Game;

class Scene {
public:
	Scene(const std::vector<Player*>& players);
	~Scene();
	
	void display() const;
	void update_light();
	
	// Camera control
	void set_camera_center(vector3 point);
	void rotate_camera(double x, double y);
	void zoom_camera(double a);
	
	// Cue control
	void set_cue_ball(GL_phys_sphere* ball);
	void set_cue_offset(vector3 offset);
	void add_cue_offset(vector3 offset);
	void set_cue_angle(double angle);
	void add_cue_angle(double angle);
	void set_cue_rotation(double angle);
	void add_cue_rotation(double angle);
	
	// Player
	void update_player_names();
	void update_player_scores();
	
	void update_textures();
	void update();
	
	Table& get_table() const;
	Game& get_game() const;
	
	void start_simulation();
	void on_stop_simulation();
	
private:
	Camera* camera;
	Table* table;
	Light* light;
	Cue* cue;
	Game* game;
	
	bool simulating;
	
	//		std::vector<GL_object*> objs;
};

#endif // SCENE_H
