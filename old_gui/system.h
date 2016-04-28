#ifndef SYSTEM_H
#define SYSTEM_H

#include "widget.h"

namespace GUI {
	class System {
		public:
			~System();
			static System& get();
			void set_root_widget(Widget* widget);
			void display() const;
			void inject_mouse_position(float x, float y);

		private:
			System();

			Widget* root_widget;
			Widget* mouse_over_widget;
	};
}



#endif // SYSTEM_H
