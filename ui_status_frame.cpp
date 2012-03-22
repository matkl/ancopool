#include "ui_status_frame.h"

namespace UI {
	Status_frame::Status_frame(Object* parent) : Frame(parent) {
		const std::string font = "media/fonts/Arial.ttf";
		const int size = 36;
	
/*		player_one_name = create_font_string();
		player_one_name->set_font(font, size);
	
		player_two_name = create_font_string();
		player_two_name->set_font(font, size);
		player_two_name->set_position(vector2(100.0, 0.0));
		*/
	}
	
	Status_frame::~Status_frame() {
	}
	
	//	glPixelTransferf(GL_GREEN_BIAS, -1.0f);
	
	void Status_frame::set_player_one_name(const std::string& name) {
//		player_one_name->set_text(name);
	}
	
	void Status_frame::set_player_two_name(const std::string& name) {
//		player_two_name->set_text(name);
	}
}
