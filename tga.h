#ifndef TGA_H
#define TGA_H

#include <OpenGL/gl.h>
#include <string>
#include <stdio.h>

using std::string;

class Tga {
	public:
		Tga();
		~Tga();
		bool load(const string& filename);
		GLushort getWidth() const;
		GLushort getHeight() const;
		GLubyte getBpp() const;
		GLuint getType() const;
		GLuint getSize() const;
		GLubyte* getImageData() const;

	private:
		bool loadUncompressed(FILE* file);
		bool loadCompressed(FILE* file);

		GLubyte header[18];
		GLubyte* image_data;
		GLushort image_width;
		GLushort image_height;
		GLubyte pixel_depth;
		GLuint type;
		GLuint image_size;
		GLuint bytes_per_pixel;
};

#endif // TGA_H
