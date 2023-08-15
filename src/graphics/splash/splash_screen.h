#pragma once

#include <graphics/framebuffer.h>

using Cobalt::Graphics::Framebuffer;

namespace Cobalt::Graphics {
	class SplashScreen {
		public:
			SplashScreen(Framebuffer framebuffer) : _framebuffer(framebuffer) { };

			void initial_render();
			void update_status(const char *);

		private:
			Framebuffer _framebuffer;
	};
}