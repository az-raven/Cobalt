#pragma once

#include <stddef.h>

namespace Cobalt::Graphics {
	class Coordinate {
		public:
			Coordinate(size_t x, size_t y) : _x(x), _y(y) { }

			inline size_t x() const {
				return _x;
			};
			inline size_t y() const {
				return _y;
			};

		private:
			size_t _x;
			size_t _y;
	};
}