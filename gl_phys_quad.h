#ifndef GL_PHYS_QUAD_H
#define GL_PHYS_QUAD_H

#include "gl_quad.h"
#include "phys_quad.h"

class GL_phys_quad : public GL_quad, public Phys_quad {
	public:
//		GL_phys_quad();
		GL_phys_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, vector2 t0=vector2(0.0, 0.0), vector2 t1 = vector2(0.0, 1.0), vector2 t2 = vector2(1.0, 1.0), vector2 t3 = vector2(1.0, 0.0));
};


#endif // GL_PHYS_QUAD_H
