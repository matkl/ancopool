#include "table.h"

#include "game.h"

Table::Table(Scene& scene, double ball_radius, double ball_mass) : scene(scene), ball_radius(ball_radius), ball_mass(ball_mass) {
}

Table::~Table() {
	for (std::vector<Phys_quad*>::iterator p = event_quads.begin(); p != event_quads.end(); p++) {
		delete *p;
	}
}

void Table::set_cloth_texture(const std::string& filename) {
	top_quads.set_texture(filename);
	rail_quads.set_texture(filename);

//	for (std::vector<GL_phys_quad*>::iterator p = top_quads.begin(); p != top_quads.end(); p++) {
//		GL_phys_quad* quad = *p;
//		quad->set_texture(filename);
//	}

//	for (std::vector<GL_phys_quad*>::iterator p = rail_quads.begin(); p != rail_quads.end(); p++) {
//		GL_phys_quad* quad = *p;
//		quad->set_texture(filename);
//	}
}

void Table::set_frame_texture(const std::string& filename) {
	frame_quads.set_texture(filename);
//	for (std::vector<GL_phys_quad*>::iterator p = frame_quads.begin(); p != frame_quads.end(); p++) {
//		GL_quad* quad = *p;
//		quad->set_texture(filename);
//	}
}

void Table::set_pocket_texture(const std::string& filename) {
	pocket_quads.set_texture(filename);
//	for (std::vector<GL_phys_quad*>::iterator p = pocket_quads.begin(); p != pocket_quads.end(); p++) {
//		GL_quad* quad = *p;
//		quad->set_texture(filename);
//	}
}

double Table::get_ball_radius() const {
	return ball_radius;
}

double Table::get_ball_mass() const {
	return ball_mass;
}

void Table::set_ball_radius(double radius) {
	ball_radius = radius;
}

void Table::set_ball_mass(double mass) {
	ball_mass = mass;
}

GL_phys_sphere* Table::create_ball(const std::string& texture) {
	GL_phys_sphere* ball = new GL_phys_sphere(ball_radius, ball_mass);
	ball->set_texture(texture);
	add_ball(ball);

	return ball;
}


void Table::add_top_quad(GL_phys_quad* quad) {
//	GL_quads::push_back(quad);
	Phys_simulator::register_quad(quad);
	top_quads.push_back(quad);
}

void Table::add_rail_quad(GL_phys_quad* quad) {
	quad->set_quad_type(QUAD_TYPE_RAIL);
//	GL_quads::push_back(quad);
	Phys_simulator::register_quad(quad);
	rail_quads.push_back(quad);
}

void Table::add_frame_quad(GL_phys_quad* quad) {
//	GL_quads::push_back(quad);
	Phys_simulator::register_quad(quad);
	frame_quads.push_back(quad);
}

void Table::add_pocket_quad(GL_phys_quad* quad) {
//	GL_quads::push_back(quad);
	Phys_simulator::register_quad(quad);
	pocket_quads.push_back(quad);
}

void Table::add_ball(GL_phys_sphere* ball) {
//	GL_quads::push_back(ball);
	Phys_simulator::register_sphere(ball);
	balls.push_back(ball);
}

void Table::add_event_quad(Phys_quad* quad) {
	Phys_simulator::register_quad(quad);
	event_quads.push_back(quad);
}

void Table::set_top_restitution(double cor) {
	set_restitution(top_quads, cor);
}

void Table::set_rail_restitution(double cor) {
	set_restitution(rail_quads, cor);
}

void Table::set_frame_restitution(double cor) {
	set_restitution(frame_quads, cor);
}

void Table::set_pocket_restitution(double cor) {
	set_restitution(pocket_quads, cor);
}

void Table::set_head_spot(vector3 spot) {
	head_spot = spot;
}

void Table::set_foot_spot(vector3 spot) {
	foot_spot = spot;
}

vector3 Table::get_head_spot() const {
	return head_spot;
}

vector3 Table::get_foot_spot() const {
	return foot_spot;
}

void Table::on_collision_event(Phys_collision_type type_id, Phys_object* obj1, Phys_object* obj2) {
	scene.get_game().on_collision_event(type_id, obj1, obj2);
}

void Table::set_restitution(GL_object_container& quads, double cor) {
	for (std::vector<GL_object*>::iterator p = quads.begin(); p != quads.end(); p++) {
		GL_phys_quad* quad = static_cast<GL_phys_quad*>(*p);
		quad->set_coefficient_of_restitution(cor);
	}
}

void Table::display() const {
	top_quads.display();
	rail_quads.display();
	pocket_quads.display();
	frame_quads.display();
	balls.display();

//	for (std::vector<GL_phys_sphere*>::iterator p = balls.begin(); p != balls.end(); p++) {
//		GL_phys_sphere* ball = *p;
//		ball->display();
//	}
}
