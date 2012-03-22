#ifndef TABLE_H
#define TABLE_H

#include <string>
#include <vector>
#include <iostream>
#include "gl_object_container.h"
#include "phys_simulator.h"
#include "gl_phys_quad.h"
#include "gl_phys_sphere.h"
#include "scene.h"

class Scene;

class Table : public Phys_simulator, public GL_object_container {
	public:
		Table(Scene& scene, double ball_radius, double ball_mass);
		~Table();

		void set_cloth_texture(const std::string& filename);
		void set_frame_texture(const std::string& filename);
		void set_pocket_texture(const std::string& filename);

		double get_ball_radius() const;
		double get_ball_mass() const;
		void set_ball_radius(double radius);
		void set_ball_mass(double mass);

		vector3 get_head_spot() const;
		vector3 get_foot_spot() const;

		GL_phys_sphere* create_ball(const std::string& texture);

		void display() const;

	protected:
		void add_top_quad(GL_phys_quad* quad);
		void add_rail_quad(GL_phys_quad* quad);
		void add_frame_quad(GL_phys_quad* quad);
		void add_pocket_quad(GL_phys_quad* quad);
		void add_event_quad(Phys_quad* quad);
		void add_ball(GL_phys_sphere* ball);

		void set_top_restitution(double cor);
		void set_rail_restitution(double cor);
		void set_frame_restitution(double cor);
		void set_pocket_restitution(double cor);

		void set_head_spot(vector3 spot);
		void set_foot_spot(vector3 spot);

	private:
		virtual void on_collision_event(Phys_collision_type type_id, Phys_object* obj1, Phys_object* obj2);

		void set_restitution(GL_object_container& quads, double cor);

		GL_object_container balls;
//		std::vector<GL_phys_sphere*> balls;
		GL_object_container top_quads;
		GL_object_container rail_quads;
		GL_object_container pocket_quads;
		GL_object_container frame_quads;

		std::vector<Phys_quad*> event_quads;

		Scene& scene;

		double ball_radius;
		double ball_mass;

		vector3 head_spot;
		vector3 foot_spot;
};


#endif // TABLE_H
