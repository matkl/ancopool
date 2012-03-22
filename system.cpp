#include "system.h"

#include <OpenGL/GL.h>

GUI::System::~System() {
}

GUI::System& GUI::System::get() {
	static GUI::System instance;
	return instance;
}

void GUI::System::set_root_widget(GUI::Widget* widget) {
	root_widget = widget;
}

void GUI::System::display() const {
	if (!root_widget)
		return;

	GLint view_port[4];
	glGetIntegerv(GL_VIEWPORT, view_port);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0, view_port[2], 0, view_port[3], -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	GUI::Rect rect;
	rect.left = 0;
	rect.right = view_port[2];
	rect.bottom = 0;
	rect.top = view_port[3];
	root_widget->set_rect(rect);
	root_widget->display();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void GUI::System::inject_mouse_position(float x, float y) {
	if (!root_widget)
		return;

	// swap y coordinate
	GLint view_port[4];
	glGetIntegerv(GL_VIEWPORT, view_port);
	y = view_port[3] - (y + 1.0);

	GUI::Widget* widget = root_widget->get_child_at_position(x, y);

	if (widget != mouse_over_widget) {
		if (mouse_over_widget)
			mouse_over_widget->on_mouse_leave();

		if (widget)
			widget->on_mouse_enter();

		mouse_over_widget = widget;
	}
}

GUI::System::System() : root_widget(0), mouse_over_widget(0) {
}

