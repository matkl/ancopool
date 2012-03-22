#include <sstream>

#include "scene.h"
#include "camera.h"
#include "pool_table.h"
#include "anco_pool.h"
#include "nine_ball.h"
#include "resource_manager.h"

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

//#include <CEGUIBase/CEGUI.h>
//#include <CEGUIBase/RendererModules/OpenGL/CEGUIOpenGLRenderer.h>


Scene::Scene(const std::vector<Player*>& players) : simulating(false) {
	Resource_manager& rmgr = Resource_manager::get();
	std::string rdir = rmgr.get_resources_directory();
		
	// Set up camera.
	camera = new Camera;

	// Set up lighting.
	light = new Light;
	light->set_ambient(0.5, 0.5, 0.5, 1.0);
	light->set_position(0.0, 1.0, 0.0, 1.0);
	light->set_spot_direction(0.0, -1.0, 0.0);
	light->set_spot_cutoff(120.0);
	light->set_spot_exponent(15.0);
	light->update();
	light->enable();
	
	// Set up table.
	Pool_table* pool_table = new Pool_table(*this, 9.0);
	table = pool_table;
	pool_table->set_cloth_texture(rmgr.get_resource("pooltablefelt1.png"));
	pool_table->set_frame_texture(rmgr.get_resource("wood_mahagony.png"));
	pool_table->set_pocket_texture(rmgr.get_resource("blackleather.png"));

	// Set up cue.
	cue = new Cue(*this, 1.50, 0.006, 0.02, 0.540);
	cue->set_position(vector3(0.0, 0.10, 0.0));

	// Set up game.
	game = new Nine_ball(*this, players);
	
	update_player_names();
	update_player_scores();
}

Scene::~Scene() {
	if (camera)
		delete camera;

	if (table)
		delete table;

	if (light)
		delete light;

	if (cue)
		delete cue;

	if (game)
		delete game;
}

void Scene::update_light() {
	if (light) {
	//	camera->look();
		light->update();
	}
}

void Scene::display() const {
	camera->look();
	light->display();

	table->display();

	if (!simulating)
		cue->display();

	// Display user interface objects.
//	root_frame->display();

}

void Scene::set_camera_center(vector3 point) {
	camera->set_center(point);
	update_light();
}

void Scene::rotate_camera(double x, double y) {
	camera->rotate(vector3(x, y, 0.0));
	update_light();
}

void Scene::zoom_camera(double a) {
	camera->zoom(a);
	update_light();
}

void Scene::update_textures() {
	table->update_texture();
	cue->update_texture();

//	for (std::vector<GL_object*>::iterator p = objs.begin(); p != objs.end(); p++) {
//		(*p)->update_texture();
//	}
}

void Scene::update() {
	if (camera)
		camera->look();

	update_textures();
	update_light();
}

Table& Scene::get_table() const {
	return *table;
}

Game& Scene::get_game() const {
	return *game;
}

void Scene::set_cue_ball(GL_phys_sphere* ball) {
	cue->set_cue_ball(ball);
}

void Scene::set_cue_offset(vector3 offset) {
	cue->set_offset(offset);
}

void Scene::add_cue_offset(vector3 offset) {
	vector3 old_offset = cue->get_offset();
	vector3 new_offset = old_offset + offset;
	cue->set_offset(new_offset);
}

void Scene::set_cue_angle(double angle) {
	cue->set_angle(angle);
}

void Scene::add_cue_angle(double angle) {
	cue->set_angle(cue->get_angle() + angle);
}

void Scene::set_cue_rotation(double angle) {
	cue->set_rotation(angle);
}

void Scene::add_cue_rotation(double angle) {
	cue->set_rotation(cue->get_rotation() + angle);
}

void Scene::on_stop_simulation() {
	std::cout << "Simulation has stopped" << std::endl;
	simulating = false;
	game->on_end_of_shot();
}

void Scene::start_simulation() {
	simulating = true;
	Anco_pool::get().start_simulation(table);
}

void Scene::update_player_names() {
	//CEGUI::Window* player_one_name_wnd = CEGUI::WindowManager::getSingleton().getWindow("playerOneName");
	//CEGUI::Window* player_two_name_wnd = CEGUI::WindowManager::getSingleton().getWindow("playerTwoName");
	
//	player_one_name_wnd->setText(game->get_player_name(0));
//	player_two_name_wnd->setText(game->get_player_name(1));
	
	switch (game->get_active_player_num()) {
		case 0:
	//		player_one_name_wnd->setAlpha(1.0);
	//		player_two_name_wnd->setAlpha(0.2);
			break;
		case 1:
	//		player_one_name_wnd->setAlpha(0.2);
	//		player_two_name_wnd->setAlpha(1.0);
			break;
	}
}
	

void Scene::update_player_scores() {
	std::ostringstream score;
	score << game->get_player_score(0) << " : " << game->get_player_score(1);
	//CEGUI::WindowManager::getSingleton().getWindow("score")->setText(score.str());
}

/*
int Scene::get_num_players() const {
	return players.size();
}

std::string Scene::get_player_name(int i) const {
	return players[i]->get_name();
}

int Scene::get_player_score(int i) const {
	return players[i]->get_score();
}

void Scene::set_player_score(int i, int score) {
	players[i]->set_score(score);
}
 */
