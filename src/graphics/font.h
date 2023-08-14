#pragma once

#include <graphics/bitmap.h>

using Cobalt::Graphics::Bitmap;

namespace Cobalt::Graphics {
	class Font {
		public:
			virtual Bitmap get(char _) = 0;
	};
}