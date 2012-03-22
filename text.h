#ifndef TEXT_H
#define TEXT_H

#include "widget.h"
#include <string>
#include "font_manager.h"

namespace GUI {
	class Text : public Widget {
		public:
			Text(GUI::Widget* parent = 0);
			void set_text(const std::string& text);
			void set_font(const std::string& font, int size);
			virtual void draw() const;
		private:
			std::string text;
			Font* font;
	};
}

#endif // TEXT_H
