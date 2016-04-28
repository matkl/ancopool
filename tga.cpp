#include "tga.h"
#include <stdlib.h>

Tga::Tga() {
	image_data = NULL;
}

Tga::~Tga() {
	if (image_data != NULL) {
		free(image_data);
	}
}

bool Tga::load(const string& filename) {
	FILE* file = fopen(filename.c_str(), "rb");

	if (file == NULL) {
		return false;
	}

	if (fread(&header, sizeof(header), 1, file) == 0) {
		if (file != NULL) {
			fclose(file);
		}
		return false;
	}

	GLubyte id_length = header[0];
	GLubyte color_map_type = header[1];
	GLubyte image_type = header[2];
	GLushort color_map_begin = header[4]*256 + header[3];
	GLushort color_map_length = header[6]*256 + header[5];
	GLubyte color_map_entry_size = header[7];
	GLushort x_origin_of_image = header[9]*256 + header[8];
	GLushort y_origin_of_image = header[11]*256 + header[10];
	image_width = header[13]*256 + header[12];
	image_height = header[15]*256 + header[14];
	pixel_depth = header[16];
	GLubyte image_descriptor = header[17];

	printf("id_length: %d\n", id_length);
	printf("color_map_length: %d\n", color_map_length);



	if (pixel_depth == 24) {
		type = GL_RGB;
	} else if (pixel_depth == 32) {
		type = GL_RGBA;
	} else {
		fclose(file);
		return false;
	}

	bytes_per_pixel = pixel_depth / 8;
	image_size = bytes_per_pixel * image_width * image_height;
	image_data = (GLubyte*) malloc(image_size);

	if (image_data == NULL) {
		fclose(file);
		return false;
	}

	// skip image id
	fseek(file, id_length, SEEK_CUR);

	// no support for color maps
	if (color_map_length > 0) {
		printf("error: %s is a color map TGA.\n", filename.c_str());
		fclose(file);
		return false;
	}

	bool rv = false;
	if (image_type == 2) {
		rv = loadUncompressed(file);
	} else if (image_type == 10) {
		rv = loadCompressed(file);
	} else {
		fclose(file);
		return false;
	}

	fclose(file);

	return rv;
}

GLushort Tga::getWidth() const {
	return image_width;
}

GLushort Tga::getHeight() const {
	return image_height;
}

GLubyte Tga::getBpp() const {
	return pixel_depth;
}

GLuint Tga::getType() const {
	return type;
}

GLuint Tga::getSize() const {
	return image_size;
}

GLubyte* Tga::getImageData() const {
	return image_data;
}

bool Tga::loadUncompressed(FILE* file) {
	if (fread(image_data, 1, image_size, file) != image_size) {
		return false;
	}

	// byte swapping
	for (GLuint cswap = 0; cswap < (int)image_size; cswap += bytes_per_pixel) {
		image_data[cswap] ^= image_data[cswap+2] ^=
		image_data[cswap] ^= image_data[cswap+2];
	}

	return true;
}

bool Tga::loadCompressed(FILE* file) {
	GLuint pixelcount = image_width * image_height;
	GLuint currentpixel = 0;
	GLuint currentbyte = 0;
	GLubyte* colorbuffer = (GLubyte*) malloc(bytes_per_pixel);
	
	if (colorbuffer == NULL) {
		return false;
	}

	do {
		GLubyte chunkheader = 0;
		if (fread(&chunkheader, sizeof(GLubyte), 1, file) == 0) {
			free(colorbuffer);
			return false;
		}

		if (chunkheader < 128) {
			// raw packet

			chunkheader++;
			for (short counter = 0; counter < chunkheader; counter++) {
				if (fread(colorbuffer, 1, bytes_per_pixel, file) != bytes_per_pixel) {
					free(colorbuffer);
					return false;
				}

				image_data[currentbyte] = colorbuffer[2];
				image_data[currentbyte+1] = colorbuffer[1];
				image_data[currentbyte+2] = colorbuffer[0];

				if (bytes_per_pixel == 4) {
					image_data[currentbyte+3] = colorbuffer[3];
				}

				currentbyte += bytes_per_pixel;
				currentpixel++;

				if (currentpixel > pixelcount) {
					free(colorbuffer);
					return false;
				}
			}
		} else {
			// chunkheader >= 128. RLE data, next color repeated chunkheader-127 times.
			chunkheader -= 127;
			if (fread(colorbuffer, 1, bytes_per_pixel, file) != bytes_per_pixel) {
				free(colorbuffer);
				return false;
			}

			for(short counter = 0; counter < chunkheader; counter++) {
				image_data[currentbyte] = colorbuffer[2];
				image_data[currentbyte+1] = colorbuffer[1];
				image_data[currentbyte+2] = colorbuffer[0];

				if (bytes_per_pixel == 4) {
					image_data[currentbyte+3] = colorbuffer[3];
				}

				currentbyte += bytes_per_pixel;
				currentpixel++;

				if (currentpixel > pixelcount) {
					free(colorbuffer);
					return false;
				}
			}
		}
	} while (currentpixel < pixelcount);

	return true;
}

