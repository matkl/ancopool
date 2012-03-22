#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "texture.h"
#include <list>
#include <map>
#include <string>

class Texture_manager {
	public:
		static Texture_manager& get();
		Texture* request_texture(const std::string& filename);
		void release_texture(Texture* texture);

	private:
		Texture_manager();
		std::list<Texture> textures;
		std::map<std::string, Texture*> textures_by_filename;
		std::map<GLuint, Texture*> textures_by_id;
};


#endif // TEXTURE_MANAGER_H
