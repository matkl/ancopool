#ifndef CAMERA_H
#define CAMERA_H

#define MIN_DISTANCE 0.20f
#define MAX_DISTANCE 5.00f
#define ZOOM_STEP 0.025f

#include <cmath>
#include "vector.h"

class Camera {
	public:
		Camera();

		void look() const;

		void set_center(vector3 point);
		void rotate(vector3 a);
		void zoom(double a);

	private:
		vector3 center;
		vector3 rotation;
		double distance;
};


#if 0
class Camera {
	public:
		Camera(double centerX, double centerY, double centerZ, double rotateX, double rotateY, double rotateZ, double dist);
		Camera();

		void set_center(vector3 point);
		void rotate(vector3 a);


		void center(double x, double y, double z);
		void translate(double x, double y, double z);
		void distance(double d);
		void move(double d);
		void rotate(double x, double y, double z);
		void look();

	private:
		vector3 center;

		double dist;
		double centerX;
		double centerY;
		double centerZ;
		double rotateX;
		double rotateY;
		double rotateZ;
};
#endif

#endif // CAMERA_H

