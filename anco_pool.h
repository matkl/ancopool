#ifndef ANCO_POOL_H
#define ANCO_POOL_H

#include <SDL/SDL.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

//#include <CEGUIBase/CEGUI.h>
//#include <CEGUIBase/RendererModules/OpenGL/CEGUIOpenGLRenderer.h>

#include "scene.h"
#include "fps.h"

#if 0
#include "main_menu.h"
#endif

#if 0
#include "scene_sheet.h"
#endif

#include "widget.h"


enum Mode_type {MODE_NONE, MODE_ZOOM, MODE_SHOOT, MODE_ENGLISH};

class Anco_pool {
	public:
		~Anco_pool();
		static Anco_pool& get();
		int run();
		void quit();

		void start_local_game();

		void start_simulation(Phys_simulator* sim);
		void set_video_mode(int width, int height, int bpp, bool fullscreen);

		void on_stop_simulation();

	private:
		Anco_pool();
		Anco_pool(const Anco_pool& anco_pool);

		// Initialization
		bool init();
		bool init_sdl();
		bool init_cegui();

		void display();
		void cleanup();


		// Event handlers
		void handle_event(const SDL_Event& event);
		void handle_active_event(const SDL_ActiveEvent& active);
		void handle_keyboard_event(const SDL_KeyboardEvent& key);
		void handle_mouse_motion_event(const SDL_MouseMotionEvent& motion);
		void handle_mouse_button_event(const SDL_MouseButtonEvent& button);
		void handle_resize_event(const SDL_ResizeEvent& resize);
		void handle_expose_event(const SDL_ExposeEvent& expose);
		void handle_quit_event(const SDL_QuitEvent& quit);
		void handle_user_event(const SDL_UserEvent& user);
		void handle_syswm_event(const SDL_SysWMEvent& syswm);

		void set_mode(enum Mode_type m);
		void lock_mouse();
		void unlock_mouse();

		void update_fps();


		SDL_Surface* surface;
		//CEGUI::OpenGLRenderer* renderer;
		bool running;
		bool simulation_running;
		Fps fps;
		double z_near;
		double z_far;
		int width;
		int height;
		Scene* scene;

		enum Mode_type mode;
		bool active;	// window active?
		bool mouse_locked;

		bool gui_active;

#if 0
		Main_menu* main_menu;
		Scene_sheet* scene_sheet;
#endif


		// GUI stuff
		GUI::Widget root_widget;
};

Uint32 Anco_pool_timer_callback(Uint32 interval, void* param);

#endif // ANCO_POOL_H
