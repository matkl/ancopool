#include "anco_pool.h"

#include <SDL/SDL_opengl.h>
#include <iostream>
#include <cstdlib>


#include "phys_simulator.h"
#include "resource_manager.h"
#include "player_manager.h"
#include "system.h"
#include "default_button.h"

using std::cerr;
using std::endl;

Anco_pool::Anco_pool() : surface(0), running(false), z_near(0.01), z_far(20.0), scene(0), mode(MODE_NONE), simulation_running(false), main_menu(0), scene_sheet(0) {
}

Anco_pool::~Anco_pool() {
	if (scene)
		delete scene;
	
	if (main_menu)
		delete main_menu;
	
	if (scene_sheet)
		delete scene_sheet;
}

Anco_pool& Anco_pool::get() {
	static Anco_pool instance;
	return instance;
}

int Anco_pool::run() {
	init();

	SDL_Event event;
	running = true;
	active = true;

	while (running) {
		while (SDL_PollEvent(&event)) {
			handle_event(event);
		}

		update_fps();
		display();
	}

	cleanup();
	return 0;
}

void Anco_pool::start_local_game() {
//	scene_sheet->show();

	Player_manager& pmgr = Player_manager::get();
	
	std::vector<Player*> active_players;
	active_players.push_back(pmgr.get_player("Fast Eddie"));
	active_players.push_back(pmgr.get_player("Jimmy White"));
	
	scene = new Scene(active_players);
}
	

void Anco_pool::start_simulation(Phys_simulator* sim) {
	SDL_AddTimer(10, Anco_pool_timer_callback, sim);
	simulation_running = true;
}

bool Anco_pool::init() {
	// Initialize SDL.
	if (!init_sdl())
		return false;
	
	// Initialize resource management.
	Resource_manager::get().initialize();
	
	// Initialize GUI.
	//if (!init_cegui())
	//	return false;


#if 0
	GUI::Rect rect;
	rect.left = 0.0;
	rect.right = 100.0;
	rect.bottom = 0.0;
	rect.top = 100.0;
	
	GUI::Color color;
	color.r = 0.0;
	color.g = 0.0;
	color.b = 0.0;
	color.a = 0.0;

	root_widget.set_rect(rect);
//	root_widget.set_background_color(color);

	GUI::Default_button* local_game = new GUI::Default_button(&root_widget);
	local_game->set_anchor(GUI::BOTTOM);
	local_game->set_rect(GUI::Rect(-320.0, -192.0, 0.0, 128.0));
	local_game->set_icon(Resource_manager::get().get_resource("users.png"));
	local_game->set_text("Start local game");
	
	GUI::Default_button* network_game = new GUI::Default_button(&root_widget);
	network_game->set_anchor(GUI::BOTTOM);
	network_game->set_rect(GUI::Rect(-160.0, -32.0, 0.0, 128.0));
	network_game->set_icon(Resource_manager::get().get_resource("web.png"));
	network_game->set_text("Start network game");
	
	GUI::Default_button* settings = new GUI::Default_button(&root_widget);
	settings->set_anchor(GUI::BOTTOM);
	settings->set_rect(GUI::Rect(32.0, 160.0, 0.0, 128.0));
	settings->set_icon(Resource_manager::get().get_resource("control_panel.png"));
	settings->set_text("Settings");
	
	GUI::Default_button* quit = new GUI::Default_button(&root_widget);
	quit->set_anchor(GUI::BOTTOM);
	quit->set_rect(GUI::Rect(192.0, 320.0, 0.0, 128.0));
	quit->set_icon(Resource_manager::get().get_resource("power.png"));
	quit->set_text("Quit game");
	


//	GUI::Image* local_game = new GUI::Image(&root_widget);
	color.r = 0.2;
	color.g = 0.2;
	color.b = 0.2;
	color.a = 1.0;
//	local_game->set_background_color(color);

	color.r = 0.6;
	color.g = 0.6;
	color.b = 0.6;
//	local_game->set_highlight_color(color);


//	local_game->set_image("users.png");
//	local_game->set_parent(&root_widget);



	GUI::System::get().set_root_widget(&root_widget);
#endif
	// Initialize random numbers generator.
	srand((unsigned)time(0));

	// initialize players
	Player_manager& pmgr = Player_manager::get();
	pmgr.new_player("Fast Eddie");
	pmgr.new_player("Jimmy White");
	
	start_local_game();

	return true;
}

bool Anco_pool::init_sdl() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		return false;
		
	set_video_mode(800, 600, 32, false);

	return true;
}

bool Anco_pool::init_cegui() {
//	std::string rdir = Resource_manager::get().get_resources_directory();
	
//	renderer = &(CEGUI::OpenGLRenderer::bootstrapSystem());
	
//	CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
//	rp->setResourceGroupDirectory("resources", rdir);
	
	//	rp->setResourceGroupDirectory("schemes", rdir);
	//	rp->setResourceGroupDirectory("imagesets", rdir);
	//	rp->setResourceGroupDirectory("fonts", rdir);
	//	rp->setResourceGroupDirectory("layouts", rdir);
	//	rp->setResourceGroupDirectory("looknfeels", rdir);
	//	rp->setResourceGroupDirectory("lua_scripts", rdir);
	
	// Set the default resource groups to be used.
/*	CEGUI::Imageset::setDefaultResourceGroup("resources");
	CEGUI::Font::setDefaultResourceGroup("resources");
	CEGUI::Scheme::setDefaultResourceGroup("resources");
	CEGUI::WidgetLookManager::setDefaultResourceGroup("resources");
	CEGUI::WindowManager::setDefaultResourceGroup("resources");
	CEGUI::ScriptModule::setDefaultResourceGroup("resources");
	
	// Setup default group for validation schemas.
	CEGUI::XMLParser* parser = CEGUI::System::getSingleton().getXMLParser();
	if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
		parser->setProperty("SchemaDefaultResourceGroup", "resources");

	CEGUI::SchemeManager::getSingleton().create("TaharezLook.scheme");
	CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
	
	main_menu = new Main_menu;
	scene_sheet = new Scene_sheet;
	
	main_menu->show();

	//CEGUI::System::getSingleton().setGUISheet(rootwnd);
*/		
	return true;
}

void Anco_pool::handle_event(const SDL_Event& event) {
	switch (event.type) {
		case SDL_MOUSEMOTION:
			handle_mouse_motion_event(event.motion);
			break;
			
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			handle_mouse_button_event(event.button);
			break;
			
		case SDL_KEYUP:
		case SDL_KEYDOWN:
			handle_keyboard_event(event.key);
			break;
		
		case SDL_ACTIVEEVENT:
			handle_active_event(event.active);
			break;
			
		case SDL_VIDEORESIZE:
			handle_resize_event(event.resize);
			break;

		case SDL_VIDEOEXPOSE:
			handle_expose_event(event.expose);
			break;
			
		case SDL_QUIT:
			handle_quit_event(event.quit);
			break;
			
		case SDL_USEREVENT:
			handle_user_event(event.user);
			break;
			
		case SDL_SYSWMEVENT:
			handle_syswm_event(event.syswm);
			break;
	}
}

void Anco_pool::handle_active_event(const SDL_ActiveEvent& active) {
	this->active = active.gain;
}

void Anco_pool::handle_resize_event(const SDL_ResizeEvent& resize) {
	set_video_mode(resize.w, resize.h, 32, false);
}

void Anco_pool::handle_expose_event(const SDL_ExposeEvent& expose) {
}

void Anco_pool::handle_quit_event(const SDL_QuitEvent& quit) {
	running = false;
}

void Anco_pool::handle_syswm_event(const SDL_SysWMEvent& syswm) {
}

void Anco_pool::handle_mouse_motion_event(const SDL_MouseMotionEvent& motion) {
	GUI::System::get().inject_mouse_position(static_cast<float>(motion.x), static_cast<float>(motion.y));


//	CEGUI::System::getSingleton().injectMousePosition(static_cast<float>(motion.x), static_cast<float>(motion.y));
	
	if (!active || !scene)
		return;
	
	if (!mouse_locked)
		return;
	
	static bool warped_mouse = false;
	
	if (warped_mouse) {
		warped_mouse = false;
		return;
	}
	
	switch (mode) {
		case MODE_NONE:
			scene->rotate_camera(motion.yrel/8.0, motion.xrel/8.0);
			scene->add_cue_rotation(-motion.xrel/8.0);
			break;
		case MODE_SHOOT:
			if (!simulation_running)
				scene->add_cue_offset(vector3(0.0, 0.0, motion.yrel/500.0));
			break;
		case MODE_ENGLISH:
			if (!simulation_running)
				scene->add_cue_offset(vector3(motion.xrel/5000.0, -motion.yrel/5000.0, 0.0));
			break;
		case MODE_ZOOM:
			scene->zoom_camera(motion.yrel/256.0);
			break;
	}
	
	// Set mouse position to center.
	warped_mouse = true;
	SDL_WarpMouse(width/2, height/2);
}

void Anco_pool::handle_mouse_button_event(const SDL_MouseButtonEvent& button) {
	switch (button.type) {
		case SDL_MOUSEBUTTONDOWN:
			switch(button.button) {
				case SDL_BUTTON_LEFT:
					//CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
					
					if (scene) {
						set_mode(MODE_SHOOT);
					}
					break;
					
				case SDL_BUTTON_RIGHT:
					//CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
					
					if (scene) {
						set_mode(MODE_ENGLISH);
					}
					break;
					
				case SDL_BUTTON_MIDDLE:
					//CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::MiddleButton);
					
					if (scene) {
						set_mode(MODE_ZOOM);
					}
					break;
					
				case SDL_BUTTON_WHEELUP:
					//CEGUI::System::getSingleton().injectMouseWheelChange(+1);
					
					if (scene) {
						scene->zoom_camera(-0.01);
					}
					break;
					
				case SDL_BUTTON_WHEELDOWN:
					//CEGUI::System::getSingleton().injectMouseWheelChange(-1);
					
					if (scene) {
						scene->zoom_camera(0.01);
					}
					break;
			}
			
			if (scene) {
				lock_mouse();
			}
			break;
			
		case SDL_MOUSEBUTTONUP:
			switch (button.button) {
				case SDL_BUTTON_LEFT:
					//CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
					break;
					
				case SDL_BUTTON_MIDDLE:
					//CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::MiddleButton);
					break;
					
				case SDL_BUTTON_RIGHT:
					//CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
					break;
			}
			
			if (scene) {
				set_mode(MODE_NONE);
			}
			break;
	}
}

void Anco_pool::handle_keyboard_event(const SDL_KeyboardEvent& key) {
	switch (key.type) {
		case SDL_KEYDOWN:
			//CEGUI::System::getSingleton().injectKeyDown(key.keysym.scancode);
			
			if (!scene)
				break;
			
			switch(key.keysym.sym) {
				case SDLK_ESCAPE:
					unlock_mouse();
					break;
				case SDLK_1:
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_LIGHTING);
					break;
				case SDLK_2:
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
					glEnable(GL_TEXTURE_2D);
					glEnable(GL_LIGHTING);
					break;
				case SDLK_f:
					set_video_mode(800, 600, 32, false);
					break;
				case SDLK_g:
					set_video_mode(1680, 1050, 32, true);
					break;
				case SDLK_s:
					set_mode(MODE_SHOOT);
					break;
				case SDLK_e:
					set_mode(MODE_ENGLISH);
					break;
			}
			break;
			
		case SDL_KEYUP:
			//CEGUI::System::getSingleton().injectKeyUp(key.keysym.scancode);
			
			if (!scene)
				break;
			
			switch(key.keysym.sym) {
				case SDLK_e:
				case SDLK_s:
					set_mode(MODE_NONE);
					break;
			}
			break;
	}
}

void Anco_pool::handle_user_event(const SDL_UserEvent& user) {
	switch (user.code) {
		case 1:
			// Simulation has stopped.
			on_stop_simulation();
			break;
	}
}

void Anco_pool::update_fps() {
	fps.update();
}

void Anco_pool::display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	if (scene) {
		// Display scene.
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);
		scene->display();
	}
		
	// Display user interface.
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	GUI::System::get().display();


//	CEGUI::System::getSingleton().renderGUI();

	SDL_GL_SwapBuffers();
}

void Anco_pool::cleanup() {
	SDL_FreeSurface(surface);
	SDL_Quit();
}

void Anco_pool::set_video_mode(int width, int height, int bpp, bool fullscreen) {
	this->width = width;
	this->height = height;

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

	Uint32 flags = SDL_OPENGL | SDL_RESIZABLE;
	flags |= fullscreen ? SDL_FULLSCREEN : 0;

	// Save GUI textures to buffer.
//	if (renderer)
//		renderer->grabTextures();
	
	surface = SDL_SetVideoMode(width, height, bpp, flags);

	if (surface == 0) {
		std::cerr << "Couldn't set video mode: " << SDL_GetError() << std::endl;
		SDL_Quit();
	}

	// Restore GUI textures.
/*	if (renderer) {
		renderer->restoreTextures();
		renderer->setDisplaySize(CEGUI::Size(width, height));
	}
*/		
//	lock_mouse();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0f);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (GLfloat)(width)/(GLfloat)(height), z_near, z_far);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//if (renderer)
	//	CEGUI::System::getSingleton().notifyDisplaySizeChanged(CEGUI::Size(width, height));

	// Reload textures from disk.
	//reload_textures();

	if (scene)
		scene->update();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Anco_pool::set_mode(enum Mode_type m) {
	mode = m;
}

void Anco_pool::on_stop_simulation() {
	scene->on_stop_simulation();
	simulation_running = false;
}

void Anco_pool::unlock_mouse() {
	SDL_ShowCursor(SDL_ENABLE);
	mouse_locked = false;
}

void Anco_pool::lock_mouse() {
	SDL_ShowCursor(SDL_DISABLE);
	mouse_locked = true;
}

void Anco_pool::quit() {
	running = false;
}


Uint32 Anco_pool_timer_callback(Uint32 interval, void* param) {
//	printf("SIMULATING %d\n", interval);
	bool continue_sim = ((class Phys_simulator *)param)->simulate(interval / 1000.0);

	if (continue_sim) {
		return interval;
	} else {
		SDL_Event event;
		event.type = SDL_USEREVENT;
		event.user.code = 1;
		event.user.data1 = NULL;
		event.user.data2 = NULL;

		int rv = SDL_PushEvent(&event);
		printf("rv=%d\n", rv);

		printf("STOP\n");
		return 0;
	}
}
