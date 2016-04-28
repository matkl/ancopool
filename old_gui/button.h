#ifndef BUTTON_H
#define BUTTON_H

#include "widget.h"

namespace GUI {
	class Button : public Widget {
		public:
			Button(Widget* parent = 0);
//			void set_highlight_color(Color color);
//			Color get_highlight_color() const;
//			void set_clicked_color(Color color);
//			Color get_clicked_color() const;
			virtual void on_mouse_enter();
			virtual void on_mouse_leave();
			virtual void draw() const;

		private:
//			Color highlight_color;
//			Color clicked_color;
//			Color saved_background_color;
	};
}

#endif // BUTTON_H

