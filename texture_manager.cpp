#include "texture_manager.h"

#include <iostream>

Texture_manager& Texture_manager::get() {
	static Texture_manager instance;
	return instance;
}

Texture* Texture_manager::request_texture(const std::string& filename) {	
	// Check if texture is already loaded.
	std::map<std::string, Texture*>::iterator p = textures_by_filename.find(filename);
	if (p == textures_by_filename.end()) {
		// Texture not found in list. Load.

		std::list<Texture>::iterator iter = textures.insert(textures.end(), Texture());
		Texture& texture = *iter;
		texture.set_iterator(iter);

		if (!texture.load(filename)) {
			std::cerr << "Error: Texture " << filename << " couldn't be found!" << std::endl;
			textures.pop_back();
			return 0;
		}
		std::cerr << "Loaded texture " << filename << " with id = " << texture.get_id() << "." << std::endl;

		// Update maps.
		texture.incr_users();
		GLuint id = texture.get_id();
		textures_by_filename[filename] = &texture;
		textures_by_id[id] = &texture;

		return &texture;
	} 

	Texture* texture_pointer = p->second;
	texture_pointer->incr_users();

	return texture_pointer;
}

void Texture_manager::release_texture(Texture* texture) {
	if (texture->decr_users() <= 0) {
		GLuint id = texture->get_id();
		if (id == 0) {
			std::cerr << "Warning: Deleting texture with id = 0." << std::endl;
		}

		const std::string& filename = texture->get_filename();

		// Texture is not used anymore. Delete.
		if (textures_by_id.erase(id) == 0) {
			std::cerr << "Error: Texture not found in id map while releasing." << std::endl;
		}
		if (textures_by_filename.erase(filename) == 0) {
			std::cerr << "Error: Texture not found in filename map while releasing." << std::endl;
		}

		std::list<Texture>::iterator iter = texture->get_iterator();
		textures.erase(iter);
	}
}

Texture_manager::Texture_manager() {
}

