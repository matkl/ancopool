#include "phys_simulator.h"

#include <cmath>
#include <iostream>

using std::cout;
using std::cerr;
using std::endl;

const vector3 Phys_simulator::default_gravity = vector3(0.0, -9.81, 0.0);
// const vector3 Phys_simulator::default_gravity = vector3(0.0, 0.0, 0.0);

Phys_simulator::Phys_simulator() : gravity(default_gravity) {
}

double Phys_simulator::intersect_ray_plane(vector3 ray_origin, vector3 ray_vector, vector3 plane_origin, vector3 plane_normal) {
	double d = -dot(plane_normal, plane_origin);
	double numer = dot(plane_normal, ray_origin) + d;
	double denom = dot(plane_normal, ray_vector);
	return -(numer/denom);
}

vector3 Phys_simulator::closest_point_on_line(vector3 a, vector3 b, vector3 p) {
	// Determine t (the length of the vector from 'a' to 'p').
	vector3 c = p - a;
	vector3 V = (b - a).normalize();
	double d = a.distance(b);
	double t = dot(V, c);

	// Check to see if 't' is beyond the extents of the line segment.
	if (t < 0) return a;
	if (t > d) return b;

	// Return the point between 'a' and 'b'.
	return a + V*t;
}

// TODO: Optimization.
vector3 Phys_simulator::closest_point_to_point(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p) {
	double Da = p.distance(a);
	double Db = p.distance(b);
	double Dc = p.distance(c);
	double Dd = p.distance(d);

	if (Da < Db && Da < Dc && Da < Dd)
		return a;
	if (Db < Dc && Db < Dd)
		return b;
	if (Dc < Dd)
		return c;

	return d;
}

vector3 Phys_simulator::closest_point_on_quad(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p) {
	vector3 Rab = closest_point_on_line(a, b, p);
	vector3 Rbc = closest_point_on_line(b, c, p);
	vector3 Rcd = closest_point_on_line(c, d, p);
	vector3 Rda = closest_point_on_line(d, a, p);

	return closest_point_to_point(Rab, Rbc, Rcd, Rda, p);
}

double Phys_simulator::intersect_ray_sphere(vector3 ray_origin, vector3 ray_vector, vector3 sphere_origin, double sphere_radius) {
	vector3 Q = sphere_origin - ray_origin;
	double c = Q.abs();
	double v = dot(Q, ray_vector);
	double d = sphere_radius*sphere_radius - (c*c - v*v);

	// If there was no intersection, return -1
	if (d < 0.0) return -1.0;

	// Return the distance to the [first] intersecting point.
	return v - sqrt(d);
}

// Returns true if a point is on the inside of four planes spanned by a quad.
bool Phys_simulator::is_point_inside_quad(vector3 a, vector3 b, vector3 c, vector3 d, vector3 p, vector3 n) {
	vector3 normal;

	// Calculate plane normal, pointing inside the quad.
	normal = cross((b - a), n);
	// Calculate dot product. If positive, the point is on the side of the plane that is pointing towards the
	// quad. If it is negative, the point can't be inside our quad, so bail.
	if (dot(a - p, normal) < 0.0) return false;

	normal = cross((c - b), n);
	if (dot(b - p, normal) < 0.0) return false;

	normal = cross((d - c), n);
	if (dot(c - p, normal) < 0.0) return false;

	normal = cross((a - d), n);
	if (dot(d - p, normal) < 0.0) return false;

	return true;
}

#define EPSILON 0.000001	// 10^-6 m

bool Phys_simulator::simulate(double time_step) {
	// We need to do any pre-collision detection work here. Such as adding
	// gravity to our velocity vector. We want to do it in this sperate
	// routine because the following routine is recursive, and we don't want
	// to recursively addd gravity.
	
	for (std::vector<Phys_sphere*>::iterator p = spheres.begin(); p != spheres.end(); p++) {
		Phys_sphere* sphere = *p;
		if (!sphere->is_enabled())
			continue;

		sphere->accelerate(gravity, time_step);
	}
	collision_detection(time_step, time_step);

	for (std::vector<Phys_sphere*>::iterator p = spheres.begin(); p != spheres.end(); p++) {
		Phys_sphere* sphere = *p;
		if (!sphere->is_enabled())
			continue;

		vector3 velocity = sphere->get_velocity();
		if (fabs(velocity.x) > EPSILON || fabs(velocity.y) > EPSILON || fabs(velocity.z) > EPSILON) {
			return true;
		}
	}

	return false;
}

#define COLLISION_SPHERE_SPHERE 1

void Phys_simulator::collision_detection(double time, double time_step, int num_recursions) {
	if (time <= 0) return;

	static const int max_recursions = 128;
	if (num_recursions > max_recursions) {
		cerr << "Phys_simulator::collision_detection(): Maximum number of recursions reached (" << max_recursions << "). Aborting simulation." << endl;
		for (std::vector<Phys_sphere*>::iterator p = spheres.begin(); p != spheres.end(); p++) {
			Phys_sphere* sphere = *p;
			if (!sphere->is_enabled())
				continue;

			sphere->set_velocity(vector3(0.0, 0.0, 0.0));
			sphere->set_angular_velocity(vector3(0.0, 0.0, 0.0));
		}
		return;
	}

	Phys_collision_data collision;
	collision.type = PHYS_COLLISION_NONE;

//	cout << "===" << endl;
	for (std::vector<Phys_sphere*>::iterator p = spheres.begin(); p != spheres.end(); p++) {
		Phys_sphere* sphere = *p;
		if (!sphere->is_enabled())
			continue;

		vector3 source_point = sphere->get_position();
		// cout << source_point << endl;
		vector3 velocity_vector = sphere->get_velocity()*time;

		// How far do we need to go?
		double distance_to_travel = velocity_vector.abs();
	
		// Do we need to bother?
		if (distance_to_travel < EPSILON) continue;
	
		// What's our destination?
		vector3 destination_point = source_point + velocity_vector;
	
		// The radius of the sphere
		double radius = sphere->get_radius();

		// Whom might we collide with?
		std::vector<Phys_quad*> potential_colliders;

		// Create an AABB that contains the sphere at the source point
		// and at the destination point (and the space between).
		Aabb path_aabb = Aabb(source_point, radius);
		path_aabb.merge(Aabb(destination_point, radius));
		for (std::vector<Phys_quad*>::iterator q = quads.begin(); q != quads.end(); q++) {
			Phys_quad* quad = *q;
			const Aabb& quad_aabb = quad->get_aabb();

			if (path_aabb.intersect(quad_aabb)) {
				potential_colliders.push_back(quad);
			}
		}

		// Determine the nearest collider from the list potential_colliders.
		for (std::vector<Phys_quad*>::iterator q = potential_colliders.begin(); q != potential_colliders.end(); q++) {
			Phys_quad* current_collider = *q;
	
			// Plane origin/normal
			vector3 a = current_collider->get_vertex(0);
			vector3 b = current_collider->get_vertex(1);
			vector3 c = current_collider->get_vertex(2);
			vector3 d = current_collider->get_vertex(3);
	
			vector3 plane_origin = a;
			vector3 plane_normal = current_collider->get_normal();
		
			// Determine the distance from the plane to the source.
			double plane_distance = intersect_ray_plane(plane_origin, plane_normal, source_point, -plane_normal);
			vector3 sphere_intersection_point;
			vector3 plane_intersection_point;
		
	
			// Is the plane embedded?
			if (fabs(plane_distance) <= radius) {
				// Calculate the plane intersection point.
				vector3 temp = -plane_normal * radius;
				plane_intersection_point = source_point + temp;
			} else {
				// Determine the point on the surface of the sphere that will eventually
				// intersect the plane.
				// We do this by first setting the length of the plane's normal to the
				// radius of the sphere, and then we invert it.
				vector3 temp = -plane_normal * radius;
				sphere_intersection_point = source_point + temp;
		
				// Calculate the plane intersection point.
				double t = intersect_ray_plane(sphere_intersection_point, velocity_vector.normalize(), plane_origin, plane_normal);
		
				// Calculate the plane intersection point.
				vector3 V = velocity_vector.normalize()*t;
				plane_intersection_point = sphere_intersection_point + V;
			}
		
		
		
			// Unless otherwise stated, our polygon_intersection_point is the same
			// point as plane_intersection_point.
			vector3 polygon_intersection_point = plane_intersection_point;
		
			bool inside = true;
		
			// So... are they the same?
			if (!is_point_inside_quad(a, b, c, d, plane_intersection_point, plane_normal)) {
				// Determine the nearest point on polygon's perimeter to
				// plane_intersection_point.
				polygon_intersection_point = closest_point_on_quad(a, b, c, d, plane_intersection_point);
			}
		
			// Invert the velocity vector.
			vector3 negative_velocity = -velocity_vector;
		
			// Using the polygon_intersection_point, we need to reverse-intersect with
			// the sphere.
			double t = intersect_ray_sphere(polygon_intersection_point, negative_velocity.normalize(), source_point, radius);
		

			// Was there an intersection with the sphere?
			if (t >= 0.0 && t <= velocity_vector.abs()) {
				double time_until_collision = (t - EPSILON) / sphere->get_velocity().abs();
				
				vector3 V = negative_velocity.normalize()*t;
		
				// Where did we intersect the sphere?
				vector3 intersection_point = polygon_intersection_point + V;
	
				// Closest intersection thus far?
				if (collision.type == PHYS_COLLISION_NONE || time_until_collision < collision.time) {
//					cout << "pos = " << sphere->get_position() << endl;
					collision.type = PHYS_COLLISION_SPHERE_QUAD;
					collision.time = time_until_collision;
					collision.quad_intersection_point = polygon_intersection_point;
//					cout << "quad intersection point = " << polygon_intersection_point;
					collision.obj1 = sphere;
					collision.obj2 = current_collider;
//					nearest_intersection_point = intersection_point;
				}
			}
		}

		std::vector<Phys_sphere*>::iterator q = p;
		q++;
		for (; q != spheres.end(); q++) {
			Phys_sphere* other_sphere = *q;
			if (!other_sphere->is_enabled())
				continue;

			vector3 velocities = sphere->get_velocity() - other_sphere->get_velocity();
			vector3 movement_vector = velocities * time;
			double movement_vector_abs = movement_vector.abs();

			double distance = other_sphere->get_distance(*sphere);
			double sum_radii = other_sphere->get_radius() + sphere->get_radius();

			// Early escape test: if the length of the movement vector is less
			// than distance between the centers of these circles minus their
			// radii, there is no way they can hit.
			if (movement_vector_abs < distance - sum_radii)
				continue;

			vector3 velocities_normal = velocities.normalize();
			vector3 distance_vector = other_sphere->get_position() - sphere->get_position();
			double d = dot(velocities_normal, distance_vector);

			// Another early escape: make sure obj1 is moving towards obj2!
			// If the dot product between the movement vector and obj2-obj1 is
			// less than or equal to 0, obj1 isn't moving towards obj2.
			if (d <= 0.0)
				continue;

			double f = distance*distance - d*d;
			double sum_radii_squared = sum_radii*sum_radii;

			// Escape test: if the closest that obj1 will get to obj2 is more than
			// the sum of their radii, there is no way they are going to collide.
			if (f >= sum_radii_squared)
				continue;

			// We now have f and sum_radii, two sides of a right triangle.
			// Use these to find the third side, sqrt(a)
			double a = sum_radii_squared - f;

			// If there is no such triangle with sides length of sum_radii and
			// sqrt(a), a will probably be less than 0.
			// Better to check now than perform a square root of a negative number.
			if (a < 0.0)
				continue;

			// Therefore the distance the circle has to travel along the movement
			// vector is d - sqrt(a).
			double moving_distance = d - sqrt(a);

			// Finally, make sure that the distance obj1 has to move to touch obj2
			// is not greater than the magnitude of the movement vector.
			if (movement_vector_abs < moving_distance)
				continue;

			double t = moving_distance / movement_vector_abs;

//			if (t < 0.0 || t > 1.0) {
//				cout << "t nicht zwischen 0 und 1" << endl;
//				cout << "t=" << t << endl;
//				continue;
//			}

			double time_until_collision = time * t;

			if (collision.type == PHYS_COLLISION_NONE || time_until_collision < collision.time) {
				collision.type = PHYS_COLLISION_SPHERE_SPHERE;
				collision.time = time_until_collision;
				collision.obj1 = sphere;
				collision.obj2 = other_sphere;
			}
		}
	}

	
	// If we never found a collision, we can safely move to the destination
	// and bail.
	if (collision.type == PHYS_COLLISION_NONE) {
		move_spheres(time);
		return;
	}

//	cout << "collision.time = " << collision.time << endl;
//	cout << "collision.type = " << collision.type << endl;

	// Move all spheres to the time where the next collision occurs.
	move_spheres(collision.time);

	// Execute collision.
//	cout << collision.type << endl;
	collide(collision, time_step);
//	collide(nearest_sphere, nearest_quad, nearest_polygon_intersection_point, time_step);
	
	// Recursively collide.
	collision_detection(time - collision.time, time_step, num_recursions+1);
}

void Phys_simulator::move_spheres(double time) {
	for (std::vector<Phys_sphere*>::iterator p = spheres.begin(); p != spheres.end(); p++) {
		Phys_sphere* sphere = *p;
		if (!sphere->is_enabled())
			continue;

		vector3 source_point = sphere->get_position();
		vector3 velocity = sphere->get_velocity();
		vector3 destination_point = source_point + velocity * time;

		sphere->set_position(destination_point);
	}
}

void Phys_simulator::collide(const Phys_collision_data& collision, double time_step) {
	switch (collision.type) {
		case PHYS_COLLISION_SPHERE_SPHERE:
			{
				Phys_sphere* sphere1 = static_cast<Phys_sphere*>(collision.obj1);
				Phys_sphere* sphere2 = static_cast<Phys_sphere*>(collision.obj2);
				collide(sphere1, sphere2);
			}
			break;
		case PHYS_COLLISION_SPHERE_QUAD:
			{
				Phys_sphere* sphere = static_cast<Phys_sphere*>(collision.obj1);
				Phys_quad* quad = static_cast<Phys_quad*>(collision.obj2);
				collide(sphere, quad, collision.quad_intersection_point, time_step);
			}
			break;
	}

	Phys_sphere* s = static_cast<Phys_sphere*>(collision.obj1);
//	cout << s->get_velocity().abs() << " /// " << s->get_velocity() << endl;
}

void Phys_simulator::collide(Phys_sphere* sphere, Phys_quad* quad, vector3 quad_intersection_point, double time_step) {
	vector3 sphere_point = sphere->get_position();
	vector3 velocity = sphere->get_velocity();
	vector3 movement_direction = velocity.normalize();
	vector3 sphere_point_for_collision = sphere_point + movement_direction * EPSILON;
	
//	cerr << "sphere_point: " << sphere_point << " velocity: " << velocity << " sphere_point_for_collision: " << sphere_point_for_collision << endl;
	
	// Determine the sliding plane.
	vector3 slide_plane_origin = quad_intersection_point;
	vector3 slide_plane_normal = (quad_intersection_point - sphere_point_for_collision).normalize();

	// Determine the applied gravity (in one time_step) in the direction of the slide plane normal.
	double slide_plane_gravity = dot(slide_plane_normal, gravity*time_step);

	// Determine velocity in the direction of the slide plane normal.
	double slide_plane_velocity = dot(slide_plane_normal, velocity);

	// TODO: this is a hack. come up with a better solution for checking for bounce/slide.
	// Check: If the velocity in the direction of the slide plane normal is greater than the applied
	// gravity, then bounce the ball, else slide it on the slide plane.
	if (slide_plane_gravity < 0.00001 || slide_plane_velocity - slide_plane_gravity > 0.1) {
		// bounce it!
		cout << "BOUNCE" << endl;
		double e = quad->get_coefficient_of_restitution();
		vector3 collision_reaction = -(1 + e)*slide_plane_velocity*slide_plane_normal;
		vector3 new_velocity = velocity + collision_reaction;
		sphere->set_velocity(new_velocity);
	} else {
//		cout << "SLIDE" << endl;
		// Remove the sliding component from the velocity vector.
		vector3 S = slide_plane_velocity * slide_plane_normal;
		velocity += -S;

		sphere->set_velocity(velocity);

		apply_friction(sphere, quad, slide_plane_normal, time_step);
	}

	on_collision_event(PHYS_COLLISION_SPHERE_QUAD, sphere, quad);
}

void Phys_simulator::collide(Phys_sphere* sphere1, Phys_sphere* sphere2) {
	// Find the normalized vector n from the center of sphere1 to the center of sphere2.
	vector3 n = (sphere1->get_position() - sphere2->get_position()).normalize();

	vector3 v1 = sphere1->get_velocity();
	vector3 v2 = sphere2->get_velocity();
	double m1 = sphere1->get_mass();
	double m2 = sphere2->get_mass();

	// Find the length of the component of each of the movement vectors along n.
	double a1 = dot(v1, n);
	double a2 = dot(v2, n);

	// Using the optimized version,
	// optimzed_p = 2(a1 - a2)
	//              ----------
	//                m1 + m2
	double optimized_p = 2.0*(a1 - a2)/(m1 + m2);

	// Calculate the new movement vector of sphere1
	v1 += -optimized_p*m2*n;

	// Calculate the new movement vector of sphere2
	v2 += optimized_p*m1*n;

	sphere1->set_velocity(v1);
	sphere2->set_velocity(v2);

	on_collision_event(PHYS_COLLISION_SPHERE_SPHERE, sphere1, sphere2);
}



double Phys_simulator::apply_friction(Phys_sphere* sphere, const Phys_quad* quad, vector3 slide_plane_normal, double time) {
	double remaining_time = time;

	remaining_time -= apply_sliding_friction(sphere, quad, slide_plane_normal, remaining_time);
	if (remaining_time <= 0.0) return time;
	
	remaining_time -= apply_rolling_friction(sphere, quad, slide_plane_normal, remaining_time);
	if (remaining_time <= 0.0) return time;

//	cout << "STOP!" << endl;

	return time - remaining_time;
}


double Phys_simulator::apply_sliding_friction(Phys_sphere* sphere, const Phys_quad* quad, vector3 slide_plane_normal, double time) {
	double radius = sphere->get_radius();
	vector3 velocity = sphere->get_velocity();
	vector3 angular_velocity = sphere->get_angular_velocity();
	double mu_sliding = quad->get_coefficient_of_sliding_friction();

	// Determine the velocity on the sliding point.
	vector3 sliding_point_velocity = velocity - cross(slide_plane_normal, angular_velocity * radius);

	double slide_plane_gravity = dot(gravity, slide_plane_normal);

	// When will natural roll be achieved? (in seconds)
	double slide_time = 2.0/7.0 * sliding_point_velocity.abs() / (mu_sliding * slide_plane_gravity);
	if (slide_time > time) {
		slide_time = time;
	}

	vector3 acceleration = (mu_sliding * slide_plane_gravity * slide_time) * -sliding_point_velocity.normalize();
	vector3 angular_acceleration = cross(-2.5 * acceleration / radius, slide_plane_normal);

	velocity += acceleration;
	angular_velocity += angular_acceleration;

	sphere->set_velocity(velocity);
	sphere->set_angular_velocity(angular_velocity);

	return slide_time;
}

double Phys_simulator::apply_rolling_friction(Phys_sphere* sphere, const Phys_quad* quad, vector3 slide_plane_normal, double time) {
	double rg = sphere->get_radius_of_gyration();
	vector3 velocity = sphere->get_velocity();
	vector3 angular_velocity = sphere->get_angular_velocity();
	vector3 velocity_normal = velocity.normalize();
	double speed = velocity.abs();
	double mu_rolling = quad->get_coefficient_of_rolling_friction();

	double slide_plane_gravity = dot(gravity, slide_plane_normal);

	// When will the sphere stop? (seconds)
	double roll_time = speed / (mu_rolling * slide_plane_gravity);
	if (roll_time > time) {
		roll_time = time;
	}

	velocity += (mu_rolling * slide_plane_gravity * roll_time) * -velocity_normal;
	angular_velocity += cross((mu_rolling * slide_plane_gravity * roll_time) * -velocity_normal, slide_plane_normal) / rg;

	sphere->set_velocity(velocity);
	sphere->set_angular_velocity(angular_velocity);

	return roll_time;
}

void Phys_simulator::set_gravity(double g) {
	set_gravity(vector3(0.0, -g, 0.0));
}

void Phys_simulator::set_gravity(vector3 gv) {
	gravity = gv;
}

void Phys_simulator::register_quad(Phys_quad* quad) {
	quads.push_back(quad);
}

void Phys_simulator::register_sphere(Phys_sphere* sphere) {
	spheres.push_back(sphere);
}
