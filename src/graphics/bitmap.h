#pragma once

#include <stdint.h>
#include <stddef.h>

#include <graphics/coordinate.h>

using Cobalt::Graphics::Coordinate;

namespace Cobalt::Graphics {
	class Bitmap {
		public:
			Bitmap() {}
			Bitmap(uint8_t *data, size_t width, size_t height) : _data(data), _width(width), _height(height) {}

			inline bool at(Coordinate c) {
				size_t pixel_index = c.y() * this->_width + c.x();
				size_t byte_index = pixel_index / 8;
				size_t bit_index = 8 - pixel_index % 8;
				return (this->_data[byte_index] >> bit_index) & 1;
			}
			inline uint8_t* data() const { return _data; }
			inline size_t width() const { return _width; }
			inline size_t height() const { return _height; }

		private:
			uint8_t* _data;
			size_t _width;
			size_t _height;
	};
}