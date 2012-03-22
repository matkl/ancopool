#ifndef PHYS_QUAD
#define PHYS_QUAD

#include "quad.h"
#include "aabb.h"
#include "phys_object.h"

enum Quad_type {
	QUAD_TYPE_NONE = 0,
	QUAD_TYPE_RAIL = 0x1,
	QUAD_TYPE_OUT = 0x2,
	QUAD_TYPE_POCKET = 0x8,
	QUAD_TYPE_POCKET_FOOT_LEFT = 0x9,
	QUAD_TYPE_POCKET_FOOT_RIGHT = 0xa,
	QUAD_TYPE_POCKET_MIDDLE_LEFT = 0xb,
	QUAD_TYPE_POCKET_MIDDLE_RIGHT = 0xc,
	QUAD_TYPE_POCKET_HEAD_LEFT = 0xd,
	QUAD_TYPE_POCKET_HEAD_RIGHT = 0xe,
};

class Phys_quad : public virtual Quad, public Phys_object {
	public:
//		Phys_quad();
		Phys_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, enum Quad_type quad_type = QUAD_TYPE_NONE);
	
		Aabb get_aabb() const;
		double get_coefficient_of_restitution() const;
		double get_coefficient_of_rolling_friction() const;
		double get_coefficient_of_sliding_friction() const;
		enum Quad_type get_quad_type() const;
		void set_coefficient_of_restitution(double cor);
		void set_coefficient_of_rolling_friction(double mu_rolling);
		void set_coefficient_of_sliding_friction(double mu_sliding);
		void set_quad_type(enum Quad_type qt);

	private:
		Aabb aabb;
		double coefficient_of_restitution;
		double coefficient_of_rolling_friction; // mu_rolling
		double coefficient_of_sliding_friction; // mu_sliding
		enum Quad_type quad_type;
};

#endif // PHYS_QUAD
