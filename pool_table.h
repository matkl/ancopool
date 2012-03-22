#ifndef POOL_TABLE_H
#define POOL_TABLE_H

#include "table.h"

class Pool_table : public Table {
	public:
		Pool_table(Scene& scene, double size_in_feet=9.0);
		virtual void display() const;

	private:
		void create_board(double width, double length, double middle_pocket_radius, double corner_pocket_radius, double middle_pocket_angle, double corner_pocket_angle, double rail_width, double rail_height);
		void create_frame(double width, double length, double height, double middle_pocket_radius, double corner_pocket_radius, double cushion_width, double cushion_height);

		void create_board_part(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double pocket_radius);
		void create_rail(vector3 from, vector3 to, double start_angle, double end_angle, double width, double height, double bottom_width, double inner_start, double inner_height);
		void create_frame_part(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double curve_radius=0.0);
		void create_frame_middle_pocket(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double pocket_radius, double curve_radius, enum Quad_type pocket_name);
		void create_frame_corner_pocket(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double pocket_radius, double curve_radius, double pocket_spacing, enum Quad_type pocket_name);
		



//		void addRail(const vector3& from, const vector3& to, double start_angle, double end_angle, double width, double height, double bottom_width, double inner_start, double inner_height);

		double size;

		static const double default_ball_radius;
		static const double default_ball_mass;
		static const double default_rail_width;
		static const double default_rail_height;
		static const double default_middle_pocket_radius;
		static const double default_corner_pocket_radius;
		static const double default_middle_pocket_angle;
		static const double default_corner_pocket_angle;
		static const double default_cushion_width;
		static const double default_frame_height;
		static const double foot_in_m;
};



#endif // POOL_TABLE_H
