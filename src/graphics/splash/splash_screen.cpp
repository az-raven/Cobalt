#include "splash_screen.h"

#include <string.h>

#include <graphics/logo.h>
#include <graphics/fonts/basic8x8mono.h>

using Cobalt::Graphics::Fonts::Basic8x8Mono;

namespace Cobalt::Graphics {
	void SplashScreen::initial_render() {
		const size_t x_offset = (this->_framebuffer.width() - 360) / 2;
		const size_t y_offset = (this->_framebuffer.height() - 360) / 2;

		this->_framebuffer.rect(
			Coordinate(0, y_offset),
			Coordinate(this->_framebuffer.width(), y_offset + 360),
			0x00'000050
		);

		const uint32_t grey37 = 0x00'373737;
		const uint32_t grey38 = 0x00'383838;
		const uint32_t grey39 = 0x00'393939;

		for (size_t y = 1; y < 350; y++) {
			for (size_t x = 0; x < 360; x++) {
				const unsigned char r = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 0];
				const unsigned char g = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 1];
				const unsigned char b = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 2];
				const uint32_t rgb = r << 16 | g << 8 | b;

				const uint32_t masked = rgb & 0x00'FFFFFF;
				if (masked == grey37 || masked == grey38 || masked == grey39) {
					continue;
				}

				this->_framebuffer.pixel(Coordinate(x_offset + x, y_offset + y), rgb);
			}
		}

		Basic8x8Mono font = Basic8x8Mono();

		update_status("SETTING UP...");
	}

	void SplashScreen::update_status(const char *status_text) {
		Basic8x8Mono font = Basic8x8Mono();

		const size_t y_offset = (this->_framebuffer.height() - 360) / 2;

		this->_framebuffer.rect(
			Coordinate(0, y_offset + 360),
			Coordinate(this->_framebuffer.width(), y_offset + 360 + 84),
			0x00'FF0000
		);


		const size_t welcome_text_width = 8 * 5 * 21;
		const size_t welcome_text_x = (this->_framebuffer.width() - welcome_text_width) / 2;
		this->_framebuffer.text(
			Coordinate(welcome_text_x, y_offset + 365),
			&font,
			8,
			5,
			"WELCOME TO COBALT OS",
			0x00'FFFFFF
		);

		const size_t status_text_width = 8 * 3 * strlen(status_text);
		const size_t status_text_x = (this->_framebuffer.width() - status_text_width) / 2;
		this->_framebuffer.text(
			Coordinate(status_text_x, y_offset + 360 + 55),
			&font,
			8,
			3,
			status_text,
			0x00'FFFFFF
		);
	}
}
