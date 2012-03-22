#ifndef PHYS_SIMULATOR_H
#define PHYS_SIMULATOR_H

#include <vector>
#include "phys_quad.h"
#include "phys_sphere.h"

enum Phys_collision_type {
	PHYS_COLLISION_NONE = 0,
	PHYS_COLLISION_SPHERE_QUAD,
	PHYS_COLLISION_SPHERE_SPHERE
};

struct Phys_collision_data {
	Phys_collision_type type;
	double time;
	Phys_object* obj1;
	Phys_object* obj2;
	vector3 quad_intersection_point;
};

class Phys_simulator {
	public:
		Phys_simulator();
		bool simulate(double time_step);
		void set_gravity(double g);
		void set_gravity(vector3 gv);

	protected:
		void register_quad(Phys_quad* quad);
		void register_sphere(Phys_sphere* sphere);

	private:
		void collision_detection(double time, double time_step, int num_recursions = 0);
		void move_spheres(double time);

		void collide(const Phys_collision_data& collision, double time_step);
		void collide(Phys_sphere* sphere, Phys_quad* quad, vector3 quad_intersection_point, double time_step);
		void collide(Phys_sphere* sphere1, Phys_sphere* sphere2);
		double apply_friction(Phys_sphere* sphere, const Phys_quad* quad, vector3 slide_plane_normal, double time);
		double apply_sliding_friction(Phys_sphere* sphere, const Phys_quad* quad, vector3 slide_plane_normal, double time);
		double apply_rolling_friction(Phys_sphere* sphere, const Phys_quad* quad, vector3 slide_plane_normal, double time);

		static double intersect_ray_plane(vector3 ray_origin, vector3 ray_vector, vector3 plane_origin, vector3 plane_normal);
		static vector3 closest_point_on_line(vector3 a, vector3 b, vector3 p);
		static vector3 closest_point_to_point(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p);
		static vector3 closest_point_on_quad(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p);
		static double intersect_ray_sphere(vector3 ray_origin, vector3 ray_vector, vector3 sphere_origin, double sphere_radius);
		static bool is_point_inside_quad(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p, vector3 n);

		virtual void on_collision_event(Phys_collision_type type_id, Phys_object* obj1, Phys_object* obj2) = 0;

		std::vector<Phys_quad*> quads;
		std::vector<Phys_sphere*> spheres;
		vector3 gravity;

		static const vector3 default_gravity;
};

#endif // PHYS_SIMULATOR
