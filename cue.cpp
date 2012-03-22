#include "cue.h"

#include <cmath>
#include "gl_quad.h"
#include "scene.h"
#include "resource_manager.h"

Cue::Cue(Scene& scene, double length, double top_radius, double bottom_radius, double mass) : scene(scene), length(length), top_radius(top_radius), bottom_radius(bottom_radius), mass(mass), cue_ball(0), rotation(0) {
	static const int num_quads = 16;
	static const double tip_length = 0.005;
	static const double ferrule_length = 0.025;

	Resource_manager& rmgr = Resource_manager::get();
	
	// Create tip.
	GL_object_container* tip = new GL_object_container;
	for (int i=0; i<num_quads; i++) {
		double alpha = (double)i/num_quads * 2*M_PI;
		double beta = (double)(i+1)/num_quads * 2*M_PI;

		vector3 v0 = vector3(cos(alpha)*top_radius, -sin(alpha)*top_radius, 0.0);
		vector3 v1 = vector3(cos(alpha)*top_radius, -sin(alpha)*top_radius, tip_length);
		vector3 v2 = vector3(cos(beta)*top_radius, -sin(beta)*top_radius, tip_length);
		vector3 v3 = vector3(cos(beta)*top_radius, -sin(beta)*top_radius, 0.0);

		vector3 n0 = vector3(cos(alpha), -sin(alpha), 0.0);
		vector3 n1 = vector3(cos(beta), -sin(beta), 0.0);

		vector2 t0 = vector2((double)i/num_quads, 1.0);
		vector2 t1 = vector2((double)i/num_quads, 0.0);
		vector2 t2 = vector2((double)(i+1)/num_quads, 0.0);
		vector2 t3 = vector2((double)(i+1)/num_quads, 1.0);

		GL_quad* quad = new GL_quad(v0, v1, v2, v3, t0, t1, t2, t3, n0, n0, n1, n1);
		tip->push_back(quad);
	}
	tip->set_texture(rmgr.get_resource("cue_tip.png"));
	push_back(tip);

	// Create ferrule.
	GL_object_container* ferrule = new GL_object_container;
	for (int i=0; i<num_quads; i++) {
		double alpha = (double)i/num_quads * 2*M_PI;
		double beta = (double)(i+1)/num_quads * 2*M_PI;

		vector3 v0 = vector3(cos(alpha)*top_radius, -sin(alpha)*top_radius, tip_length);
		vector3 v1 = vector3(cos(alpha)*top_radius, -sin(alpha)*top_radius, tip_length + ferrule_length);
		vector3 v2 = vector3(cos(beta)*top_radius, -sin(beta)*top_radius, tip_length + ferrule_length);
		vector3 v3 = vector3(cos(beta)*top_radius, -sin(beta)*top_radius, tip_length);

		vector3 n0 = vector3(cos(alpha), -sin(alpha), 0.0);
		vector3 n1 = vector3(cos(beta), -sin(beta), 0.0);

		vector2 t0 = vector2((double)i/num_quads, 1.0);
		vector2 t1 = vector2((double)i/num_quads, 0.0);
		vector2 t2 = vector2((double)(i+1)/num_quads, 0.0);
		vector2 t3 = vector2((double)(i+1)/num_quads, 1.0);
//		GL_quad* quad = new GL_quad(v0, v1, v2, v3, t0, t1, t2, t3);

		GL_quad* quad = new GL_quad(v0, v1, v2, v3, t0, t1, t2, t3, n0, n0, n1, n1);
		ferrule->push_back(quad);
	}
	ferrule->set_texture(rmgr.get_resource("cue_ferrule.png"));
	push_back(ferrule);


	GL_object_container* bottom = new GL_object_container;
	for (int i=0; i<num_quads; i++) {
		double alpha = (double)i/num_quads * 2*M_PI;
		double beta = (double)(i+1)/num_quads * 2*M_PI;

		vector3 v0 = vector3(cos(alpha)*top_radius, -sin(alpha)*top_radius, tip_length + ferrule_length);
		vector3 v1 = vector3(cos(alpha)*bottom_radius, -sin(alpha)*bottom_radius, length);
		vector3 v2 = vector3(cos(beta)*bottom_radius, -sin(beta)*bottom_radius, length);
		vector3 v3 = vector3(cos(beta)*top_radius, -sin(beta)*top_radius, tip_length + ferrule_length);

		vector3 n0 = vector3(cos(alpha), -sin(alpha), 0.0);
		vector3 n1 = vector3(cos(beta), -sin(beta), 0.0);

		vector2 t0 = vector2((double)i/num_quads, 1.0);
		vector2 t1 = vector2((double)i/num_quads, 0.0);
		vector2 t2 = vector2((double)(i+1)/num_quads, 0.0);
		vector2 t3 = vector2((double)(i+1)/num_quads, 1.0);

		GL_quad* quad = new GL_quad(v0, v1, v2, v3, t0, t1, t2, t3, n0, n0, n1, n1);
		bottom->push_back(quad);
	}
	bottom->set_texture(rmgr.get_resource("cue2.png"));
	push_back(bottom);
}

void Cue::display() const {
	glPushMatrix();
	GL_object::display();
	glRotatef(rotation, 0.0, 1.0, 0.0);
	glRotatef(-5.0, 1.0, 0.0, 0.0);
	glTranslatef(offset.x, offset.y, offset.z);
	display_objects();
	glPopMatrix();
}

void Cue::set_offset(vector3 new_offset) {
	if (cue_ball) {
		std::cerr << new_offset.z << std::endl;
		if (new_offset.z < cue_ball->get_radius()) {
//		if (new_offset.abs() < cue_ball->get_radius()) {
			double rad = rotation * M_PI / 180.0;

			double delta_z = offset.z - new_offset.z;
			double power = delta_z * 50.0;

			// TODO: richtig berechnen. abhängig von masse des queues usw.

			vector3 velocity = vector3(-sin(rad) * power, 0.0, -cos(rad) * power);
			cue_ball->set_velocity(velocity);
			scene.start_simulation();
		}
	}

	offset = new_offset;
}

vector3 Cue::get_offset() const {
	return offset;
}

void Cue::set_angle(double a) {
	angle = a;
}

double Cue::get_angle() const {
	return angle;
}

// Set new cue ball and set cue tip position at a point that's double of the radius away from the center of that ball.
void Cue::set_cue_ball(GL_phys_sphere* ball) {
	cue_ball = ball;
	set_position(cue_ball->get_position());
	set_offset(vector3(0, 0, cue_ball->get_radius() * 2));
}

void Cue::set_rotation(double r) {
	rotation = r;
}

double Cue::get_rotation() const {
	return rotation;
}
