#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <list>
#include <map>
#include <GL/gl.h>

class Texture {
	public:
		Texture();
		~Texture();

		bool load(const std::string& filename);
		bool load_png(const std::string& filename);
		bool reload();
		GLuint get_id() const;
		int incr_users();
		int decr_users();
		void set_iterator(std::list<Texture>::iterator p);
		std::list<Texture>::iterator get_iterator() const;
		std::string get_filename() const;

	private:
		GLuint id;
		GLint bpp;
		GLint width;
		GLint height;
		GLenum type;
		int num_users;
		std::string filename;
		std::list<Texture>::iterator iter;
};
		
//Texture* request_texture(const std::string& filename);
//void release_texture(Texture* texture);
//void reload_textures();


#endif // TEXTURE_H
