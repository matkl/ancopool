#include "pool_table.h"

#include <cmath>

const double Pool_table::default_ball_radius = 0.0286;
const double Pool_table::default_ball_mass = 0.170;
const double Pool_table::default_rail_width = 0.075;
const double Pool_table::default_rail_height = 0.050;
const double Pool_table::default_middle_pocket_radius = 0.06985;
const double Pool_table::default_corner_pocket_radius = 0.09207;
const double Pool_table::default_middle_pocket_angle = 20.0;	// degrees
const double Pool_table::default_corner_pocket_angle = 35.0;	// degrees
const double Pool_table::default_cushion_width = 0.165;
const double Pool_table::default_frame_height = 0.150;
const double Pool_table::foot_in_m = 0.3048;

Pool_table::Pool_table(Scene& scene, double size_in_feet) : Table(scene, default_ball_radius, default_ball_mass), size(size_in_feet) {

	double playfield_length = size_in_feet * foot_in_m - 0.20;
	double playfield_width = playfield_length / 2;
	
	set_head_spot(vector3(0.0, 0.0, playfield_length/4));
	set_foot_spot(vector3(0.0, 0.0, -playfield_length/4));

	double board_length = playfield_length + 2 * default_rail_width;
	double board_width = playfield_width + 2 * default_rail_width;

	double middle_pocket_angle = default_middle_pocket_angle * M_PI/180.0;
	double corner_pocket_angle = (90.0-default_corner_pocket_angle) * M_PI/180.0;
	
	create_board(board_width, board_length, default_middle_pocket_radius, default_corner_pocket_radius, middle_pocket_angle, corner_pocket_angle, default_rail_width, default_rail_height);
	
	double frame_width = board_width + 2*default_cushion_width;
	double frame_length = board_length + 2*default_cushion_width;
	create_frame(frame_width, frame_length, default_frame_height, default_middle_pocket_radius, default_corner_pocket_radius, default_cushion_width, default_rail_height);

	// Left side out.
	add_event_quad(new Phys_quad(
								 vector3(-frame_width/2, -100.0, frame_length/2),
								 vector3(-frame_width/2, -100.0, -frame_length/2),
								 vector3(-frame_width/2, 100.0, -frame_length/2),
								 vector3(-frame_width/2, 100.0, frame_length/2),
								 QUAD_TYPE_OUT));
	
	// Foot side out.
	add_event_quad(new Phys_quad(
								 vector3(-frame_width/2, -100.0, -frame_length/2),
								 vector3(frame_width/2, -100.0, -frame_length/2),
								 vector3(frame_width/2, 100.0, -frame_length/2),
								 vector3(-frame_width/2, 100.0, -frame_length/2),
								 QUAD_TYPE_OUT));
	
	// Right side out.
	add_event_quad(new Phys_quad(
								 vector3(frame_width/2, -100.0, -frame_length/2),
								 vector3(frame_width/2, -100.0, frame_length/2),
								 vector3(frame_width/2, 100.0, frame_length/2),
								 vector3(frame_width/2, 100.0, -frame_length/2),
								 QUAD_TYPE_OUT));
	
	// Head side out.
	add_event_quad(new Phys_quad(
								 vector3(frame_width/2, -100.0, frame_length/2),
								 vector3(-frame_width/2, -100.0, frame_length/2),
								 vector3(-frame_width/2, 100.0, frame_length/2),
								 vector3(frame_width/2, 100.0, frame_length/2),
								 QUAD_TYPE_OUT));
	
	
	set_top_restitution(0.5);
	set_rail_restitution(0.7);
	set_pocket_restitution(0.3);
	set_frame_restitution(0.4);
}

void Pool_table::create_board(double width, double length, double middle_pocket_radius, double corner_pocket_radius, double middle_pocket_angle, double corner_pocket_angle, double rail_width, double rail_height) {
	vector3 bottomleft 	= vector3(-width/2, 0.0, length/2);
	vector3 bottomright	= vector3(width/2, 0.0, length/2);
	vector3 topright	= vector3(width/2, 0.0, -length/2);
	vector3 topleft		= vector3(-width/2, 0.0, -length/2);
	vector3 left		= vector3(-width/2, 0.0, 0.0);
	vector3 right		= vector3(width/2, 0.0, 0.0);

	vector3 x = (bottomright - bottomleft)/2;
	vector3 z = (topleft - bottomleft)/4;

	// Create bottom left part (with corner pocket).
	create_board_part(
			bottomleft,
			bottomleft + x,
			bottomleft + x + z,
			bottomleft + z,
			corner_pocket_radius);

	// Create bottom right part (with corner pocket).
	create_board_part(
			bottomright,
			bottomright + z,
			bottomright + z - x,
			bottomright - x,
			corner_pocket_radius);

	// Create top right part (with corner pocket).
	create_board_part(
			topright,
			topright - x,
			topright - x - z,
			topright - z,
			corner_pocket_radius);

	// Create top left part (with corner pocket).
	create_board_part(
			topleft,
			topleft - z,
			topleft - z + x,
			topleft + x,
			corner_pocket_radius);

	// Create left part (with middle pocket).
	create_board_part(
			left,
			left + x,
			left + x + z,
			left + z,
			middle_pocket_radius);
	create_board_part(
			left,
			left - z,
			left + x - z,
			left + x,
			middle_pocket_radius);

	// Create right part (with middle pocket),
	create_board_part(
			right,
			right + z,
			right - x + z,
			right - x,
			middle_pocket_radius);
	create_board_part(
			right,
			right - x,
			right - x - z,
			right - z,
			middle_pocket_radius);


	// Create rails.
	
	// Left side, from bottom corner pocket to middle pocket.
	create_rail(
			bottomleft + vector3(0.0, 0.0, -corner_pocket_radius),
			left + vector3(0.0, 0.0, middle_pocket_radius),
			corner_pocket_angle,
			middle_pocket_angle,
			rail_width,
			rail_height,
			rail_width/2,
			rail_height/2,
			0.005);

	// Left side, from middle pocket to top corner pocket.
	create_rail(
			left + vector3(0.0, 0.0, -middle_pocket_radius),
			topleft + vector3(0.0, 0.0, corner_pocket_radius),
			middle_pocket_angle,
			corner_pocket_angle,
			rail_width,
			rail_height,
			rail_width/2,
			rail_height/2,
			0.005);

	// Right side, from middle pocket to bottom corner pocket.
	create_rail(
			right + vector3(0.0, 0.0, middle_pocket_radius),
			bottomright + vector3(0.0, 0.0, -corner_pocket_radius),
			middle_pocket_angle,
			corner_pocket_angle,
			rail_width,
			rail_height,
			rail_width/2,
			rail_height/2,
			0.005);

	// Right side, from top corner pocket to middle pocket.
	create_rail(
			topright + vector3(0.0, 0.0, corner_pocket_radius),
			right + vector3(0.0, 0.0, -middle_pocket_radius),
			corner_pocket_angle,
			middle_pocket_angle,
			rail_width,
			rail_height,
			rail_width/2,
			rail_height/2,
			0.005);

	// Bottom side, from right corner pocket to left corner pocket.
	create_rail(
			bottomright + vector3(-corner_pocket_radius, 0.0, 0.0),
			bottomleft + vector3(corner_pocket_radius, 0.0, 0.0),
			corner_pocket_angle,
			corner_pocket_angle,
			rail_width,
			rail_height,
			rail_width/2,
			rail_height/2,
			0.005);

	// Top side, from left corner pocket to right corner pocket.
	create_rail(
			topleft + vector3(corner_pocket_radius, 0.0, 0.0),
			topright + vector3(-corner_pocket_radius, 0.0, 0.0),
			corner_pocket_angle,
			corner_pocket_angle,
			rail_width,
			rail_height,
			rail_width/2,
			rail_height/2,
			0.005);
}

void Pool_table::create_board_part(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double pocket_radius) {
	// v0: The center of the pocket.
	// v1..v3: The three other vertices of the quad (counter clockwise).
	
	const int num_sections = 4;
	vector3 qv0, qv1, qv2, qv3, qv0d, qv3d; // the quad vertices we create.

	const double depth = 0.20;
	vector3 down = vector3(0.0, -depth, 0.0);
//	vector3 down = vector3(0.0, -pocket_radius, 0.0);

	vector2 t0 = vector2(0.0, 0.0);
	vector2 t1 = vector2(4.0, 0.0);
	vector2 t2 = vector2(4.0, 4.0);
	vector2 t3 = vector2(0.0, 4.0);
	double t_pocket_radius = 4.0 * pocket_radius/(v1 - v0).abs();
	vector2 tx_down = vector2(-t_pocket_radius, 0.0);
	vector2 ty_down = vector2(0.0, -t_pocket_radius);
	vector2 qt0, qt1, qt2, qt3, qt0d, qt3d;	// tex coords

	for (int i=0; i<num_sections; i++) {
		qv1 = v1 + (v2 - v1)*((double)i/num_sections);
		qt1 = t1 + (t2 - t1)*((double)i/num_sections);

		qv2 = v1 + (v2 - v1)*((double)(i + 1)/num_sections);
		qt2 = t1 + (t2 - t1)*((double)(i + 1)/num_sections);

		qv0 = v0 + (qv1 - v0).normalize()*pocket_radius;
		qt0 = t0 + (qt1 - t0).normalize()*t_pocket_radius;

		qv3 = v0 + (qv2 - v0).normalize()*pocket_radius;
		qt3 = t0 + (qt2 - t0).normalize()*t_pocket_radius;

		add_top_quad(new GL_phys_quad(qv0, qv1, qv2, qv3, qt0, qt1, qt2, qt3));

		qv0d = qv0 + down;
		qt0d = qt0 + tx_down;	// TODO: this is not correct, but noone will notice :o)

		qv3d = qv3 + down;
		qt3d = qt3 + tx_down;	// TODO: this is not correct, but noone will notice :o)

		add_top_quad(new GL_phys_quad(qv0, qv3, qv3d, qv0d, qt0, qt3, qt3d, qt0d));
	}

	for (int i=0; i<num_sections; i++) {
		qv1 = v2 + (v3 - v2)*((double)i/num_sections);
		qt1 = t2 + (t3 - t2)*((double)i/num_sections);

		qv2 = v2 + (v3 - v2)*((double)(i + 1)/num_sections);
		qt2 = t2 + (t3 - t2)*((double)(i + 1)/num_sections);

		qv0 = v0 + (qv1 - v0).normalize()*pocket_radius;
		qt0 = t0 + (qt1 - t0).normalize()*t_pocket_radius;

		qv3 = v0 + (qv2 - v0).normalize()*pocket_radius;
		qt3 = t0 + (qt2 - t0).normalize()*t_pocket_radius;
		
		add_top_quad(new GL_phys_quad(qv0, qv1, qv2, qv3, qt0, qt1, qt2, qt3));
		
		qv0d = qv0 + down;
		qt0d = qt0 + ty_down;	// TODO: this is not correct, but noone will notice :o)


		qv3d = qv3 + down;
		qt3d = qt3 + ty_down;	// TODO: this is not correct, but noone will notice :o)


		add_top_quad(new GL_phys_quad(qv0, qv3, qv3d, qv0d, qt0, qt3, qt3d, qt0d));
	}
}

void Pool_table::create_rail(vector3 from, vector3 to, double start_angle, double end_angle, double width, double height, double bottom_width, double inner_start, double inner_height) {
	vector3 start_to_end = to - from;
	vector3 normal = start_to_end.normalY().normalize();
	vector3 up = vector3(0.0, 1.0, 0.0);

	vector3 start_bottomleft = from;
	vector3 start_bottomright = from + normal * bottom_width - normal.normalY() * tan(start_angle) * bottom_width;
	vector3 start_right = from + normal * width - normal.normalY() * tan(start_angle) * width + up*inner_start;
	vector3 start_topright = start_right + up*inner_height;
	vector3 start_topleft = from + up*height;
	vector3 end_bottomleft = to;
	vector3 end_bottomright = to + normal * bottom_width + normal.normalY() * tan(end_angle) * bottom_width;
	vector3 end_right = to + normal * width + normal.normalY() * tan(end_angle) * width + up*inner_start;
	vector3 end_topright = end_right + up*inner_height;
	vector3 end_topleft = to + up*height;


	const double texture_size = 0.160; // in m

	vector2 tex_bottomleft	= vector2(0.0, 0.0);
	vector2 tex_bottomright	= vector2(start_topleft.distance(start_topright)/texture_size, 0.0);
	vector2 tex_topleft		= vector2(0.0, start_topleft.distance(end_topleft)/texture_size);
	vector2 tex_topright	= vector2(end_topleft.distance(end_topright)/texture_size, start_topright.distance(end_topright)/texture_size);

	// Top.
	add_rail_quad(new GL_phys_quad(start_topleft, start_topright, end_topright, end_topleft, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));

	// Right (the part where the ball hits the rail usually).
	add_rail_quad(new GL_phys_quad(start_topright, start_right, end_right, end_topright, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));

	// Bottom right.
	// TODO: could be normal quad since the ball never hits this part.
	add_rail_quad(new GL_phys_quad(start_right, start_bottomright, end_bottomright, end_right, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));

	// Front.
	add_rail_quad(new GL_phys_quad(start_bottomleft, start_bottomright, start_right, start_bottomleft + (start_topleft - start_bottomleft)/2, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));
	add_rail_quad(new GL_phys_quad(start_topleft, start_bottomleft + (start_topleft - start_bottomleft)/2, start_right, start_topright, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));

	// Back.
	add_rail_quad(new GL_phys_quad(end_bottomright, end_bottomleft, end_bottomleft + (end_topleft - end_bottomleft)/2, end_right, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));
	add_rail_quad(new GL_phys_quad(end_right, end_bottomleft + (end_topleft - end_bottomleft)/2, end_topleft, end_topright, tex_bottomleft, tex_bottomright, tex_topright, tex_topleft));
}



void Pool_table::create_frame(double width, double length, double height, double middle_pocket_radius, double corner_pocket_radius, double cushion_width, double cushion_height) {
	const double pocket_spacing = 0.020;
	const double curve_radius = 0.0;
//	const double curve_radius = 0.020;

	vector3 bottomleft	= vector3(-width/2, cushion_height, length/2);
	vector3 bottomright	= vector3(width/2, cushion_height, length/2);
	vector3 topright	= vector3(width/2, cushion_height, -length/2);
	vector3 topleft		= vector3(-width/2, cushion_height, -length/2);
	vector3 left		= vector3(-width/2, cushion_height, 0.0);
	vector3 right		= vector3(width/2, cushion_height, 0.0);

	vector3 down		= vector3(0.0, -(cushion_height + height), 0.0);

	// Left side, from bottom corner pocket to middle pocket.
	create_frame_part(
			bottomleft + vector3(0.0, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			bottomleft + vector3(cushion_width, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			left + vector3(cushion_width, 0.0, middle_pocket_radius + pocket_spacing),
			left + vector3(0.0, 0.0, middle_pocket_radius + pocket_spacing),
			curve_radius);

	// Left side, from middle pocket to top corner pocket.
	create_frame_part(
			left + vector3(0.0, 0.0, -middle_pocket_radius - pocket_spacing),
			left + vector3(cushion_width, 0.0, -middle_pocket_radius - pocket_spacing),
			topleft + vector3(cushion_width, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			topleft + vector3(0.0, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			curve_radius);

	// Right side, from middle pocket to bottom corner pocket.
	create_frame_part(
			right + vector3(0.0, 0.0, middle_pocket_radius + pocket_spacing),
			right + vector3(-cushion_width, 0.0, middle_pocket_radius + pocket_spacing),
			bottomright + vector3(-cushion_width, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			bottomright + vector3(0.0, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			curve_radius);

	// Right side, from top corner pocket to middle pocket.
	create_frame_part(
			topright + vector3(0.0, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			topright + vector3(-cushion_width, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			right + vector3(-cushion_width, 0.0, -middle_pocket_radius - pocket_spacing),
			right + vector3(0.0, 0.0, -middle_pocket_radius - pocket_spacing),
			curve_radius);

	// Bottom side, from right corner pocket to left corner pocket.
	create_frame_part(
			bottomright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, 0.0),
			bottomright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, -cushion_width),
			bottomleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, -cushion_width),
			bottomleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, 0.0),
			curve_radius);

	// Top side, from left corner pocket to right corner pocket.
	create_frame_part(
			topleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, 0.0),
			topleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, cushion_width),
			topright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, cushion_width),
			topright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, 0.0),
			curve_radius);

	// Front side.
	create_frame_part(
			bottomright,
			bottomleft,
			bottomleft + down,
			bottomright + down);

	// Left side.
	create_frame_part(
			bottomleft,
			topleft,
			topleft + down,
			bottomleft + down);

	// Back side.
	create_frame_part(
			topleft,
			topright,
			topright + down,
			topleft + down);

	// Right side.
	create_frame_part(
			topright,
			bottomright,
			bottomright + down,
			topright + down);

	// Left middle pocket.
	create_frame_middle_pocket(
			left + vector3(0.0, 0.0, middle_pocket_radius + pocket_spacing),
			left + vector3(cushion_width, 0.0, middle_pocket_radius + pocket_spacing),
			left + vector3(cushion_width, 0.0, -middle_pocket_radius - pocket_spacing),
			left + vector3(0.0, 0.0, -middle_pocket_radius - pocket_spacing),
			middle_pocket_radius,
			curve_radius,
			QUAD_TYPE_POCKET_MIDDLE_LEFT);

	// Right middle pocket.
	create_frame_middle_pocket(
			right + vector3(0.0, 0.0, -middle_pocket_radius - pocket_spacing),
			right + vector3(-cushion_width, 0.0, -middle_pocket_radius - pocket_spacing),
			right + vector3(-cushion_width, 0.0, middle_pocket_radius + pocket_spacing),
			right + vector3(0.0, 0.0, middle_pocket_radius + pocket_spacing),
			middle_pocket_radius,
			curve_radius,
			QUAD_TYPE_POCKET_MIDDLE_RIGHT);

	// Bottom left corner pocket.
	create_frame_corner_pocket(
			bottomleft,
			bottomleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, 0.0),
			bottomleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			bottomleft + vector3(0.0, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			corner_pocket_radius,
			curve_radius,
			pocket_spacing,
			QUAD_TYPE_POCKET_HEAD_LEFT);

	// Bottom right corner pocket.
	create_frame_corner_pocket(
			bottomright,
			bottomright + vector3(0.0, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			bottomright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, -cushion_width - corner_pocket_radius - pocket_spacing),
			bottomright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, 0.0),
			corner_pocket_radius,
			curve_radius,
			pocket_spacing,
			QUAD_TYPE_POCKET_HEAD_RIGHT);

	// Top right corner pocket.
	create_frame_corner_pocket(
			topright,
			topright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, 0.0),
			topright + vector3(-cushion_width - corner_pocket_radius - pocket_spacing, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			topright + vector3(0.0, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			corner_pocket_radius,
			curve_radius,
			pocket_spacing,
			QUAD_TYPE_POCKET_FOOT_RIGHT);

	// Top left corner pocket.
	create_frame_corner_pocket(
			topleft,
			topleft + vector3(0.0, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			topleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, cushion_width + corner_pocket_radius + pocket_spacing),
			topleft + vector3(cushion_width + corner_pocket_radius + pocket_spacing, 0.0, 0.0),
			corner_pocket_radius,
			curve_radius,
			pocket_spacing,
			QUAD_TYPE_POCKET_FOOT_LEFT);

}

void Pool_table::create_frame_part(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double curve_radius) {
	const double texture_size = 0.320; // 32x32cm
	
	// Create top.
	vector3 qv0 = v0 + (v1 - v0).normalize()*curve_radius;
	vector3 qv1 = v1;
	vector3 qv2 = v2;
	vector3 qv3 = v3 + (v2 - v3).normalize()*curve_radius;

	vector2 qt0 = vector2(M_PI/2*curve_radius/texture_size, 0.0);
	vector2 qt1 = qt0 + vector2(qv0.distance(qv1)/texture_size, 0.0);
	vector2 qt3 = vector2(M_PI/2*curve_radius/texture_size, qv0.distance(qv3)/texture_size);
	vector2 qt2 = qt3 + vector2(qv3.distance(qv2)/texture_size, 0.0);

	GL_phys_quad* top_quad = new GL_phys_quad(qv0, qv1, qv2, qv3, qt0, qt1, qt2, qt3);
	vector3 normal = top_quad->get_normal();
	add_frame_quad(top_quad);

	if (curve_radius <= 0.0)
		return;

	// Create curve.
	const int num_curve_strips = 8;
	vector3 curve_center_bottom = qv0 - normal*curve_radius;
	vector3 curve_center_top = qv3 - normal*curve_radius;

	// quick hack.
	vector3 direction = (qv1 - qv0).normalize();

	vector2 t_incr = vector2(-M_PI/2/num_curve_strips, 0.0);
	for (int i=0; i<num_curve_strips; i++) {
		double alpha = (double)i/num_curve_strips*(M_PI/2) + M_PI/2;
		double beta = (double)(i + 1)/num_curve_strips*(M_PI/2) + M_PI/2;

		qv0 = curve_center_bottom + cos(beta)*curve_radius*direction + vector3(0.0, sin(beta)*curve_radius, 0.0);
		qv1 = curve_center_bottom + cos(alpha)*curve_radius*direction + vector3(0.0, sin(alpha)*curve_radius, 0.0);
		qv2 = curve_center_top + cos(alpha)*curve_radius*direction + vector3(0.0, sin(alpha)*curve_radius, 0.0);
		qv3 = curve_center_top + cos(beta)*curve_radius*direction + vector3(0.0, sin(beta)*curve_radius, 0.0);

		qt0 += t_incr;
		qt1 += t_incr;
		qt2 += t_incr;
		qt3 += t_incr;

		add_frame_quad(new GL_phys_quad(qv0, qv1, qv2, qv3, qt0, qt1, qt2, qt3));
	}
}

void Pool_table::create_frame_middle_pocket(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double pocket_radius, double curve_radius, enum Quad_type pocket_name) {
	vector3 center = v1 + (v2 - v1)/2;
//	vector3 circle = pocket_radius/center.distance(v1)*(v1 - center);

	vector3 qv0 = v0 + (v1 - v0).normalize()*curve_radius;
	vector3 qv1 = v1;
	vector3 qv2 = v2;
	vector3 qv3 = v3 + (v2 - v3).normalize()*curve_radius;

	// Calculate normal.
	Quad quad(qv0, qv1, qv2, qv3);
	vector3 n = quad.get_normal();
	const double depth = 0.20;
	vector3 down = n * -depth;

	const int num_quads = 16;

	vector3 cv0, cv1, cv2, cv3, cv4, cv5;
	vector2 ct0, ct1, ct2, ct3;
	for (int i=0; i<num_quads/4; i++) {
		cv0 = qv1 + (qv0 - qv1)*((double)i/(num_quads/4));
		cv1 = cv0 + (center - cv0)*(1.0 - pocket_radius/(center - cv0).abs());
		cv3 = qv1 + (qv0 - qv1)*((double)(i + 1)/(num_quads/4));
		cv2 = cv3 + (center - cv3)*(1.0 - pocket_radius/(center - cv3).abs());

		add_pocket_quad(new GL_phys_quad(cv0, cv1, cv2, cv3, ct0, ct1, ct2, ct3));

		cv4 = cv1 + down;
		cv5 = cv2 + down;
		add_pocket_quad(new GL_phys_quad(cv1, cv4, cv5, cv2, ct0, ct2, ct2, ct3));
	}

	for (int i=0; i<num_quads/2; i++) {
		cv0 = qv0 + (qv3 - qv0)*((double)i/(num_quads/2));
		cv1 = cv0 + (center - cv0)*(1.0 - pocket_radius/(center - cv0).abs());
		cv3 = qv0 + (qv3 - qv0)*((double)(i + 1)/(num_quads/2));
		cv2 = cv3 + (center - cv3)*(1.0 - pocket_radius/(center - cv3).abs());

		add_pocket_quad(new GL_phys_quad(cv0, cv1, cv2, cv3, ct0, ct1, ct2, ct3));

		cv4 = cv1 + down;
		cv5 = cv2 + down;
		add_pocket_quad(new GL_phys_quad(cv1, cv4, cv5, cv2, ct0, ct2, ct2, ct3));
	}

	for (int i=0; i<num_quads/4; i++) {
		cv0 = qv3 + (qv2 - qv3)*((double)i/(num_quads/4));
		cv1 = cv0 + (center - cv0)*(1.0 - pocket_radius/(center - cv0).abs());
		cv3 = qv3 + (qv2 - qv3)*((double)(i + 1)/(num_quads/4));
		cv2 = cv3 + (center - cv3)*(1.0 - pocket_radius/(center - cv3).abs());

		add_pocket_quad(new GL_phys_quad(cv0, cv1, cv2, cv3, ct0, ct1, ct2, ct3));

		cv4 = cv1 + down;
		cv5 = cv2 + down;
		add_pocket_quad(new GL_phys_quad(cv1, cv4, cv5, cv2, ct0, ct2, ct2, ct3));
	}

	// create "event" quad.
	qv0 = v0 + down;
	qv1 = v1 + down;
	qv2 = v2 + down;
	qv3 = v3 + down;
	
	add_event_quad(new Phys_quad(qv0, qv1, qv2, qv3, pocket_name));



	if (curve_radius <= 0.0)
		return;


	// Create curve.
	const int num_curve_strips = 8;
	vector2 qt0, qt1, qt2, qt3;
	GL_quad top_quad = GL_quad(qv0, qv1, qv2, qv3, qt0, qt1, qt2, qt3);
	vector3 normal = top_quad.get_normal();
	vector3 curve_center_bottom = qv0 - normal*curve_radius;
	vector3 curve_center_top = qv3 - normal*curve_radius;


	// quick hack.
	vector3 direction = (qv1 - qv0).normalize();

	vector2 t_incr = vector2(-M_PI/2/num_curve_strips, 0.0);
	for (int i=0; i<num_curve_strips; i++) {
		double alpha = (double)i/num_curve_strips*(M_PI/2) + M_PI/2;
		double beta = (double)(i + 1)/num_curve_strips*(M_PI/2) + M_PI/2;

		qv0 = curve_center_bottom + cos(beta)*curve_radius*direction + vector3(0.0, sin(beta)*curve_radius, 0.0);
		qv1 = curve_center_bottom + cos(alpha)*curve_radius*direction + vector3(0.0, sin(alpha)*curve_radius, 0.0);
		qv2 = curve_center_top + cos(alpha)*curve_radius*direction + vector3(0.0, sin(alpha)*curve_radius, 0.0);
		qv3 = curve_center_top + cos(beta)*curve_radius*direction + vector3(0.0, sin(beta)*curve_radius, 0.0);

//		qt0 += t_incr;
//		qt1 += t_incr;
//		qt2 += t_incr;
//		qt3 += t_incr;

		add_pocket_quad(new GL_phys_quad(qv0, qv1, qv2, qv3, qt0, qt1, qt2, qt3));
	}

}

void Pool_table::create_frame_corner_pocket(vector3 v0, vector3 v1, vector3 v2, vector3 v3, double pocket_radius, double curve_radius, double pocket_spacing, enum Quad_type pocket_name) {
	const int num_quads = 24;
	const double depth = 0.20;

	// Calculate quad normal.
	vector3 n = Quad(v0, v1, v2, v3).get_normal();

	vector3 down = n*(-depth);

	// vt0 is between v1 and v2. The "right" side of the element.
	// Here we start creating the quads around the corner pocket.
	vector3 vt0 = v2 + (v1 - v2).normalize()*(pocket_spacing + pocket_radius);

	// vt1 is v1 minus curve_radius (bottom right).
	vector3 vt1 = v1 + (v2 - v1).normalize()*curve_radius;

	// vt2 is v0 minus curve radius (bottom left).
	vector3 vt2 = v0 + (v3 - v0).normalize()*curve_radius + (v1 - v0).normalize()*curve_radius;

	// vt3 is v3 minus curve radius (top left).
	vector3 vt3 = v3 + (v2 - v3).normalize()*curve_radius;

	// vt4 is between v2 and v3. The "top" side of the element.
	// The quads end here.
	vector3 vt4 = v2 + (v3 - v2).normalize()*(pocket_spacing + pocket_radius);

	// vc is the center of the corner pocket.
	vector3 vc = vt0 + cross(n, (v2 - v1).normalize())*(pocket_spacing + pocket_radius);

	vector3 qv0, qv1, qv2, qv3;
	for (int i=0; i<num_quads/6; i++) {
		qv0 = vt0 + (vt1 - vt0)*((double)i/(num_quads/6));
		qv3 = vt0 + (vt1 - vt0)*((double)(i + 1)/(num_quads/6));
		qv1 = vc + (qv0 - vc).normalize()*pocket_radius;
		qv2 = vc + (qv3 - vc).normalize()*pocket_radius;

		add_pocket_quad(new GL_phys_quad(qv0, qv1, qv2, qv3));
		add_pocket_quad(new GL_phys_quad(qv1 + down, qv2 + down, qv2, qv1));
	}

	for (int i=0; i<num_quads/3; i++) {
		qv0 = vt1 + (vt2 - vt1)*((double)i/(num_quads/3));
		qv3 = vt1 + (vt2 - vt1)*((double)(i + 1)/(num_quads/3));
		qv1 = vc + (qv0 - vc).normalize()*pocket_radius;
		qv2 = vc + (qv3 - vc).normalize()*pocket_radius;

		add_pocket_quad(new GL_phys_quad(qv0, qv1, qv2, qv3));
		add_pocket_quad(new GL_phys_quad(qv1 + down, qv2 + down, qv2, qv1));
	}

	for (int i=0; i<num_quads/3; i++) {
		qv0 = vt2 + (vt3 - vt2)*((double)i/(num_quads/3));
		qv3 = vt2 + (vt3 - vt2)*((double)(i + 1)/(num_quads/3));
		qv1 = vc + (qv0 - vc).normalize()*pocket_radius;
		qv2 = vc + (qv3 - vc).normalize()*pocket_radius;

		add_pocket_quad(new GL_phys_quad(qv0, qv1, qv2, qv3));
		add_pocket_quad(new GL_phys_quad(qv1 + down, qv2 + down, qv2, qv1));
	}

	for (int i=0; i<num_quads/6; i++) {
		qv0 = vt3 + (vt4 - vt3)*((double)i/(num_quads/6));
		qv3 = vt3 + (vt4 - vt3)*((double)(i + 1)/(num_quads/6));
		qv1 = vc + (qv0 - vc).normalize()*pocket_radius;
		qv2 = vc + (qv3 - vc).normalize()*pocket_radius;

		add_pocket_quad(new GL_phys_quad(qv0, qv1, qv2, qv3));
		add_pocket_quad(new GL_phys_quad(qv1 + down, qv2 + down, qv2, qv1));
	}

	// create "event" quad.
	qv0 = v0 + down;
	qv1 = v1 + down;
	qv2 = v2 + down;
	qv3 = v3 + down;
	
	add_event_quad(new Phys_quad(qv0, qv1, qv2, qv3, pocket_name));
}
		

void Pool_table::display() const {
	Table::display();
//	glTranslatef(point.x, point.y, point.z);

//	board->display();

}



