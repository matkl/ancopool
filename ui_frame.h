#ifndef UI_FRAME_H
#define UI_FRAME_H

#include "ui_object.h"

namespace UI {
	class Font_string;

	class Frame : public Object {
		public:
			Frame(Object* parent);

			Font_string* create_font_string();
	};
}


#endif // UI_FRAME_H
