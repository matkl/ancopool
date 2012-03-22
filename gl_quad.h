#ifndef GL_QUAD_H
#define GL_QUAD_H

#include "quad.h"
#include "gl_object.h"

class GL_quad : public virtual Quad, public GL_object {
	public:
		GL_quad();
		GL_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, vector2 t0, vector2 t1, vector2 t2, vector2 t3);
		GL_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, vector2 t0, vector2 t1, vector2 t2, vector2 t3, vector3 n0, vector3 n1, vector3 n2, vector3 n3);
		void set_tex_coords(vector2 v0, vector2 v1, vector2 v2, vector2 v3);
		void set_normals(vector3 v0, vector3 v1, vector3 v2, vector3 v3);
//		vector2 get_tex_coord(int i) const;
//		vector3 get_normal(int i) const;
		virtual void display() const;

	private:
		vector2 tex_coords[4];
		vector3 normals[4];
};

inline void GL_quad::set_tex_coords(vector2 v0, vector2 v1, vector2 v2, vector2 v3) {
	tex_coords[0] = v0;
	tex_coords[1] = v1;
	tex_coords[2] = v2;
	tex_coords[3] = v3;
}

inline void GL_quad::set_normals(vector3 v0, vector3 v1, vector3 v2, vector3 v3) {
	normals[0] = v0;
	normals[1] = v1;
	normals[2] = v2;
	normals[3] = v3;
}

#endif // GL_QUAD_H
