#include "fps.h"

Fps::Fps() {
	frame_count = 0;
	last_ticks = 0;
	fps = 0.0;
}

Fps::~Fps() {
}

void Fps::update() {
	frame_count++;
	
	Uint32 elapsed = SDL_GetTicks() - last_ticks;

	if (elapsed >= 1000) {
		fps = (double)frame_count / elapsed * 1000.0;
		frame_count = 0;
		last_ticks = SDL_GetTicks();
	
		printf("FPS: %f\n", fps);
	}
}

double Fps::getFps() {
	return fps;
}
