#include "gl_phys_quad.h"

// GL_phys_quad::GL_phys_quad() {
// }

GL_phys_quad::GL_phys_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, vector2 t0, vector2 t1, vector2 t2, vector2 t3) : GL_quad(v0, v1, v2, v3, t0, t1, t2, t3), Phys_quad(v0, v1, v2, v3), Quad(v0, v1, v2, v3) {
}
