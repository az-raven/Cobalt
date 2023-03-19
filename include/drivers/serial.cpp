#pragma once

#include "serial.h"

namespace cobalt {
	class SerialPort {
		public:
			static Maybe<SerialPort> create(uint16_t base) {
				SerialPort maybe = SerialPort(base);
				maybe.setup();

				if(!maybe.test())
					return Maybe<SerialPort>::none();

				return Maybe<SerialPort>::some(maybe);
			}
		private:
			SerialPort(uint16_t base) {
				this->base = base;
			}

			void SerialPort::setup() {
				// TODO setup the serial port.
			}

			bool SerialPort::test() {
				// TODO test the serial port.
			}

			uint16_t base;
	};
}