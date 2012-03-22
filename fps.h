#ifndef FPS_H
#define FPS_H

#include <SDL/SDL.h>

class Fps {
	public:
		Fps();
		~Fps();

		void update();
		double getFps();

	private:
		Uint32 frame_count;
		Uint32 last_ticks;
		double fps;
};

#endif // FPS_H
