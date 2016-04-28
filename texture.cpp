#include "texture.h"

#include <GL/glu.h>
#include <libpng12/png.h>
#include <iostream>
#include <stdlib.h>
#include "tga.h"

//static std::list<Texture> textures;
//static std::map<std::string, Texture*> textures_by_filename;
//static std::map<GLuint, Texture*> textures_by_id;

#if 0
Texture* request_texture(const std::string& filename) {	
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

void release_texture(Texture* texture) {
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

void reload_textures() {
	for (std::list<Texture>::iterator p = textures.begin(); p != textures.end(); p++) {
		(*p).reload();
	}
}
#endif 

Texture::Texture() : id(0), num_users(0) {
}

Texture::~Texture() {
	// Delete OpenGL texture.
	if (id != 0) {
		glDeleteTextures(1, &id);
	}
}

bool Texture::load(const std::string& filename) {
	size_t dot_pos = filename.find_last_of(".");

	if (dot_pos == std::string::npos) {
		// filename without extension
		return false;
	}

	std::string extension = filename.substr(dot_pos);

	GLuint id = 0;

	if (extension.compare(".png") == 0) {
		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		load_png(filename);
	} else if (extension.compare(".tga") == 0) {
		Tga tga;
		if (tga.load(filename)) {
			bpp = tga.getBpp();
			width = tga.getWidth();
			height = tga.getHeight();
			type = tga.getType();
			GLubyte* imageData = tga.getImageData();

			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			gluBuild2DMipmaps(GL_TEXTURE_2D, bpp/8, width, height, type, GL_UNSIGNED_BYTE, imageData);
		} else {
			return false;
		}
	} else {
		return false;
	}

	this->filename = filename;
	this->id = id;

	return true;
}

bool Texture::load_png(const std::string& filename) {
    FILE *PNG_file = fopen(filename.c_str(), "rb");
    if (PNG_file == NULL)
    {
		std::cerr << "Can't open PNG file " << filename << std::endl;
        return false;
    }
    
	const int png_header_size = 8;
    GLubyte PNG_header[png_header_size];
    
    fread(PNG_header, 1, png_header_size, PNG_file);
    if (png_sig_cmp(PNG_header, 0, png_header_size) != 0)
    {
		std::cerr << filename << " is not a PNG file" << std::endl;
		fclose(PNG_file);
        return false;
    }
    
    png_structp PNG_reader = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (PNG_reader == NULL)
    {
		std::cerr << "Can't start reading PNG file " << filename << std::endl;
        fclose(PNG_file);
        return false;
    }

    png_infop PNG_info = png_create_info_struct(PNG_reader);
    if (PNG_info == NULL)
    {
		std::cerr << "Can't get info for PNG file " << filename << std::endl;
        png_destroy_read_struct(&PNG_reader, NULL, NULL);
        fclose(PNG_file);
        return false;
    }

    png_infop PNG_end_info = png_create_info_struct(PNG_reader);
    if (PNG_end_info == NULL)
    {
		std::cerr << "Can't get end info for PNG file " << filename << std::endl;
        png_destroy_read_struct(&PNG_reader, &PNG_info, NULL);
        fclose(PNG_file);
        return false;
    }
    
    if (setjmp(png_jmpbuf(PNG_reader)))
    {
		std::cerr << "Can't load PNG file " << filename << std::endl;
        png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
        fclose(PNG_file);
        return false;
    }
    
    png_init_io(PNG_reader, PNG_file);
    png_set_sig_bytes(PNG_reader, png_header_size);
    
    png_read_info(PNG_reader, PNG_info);
    
    png_uint_32 width, height;
    width = png_get_image_width(PNG_reader, PNG_info);
    height = png_get_image_height(PNG_reader, PNG_info);
    
    png_uint_32 bit_depth, color_type;
    bit_depth = png_get_bit_depth(PNG_reader, PNG_info);
    color_type = png_get_color_type(PNG_reader, PNG_info);
    
    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(PNG_reader);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8) 
    {
        png_set_gray_1_2_4_to_8(PNG_reader);
    }
    
    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(PNG_reader);
    }

    if (png_get_valid(PNG_reader, PNG_info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(PNG_reader);
    }
    else
    {
        png_set_filler(PNG_reader, 0xff, PNG_FILLER_AFTER);
    }
    
    if (bit_depth == 16)
    {
        png_set_strip_16(PNG_reader);
    }
    
    png_read_update_info(PNG_reader, PNG_info);
    
    png_byte* PNG_image_buffer = (png_byte*)malloc(4 * width * height);
    png_byte** PNG_rows = (png_byte**)malloc(height * sizeof(png_byte*));
    
    unsigned int row;
    for (row = 0; row < height; ++row)
    {
        PNG_rows[height - 1 - row] = PNG_image_buffer + (row * 4 * width);
    }
    
    png_read_image(PNG_reader, PNG_rows);
    
    free(PNG_rows);
    
    png_destroy_read_struct(&PNG_reader, &PNG_info, &PNG_end_info);
    fclose(PNG_file);
    
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGBA,
        width,
        height,
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        PNG_image_buffer);
    
    free(PNG_image_buffer);
    
    return true;
}



bool Texture::reload() {
	return load(filename);
}

GLuint Texture::get_id() const {
	return id;
}

int Texture::incr_users() {
	return ++num_users;
}

int Texture::decr_users() {
	return --num_users;
}

void Texture::set_iterator(std::list<Texture>::iterator p) {
	iter = p;
}

std::list<Texture>::iterator Texture::get_iterator() const {
	return iter;
}

std::string Texture::get_filename() const {
	return filename;
}


