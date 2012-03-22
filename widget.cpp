#include "widget.h"

#include <algorithm>
#include <OpenGL/GL.h>

GUI::Widget::Widget(GUI::Widget* parent) {
	set_parent(parent);
}

void GUI::Widget::set_parent(GUI::Widget* parent) {
	if (parent)
		parent->remove_child(this);

	this->parent = parent;

	if (parent)
		parent->add_child(this);
}

void GUI::Widget::set_anchor(enum GUI::Anchor_t anchor) {
	this->anchor = anchor;
}

enum GUI::Anchor_t GUI::Widget::get_anchor() const {
	return anchor;
}

void GUI::Widget::set_rect(const GUI::Rect& rect) {
	this->rect = rect;
}

vector2 GUI::Widget::get_point(enum Anchor_t point) const {
	vector2 offset;

	switch (point) {
		case TOPLEFT:
			offset.x = rect.left;
			offset.y = rect.top;
			break;
		case TOP:
			offset.x = rect.left + (rect.right - rect.left) / 2;
			offset.y = rect.top;
			break;
		case TOPRIGHT:
			offset.x = rect.right;
			offset.y = rect.top;
			break;
		case LEFT:
			offset.x = rect.left;
			offset.y = rect.bottom + (rect.top - rect.bottom) / 2;
			break;
		case CENTER:
			offset.x = rect.left + (rect.right - rect.left) / 2;
			offset.y = rect.bottom + (rect.top - rect.bottom) / 2;
			break;
		case RIGHT:
			offset.x = rect.right;
			offset.y = rect.bottom + (rect.top - rect.bottom) / 2;
			break;
		case BOTTOMLEFT:
			offset.x = rect.left;
			offset.y = rect.bottom;
			break;
		case BOTTOM:
			offset.x = rect.left + (rect.right - rect.left) / 2;
			offset.y = rect.bottom;
			break;
		case BOTTOMRIGHT:
			offset.x = rect.right;
			offset.y = rect.bottom;
	}

	return offset;
}	

void GUI::Widget::display() const {
	glPushMatrix();

	if (parent) {
		vector2 point = parent->get_point(anchor);
		glTranslatef(point.x, point.y, 0.0);
	}

	// draw this widget

	draw();

	// draw children (if any)
	if (!children.empty()) {
		for (std::vector<GUI::Widget*>::const_iterator p = children.begin(); p != children.end(); p++) {
			(*p)->display();
		}
	}

	glPopMatrix();
}

void GUI::Widget::draw_rect() const {
	glBegin(GL_QUADS);
//		glColor4f(background_color.r, background_color.g, background_color.b, background_color.a);
		glVertex3f(rect.left, rect.top, 0.0);
		glVertex3f(rect.left, rect.bottom, 0.0);
		glVertex3f(rect.right, rect.bottom, 0.0);
		glVertex3f(rect.right, rect.top, 0.0);
	glEnd();
}

GUI::Widget* GUI::Widget::get_child_at_position(float x, float y) const {
	for (std::vector<GUI::Widget*>::const_reverse_iterator p = children.rbegin(); p != children.rend(); p++) {
		// recursively check children
		vector2 offset = get_point((*p)->get_anchor());
		float x2 = x - offset.x;
		float y2 = y - offset.y;

		GUI::Widget* widget = (*p)->get_child_at_position(x2, y2);

		if (widget)
			return widget;

		if ((*p)->is_at_position(x2, y2))
			return *p;
	}

	// nothing found
	return 0;
}

bool GUI::Widget::is_at_position(float x, float y) const {
	return (x >= rect.left && x <= rect.right && y >= rect.bottom && y <= rect.top);
}


// private functions

void GUI::Widget::add_child(GUI::Widget* child) {
	children.push_back(child);
}

void GUI::Widget::remove_child(GUI::Widget* child) {
	std::remove(children.begin(), children.end(), child);
}

	
