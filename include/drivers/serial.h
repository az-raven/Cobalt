#pragma once

#include <stdint.h>

#include "../cobalt/maybe.h"

namespace cobalt {
	class SerialPort {
		public:
			/// @brief Creates a new serial port, initializes it, and tests it.
			/// @param base The base IO port to use for this COM port.
			/// @return Some if the serial port passed testing, None otherwise.
			static Maybe<SerialPort> create(uint16_t base);

		private:
			void SerialPort::setup();

			bool SerialPort::test();

			SerialPort(uint16_t base);

			uint16_t base;
	};
}