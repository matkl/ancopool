#include "gl_quad.h"

GL_quad::GL_quad() {
}

GL_quad::GL_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, vector2 t0, vector2 t1, vector2 t2, vector2 t3) : Quad(v0, v1, v2, v3) {
	set_tex_coords(t0, t1, t2, t3);
	vector3 normal = Quad::get_normal();
	set_normals(normal, normal, normal, normal);
}

GL_quad::GL_quad(vector3 v0, vector3 v1, vector3 v2, vector3 v3, vector2 t0, vector2 t1, vector2 t2, vector2 t3, vector3 n0, vector3 n1, vector3 n2, vector3 n3) : Quad(v0, v1, v2, v3) {
	set_tex_coords(t0, t1, t2, t3);
	set_normals(n0, n1, n2, n3);
}

void GL_quad::display() const {
	glPushMatrix();
	GL_object::display();
	glBegin(GL_QUADS);
//	vector3 normal = get_normal();
	for (int i=0; i<4; i++) {
		vector3 vertex = get_vertex(i);
		vector2 tex_coord = tex_coords[i];
		vector3 normal = normals[i];
		glTexCoord2f(tex_coord.x, tex_coord.y);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertex.x, vertex.y, vertex.z);
	}
	glEnd();
	glPopMatrix();
}
