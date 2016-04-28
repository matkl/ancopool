#ifndef IMAGE_H
#define IMAGE_H

#include "widget.h"
#include "texture.h"

namespace GUI {
	class Image : public Widget {
		public:
			Image(Widget* parent = 0, const std::string& image_str = "");
			~Image();
			void set_image(const std::string& image_str);
			virtual void draw() const;
		private:
			Texture* texture;
	};
}

#endif // IMAGE_H
