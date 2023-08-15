#pragma once

#include <stdint.h>
#include <stddef.h>

#include <graphics/coordinate.h>
#include <graphics/bitmap.h>
#include <graphics/font.h>

using Cobalt::Graphics::Bitmap;
using Cobalt::Graphics::Coordinate;
using Cobalt::Graphics::Font;

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

			inline void pixel(Coordinate coords, uint32_t color) {
				uint32_t *fb_addr = (uint32_t *)this->_address;
				fb_addr[coords.y() * (this->_pitch/4) + (coords.x()*this->_bpp/32)] = color;
			}

			inline void rect(Coordinate a, Coordinate b, uint32_t color) {
				for (size_t y = a.y(); y < b.y(); y++) {
					for (size_t x = a.x(); x < b.x(); x++) {
						this->pixel(Coordinate(x, y), color);
					}
				}
			}

			inline void clear(uint32_t color) {
				for (size_t i = 0; i < this->_height; i++) {
					for (size_t j = 0; j < this->_width; j++) {
						uint32_t *fb_addr = (uint32_t *)this->_address;
						fb_addr[i * (this->_pitch/4) + (j*this->_bpp/32)] = color;
					}
				}
			}

			inline void bitmap(Coordinate upper_left, Bitmap bitmap, uint32_t color, int scale) {
				for (size_t y = 0; y < bitmap.height() * scale; y += scale) {
					for (size_t x = 0; x < bitmap.width() * scale; x += scale) {
						if (scale == 1) {
							if (bitmap.at(Coordinate(x, y))) {
								this->pixel(Coordinate(upper_left.x() + x, upper_left.y() + y), color);
							}
						} else {
							if (bitmap.at(Coordinate(x / scale, y / scale))) {
								this->rect(Coordinate(upper_left.x() + x, upper_left.y() + y), Coordinate(upper_left.x() + x + scale, upper_left.y() + y + scale), color);
							}
						}

					}
				}
			}

			inline void tile_bitmap(Coordinate upper_left, Bitmap bitmap, size_t width, size_t height, uint32_t color) {
				for (size_t y = 0; y < height; y++) {
					for (size_t x = 0; x < width; x++) {
						if (bitmap.at(Coordinate(x % bitmap.width(), y % bitmap.height()))) {
							this->pixel(Coordinate(upper_left.x() + x, upper_left.y() + y), color);
						}
					}
				}
			}

			inline void text(Coordinate upper_left, Font *font, size_t line_spacing, int scale, const char* text, uint32_t color) {
				size_t x = upper_left.x();
				size_t y = upper_left.y();
				for (size_t i = 0; text[i]; i++) {
					if (text[i] == '\n') {
						y += line_spacing * scale;
						x = upper_left.x();
						continue;
					}

					Bitmap bmp = font->get(text[i]);
					this->bitmap(Coordinate(x, y), font->get(text[i]), color, scale);
					x += bmp.width() * scale;
				}
			}
		private:
			uint8_t *_address;
			size_t _width;
			size_t _height;
			size_t _pitch;
			size_t _bpp;
	};
}
