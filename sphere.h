#ifndef SPHERE_H
#define SPHERE_H

#include <string>
#include "object.h"

class Sphere : public virtual Object {
	public:
		Sphere(double radius);
		double get_radius() const;
		void rotate(vector3 a);
	
	private:
		double radius;
		vector3 rotation;
//		float rotation_matrix[16];
};

inline double Sphere::get_radius() const {
	return radius;
}


#endif // SPHERE_H
