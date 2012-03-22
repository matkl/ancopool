#ifndef VECTOR_H
#define VECTOR_H

#include <ostream>

class vector2d {
	public:
		vector2d(double x=0.0, double y=0.0);

		vector2d& operator+= (const vector2d& a);
		vector2d& operator*= (const double& a);
		vector2d& operator/= (const double& a);

		double abs() const;
		vector2d normalize() const;

		double x, y;
};

typedef vector2d vector2;

vector2d operator- (const vector2d& a);
vector2d operator- (const vector2d& a, const vector2d& b);
vector2d operator+ (const vector2d& a, const vector2d& b);
vector2d operator* (const vector2d& a, const double& b);
vector2d operator* (const double& a, const vector2d& b);
vector2d operator/ (const vector2d& a, const double& b);


class vector3d {
	public:
		vector3d(double x=0.0, double y=0.0, double z=0.0);

		vector3d& operator+= (const vector3d& a);
		vector3d& operator*= (const double& a);
		vector3d& operator/= (const double& a);

//		double dot(const vector3d& a) const;
		double abs() const;
		double distance(const vector3d& a) const;

		vector3d rotate(double angle, const vector3d& a) const; // a is the axis (unit vector)
		vector3d normalY() const; // normal in the y-plane

		vector3d normalize() const;

		double x, y, z;

	public:
//		static vector3d quadNormal(const vector3d& a, const vector3d& b, const vector3d& c, const vector3d& d);
};

typedef vector3d vector3;

vector3d operator- (const vector3d& a);	//!< unary minus
vector3d operator- (const vector3d& a, const vector3d& b);
vector3d operator+ (const vector3d& a, const vector3d& b);
// double operator* (const vector3d& a, const vector3d& b);
vector3d operator* (const vector3d& a, const double& b);
vector3d operator* (const double& a, const vector3d& b);
vector3d operator/ (const vector3d& a, const double& b);

std::ostream& operator<< (std::ostream& ostr, const vector3d& a);

double dot(const vector3d& a, const vector3d& b);
vector3d cross(const vector3d& a, const vector3d& b);

#endif // VECTOR_H
