#include "default_button.h"

GUI::Default_button::Default_button(Widget* parent) : GUI::Button(parent), icon(this), text(this) {
	GUI::Rect rect;
	rect.left = 0.0;
	rect.right = 128.0;
	rect.bottom = -64.0;
	rect.top = 64.0;
	icon.set_rect(rect);
	icon.set_anchor(GUI::TOPLEFT);

	
	text.set_anchor(GUI::TOPLEFT);
}

void GUI::Default_button::set_icon(const std::string& icon_str) {
	icon.set_image(icon_str);
}

void GUI::Default_button::set_text(const std::string& text_str) {
	text.set_text(text_str);
}
