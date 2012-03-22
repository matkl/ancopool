#include "vector.h"

#include <cmath>

vector2d::vector2d(double x, double y) : x(x), y(y) {
}

vector2d& vector2d::operator+= (const vector2d& a) {
	x += a.x;
	y += a.y;
	return *this;
}

vector2d& vector2d::operator*= (const double& a) {
	x *= a;
	y *= a;
	return *this;
}

vector2d& vector2d::operator/= (const double& a) {
	x /= a;
	y /= a;
	return *this;
}

vector2d operator- (const vector2d& a) {
	vector2d r = a;
	return r *= -1.0;
}

vector2d operator- (const vector2d& a, const vector2d& b) {
	vector2d r = a;
	return r += -b;
}

vector2d operator+ (const vector2d& a, const vector2d& b) {
	vector2d r = a;
	return r += b;
}

vector2d operator* (const vector2d& a, const double& b) {
	vector2d r = a;
	return r *= b;
}

vector2d operator* (const double& a, const vector2d& b) {
	vector2d r = b;
	return r *= a;
}

vector2d operator/ (const vector2d& a, const double& b) {
	vector2d r = a;
	return r /= b;
}

double vector2d::abs() const {
	double r = std::sqrt(x*x + y*y);
	return r;
}

vector2d vector2d::normalize() const {
	vector2d r = *this;
	double l = abs();

	if (l > 0.0) {
		r /= l;
	}

	return r;
}


vector3d::vector3d(double x, double y, double z) : x(x), y(y), z(z) {
}

vector3d& vector3d::operator+= (const vector3d& a) {
	x += a.x;
	y += a.y;
	z += a.z;
	return *this;
}

vector3d& vector3d::operator*= (const double& a) {
	x *= a;
	y *= a;
	z *= a;
	return *this;
}

vector3d& vector3d::operator/= (const double& a) {
	x = x/a;
	y = y/a;
	z = z/a;
	return *this;
}
/*
vector3d vector3d::operator- (const vector3d& a) const {
	vector3d r = a;
	return r *= -1.0;
}
*/
double vector3d::abs() const {
	using std::sqrt;
	double r = sqrt(x*x + y*y + z*z);
	return r;
}

double dot(const vector3d& a, const vector3d& b) {
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}

double vector3d::distance(const vector3d& a) const {
	using std::sqrt;
	double r = sqrt((a.x-x)*(a.x-x) + (a.y-y)*(a.y-y) + (a.z-z)*(a.z-z));
	return r;
}

vector3d vector3d::rotate(double angle, const vector3d& a) const {
	double alpha = angle * M_PI/180.0;
	double cos_alpha = cos(alpha);
	double sin_alpha = sin(alpha);
	vector3d r;
	r.x = x * (cos_alpha + a.x*a.x*(1 - cos_alpha))
		+ y * (a.x*a.y*(1 - cos_alpha) - a.z*sin_alpha)
		+ z * (a.x*a.z*(1 - cos_alpha) + a.y*sin_alpha);
	r.y = x * (a.y*a.x*(1 - cos_alpha) + a.z*sin_alpha)
		+ y * (cos_alpha + a.y*a.y*(1 - cos_alpha))
		+ z * (a.y*a.z*(1 - cos_alpha) - a.x*sin_alpha);
	r.z = x * (a.z*a.x*(1 - cos_alpha) - a.y*sin_alpha)
		+ y * (a.z*a.y*(1 - cos_alpha) + a.x*sin_alpha)
		+ z * (cos_alpha + a.z*a.z*(1 - cos_alpha));

	return r;
}

vector3d vector3d::normalY() const {
	vector3d r = *this;
	r.x = -z;
	r.z = x; // TODO: hm
	return r;
}

vector3d vector3d::normalize() const {
	vector3d r = *this;
	double l = abs();

	if (l > 0.0) {
		r /= l;
	}

	return r;
}


vector3d operator- (const vector3d& a) {
	vector3d r = a;
	return r *= -1.0;
}

vector3d operator- (const vector3d& a, const vector3d& b) {
	vector3d r = a;
	return r += -b;
}

vector3d operator+ (const vector3d& a, const vector3d& b) {
	vector3d r = a;
	return r += b;
}

/**
 * vector product.
 */
#if 0
vector3d operator* (const vector3d& a, const vector3d& b) {
	vector3d r;
	r.x = a.y*b.z - a.z*b.y;
	r.y = a.z*b.x - a.x*b.z;
	r.z = a.x*b.y - a.y*b.x;
	return r;
}
#endif

vector3d operator* (const vector3d& a, const double& b) {
	vector3d r = a;
	return r *= b;
}

vector3d operator* (const double& a, const vector3d& b) {
	vector3d r = b;
	return r *= a;
}

vector3d operator/ (const vector3d& a, const double& b) {
	vector3d r = a;
	return r /= b;
}

std::ostream& operator<<(std::ostream& ostr, const vector3d& a) {
	ostr << "(";
   	ostr << a.x;
	ostr << ",";
    ostr << a.y;
    ostr << ",";
	ostr << a.z;
	ostr << ")";
	return ostr;
}

#if 0
// static functions
vector3d vector3d::quadNormal(const vector3d& a, const vector3d& b, const vector3d& c, const vector3d& d) {
	vector3d normal;

	normal += a * b;
	normal += b * c;
	normal += c * d;
	normal += d * a;

	normal.normalize();

	return normal;
}
#endif

vector3d cross(const vector3d& a, const vector3d& b) {
	vector3d r;
	r.x = a.y*b.z - a.z*b.y;
	r.y = a.z*b.x - a.x*b.z;
	r.z = a.x*b.y - a.y*b.x;
	return r;
}
