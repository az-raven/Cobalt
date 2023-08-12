#pragma once

#include <stdint.h>
#include <stddef.h>

#include <graphics/coordinate.h>

namespace Cobalt::Graphics {
	class Framebuffer {
		public:
			Framebuffer() {}
			Framebuffer(uint8_t *address, size_t width, size_t height, size_t pitch, size_t bpp) : 
				_address(address), _width(width), _height(height), _pitch(pitch), _bpp(bpp) { };

			inline size_t width() const {
				return this->_width;
			}
			inline size_t height() const {
				return this->_height;
			}

			inline void put_pixel(Coordinate coords, uint32_t color) {
				uint32_t *fb_addr = (uint32_t *)this->_address;
				fb_addr[coords.y() * (this->_pitch/4) + (coords.x()*this->_bpp/32)] = color;
			}
		private:
			uint8_t *_address;
			size_t _width;
			size_t _height;
			size_t _pitch;
			size_t _bpp;
	};
}
