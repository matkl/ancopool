#ifndef WIDGET_H
#define WIDGET_H

#include <vector>
#include "vector.h"
#include "color.h"
#include "rect.h"

namespace GUI {
	enum Anchor_t {
		TOPLEFT,
		TOP,
		TOPRIGHT,
		LEFT,
		CENTER,
		RIGHT,
		BOTTOMLEFT,
		BOTTOM,
		BOTTOMRIGHT
	};

	class Widget {
		public:
			Widget(Widget* parent=0);
			void set_parent(Widget* parent);
			void set_anchor(enum Anchor_t anchor);
			enum Anchor_t get_anchor() const;
			void set_rect(const Rect& rect);
//			void set_background_color(const Color& background_color);
//			Color get_background_color() const;
			vector2 get_point(enum Anchor_t point) const;
			void display() const;
			virtual void draw() const {}
			void draw_rect() const;
			Widget* get_child_at_position(float x, float y) const;
			bool is_at_position(float x, float y) const;

			// Events
			virtual void on_mouse_enter() {}
			virtual void on_mouse_leave() {}

		private:
			void add_child(Widget* child);
			void remove_child(Widget* child);

			Widget* parent;
			Anchor_t anchor;
			Rect rect;
//			Color background_color;
			std::vector<Widget*> children;
	};
}

#endif // WIDGET_H
