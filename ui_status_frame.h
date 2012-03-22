#ifndef UI_STATUS_FRAME_H
#define UI_STATUS_FRAME_H

#include "ui_frame.h"

namespace UI {
	class Status_frame : public Frame {
		public:
			Status_frame(Object* parent);
			~Status_frame();
			void set_player_one_name(const std::string& name);
			void set_player_two_name(const std::string& name);
	
		private:
//			Font_string* player_one_name;
//			Font_string* player_two_name;
	};
}

#endif // UI_STATUS_FRAME_H
