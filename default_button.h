#ifndef DEFAULT_BUTTON_H
#define DEFAULT_BUTTON_H

#include "button.h"
#include "text.h"
#include "image.h"

namespace GUI {
	class Default_button : public Button {
		public:
			Default_button(Widget* parent);
			void set_icon(const std::string& icon_str);
			void set_text(const std::string& text_str);
//			virtual void draw() const;
		private:
			Image icon;
			Text text;
	};
}

#endif // DEFAULT_BUTTON_H
