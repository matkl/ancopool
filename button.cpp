#include "button.h"

#include "texfont.h"
#include "resource_manager.h"

//TexFont* txf;

GUI::Button::Button(GUI::Widget* parent) : GUI::Widget(parent) {
//	std::string path = Resource_manager::get().get_resource("helvetica-bold.txf");
//	txf = txfLoadFont(path.c_str());
//
//	if (!txf) {
//		printf("error loading font\n");
//	}
//
//	txfEstablishTexture(txf, 30, GL_TRUE);
}

void GUI::Button::on_mouse_enter() {
}

void GUI::Button::on_mouse_leave() {
}

void GUI::Button::draw() const {
//	glTranslatef(0.0, 50.0, 0.0);
	
	//txfRenderString(txf, "Hello world", 11);
	//txfRenderString(txf, "Schalala", 8);
}

