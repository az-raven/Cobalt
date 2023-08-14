#pragma once

#include <stdint.h>
#include <stddef.h>

#include <util/io.h>
#include <util/option.h>

using Cobalt::Utility::Option;

#define debugln(cat, msg) do { Cobalt::Hardware::SerialPort::at_unchecked(0x3F8).debug_puts((cat), __FILE__, __LINE__, (msg)); } while (0)

namespace Cobalt::Hardware {
	const uint16_t POTENTIAL_SERIAL_PORT_ADDRS[8] = {
		0x3F8, // COM1
		0x2F8, // COM2

		0x3E8, // COM3?
		0x2E8, // COM4?

		0x5F8, // COM5??
		0x4F8, // COM6??

		0x5E8, // COM7???
		0x4E8  // COM8???
	};

	class SerialPort {
		public:
			SerialPort() {}
			SerialPort(uint16_t port) : _port(port) { }

			static Option<SerialPort> at(uint16_t port) {
				outbyte(port + 1, 0x00); 			// No interrupts
				outbyte(port + 3, 0x80);		
				outbyte(port + 0, 0x03);			// 38400 baud
				outbyte(port + 1, 0x00);
				outbyte(port + 3, 0x03);			// 8 bits, no parity, 1 stop bit
				outbyte(port + 2, 0xC7);			// FIFO
				outbyte(port + 4, 0x0B);			// Enable interrupts

				outbyte(port + 4, 0x1E);			// Loopback mode
				outbyte(port + 0, 0b0101'1010);		// Test

				if (inbyte(port + 0) != 0b0101'1010) {
					return Option<SerialPort>::none();
				}

				outbyte(port + 4, 0x0F);			// Normal mode
				return Option<SerialPort>::some(SerialPort(port));
			}

			static inline SerialPort at_unchecked(uint16_t addr) {
				return SerialPort(addr);
			}

			void puts(const char* str) {
				for (size_t i = 0; str[i]; i++) {
					write(str[i]);
				}
			}

			inline void putd(uint64_t decimal) {
				if (decimal == 0) {
					this->write('0');
					return;
				}

				uint64_t digits = 0;
				uint64_t temp = decimal;
				while (temp > 0) {
					temp /= 10;
					digits++;
				}

				char buffer[digits + 1];
				buffer[digits] = '\0';

				for (uint64_t i = 0; i < digits; i++) {
					buffer[digits - i - 1] = (decimal % 10) + '0';
					decimal /= 10;
				}

				this->puts(buffer);
			}

			inline void putd_hex(uint64_t hex) {
				if (hex == 0) {
					this->write('0');
					return;
				}

				uint64_t digits = 0;
				uint64_t temp = hex;
				while (temp > 0) {
					temp /= 16;
					digits++;
				}

				char buffer[digits + 1];
				buffer[digits] = '\0';

				for (uint64_t i = 0; i < digits; i++) {
					uint8_t digit = hex % 16;
					if (digit < 10) {
						buffer[digits - i - 1] = digit + '0';
					} else {
						buffer[digits - i - 1] = (digit - 10) + 'A';
					}
					hex /= 16;
				}

				this->puts(buffer);
			}

			inline void putd_bin(uint64_t binary) {
				if (binary == 0) {
					this->puts("0000'0000 0000'0000 0000'0000 0000'0000 ; 0000'0000 0000'0000 0000'0000 0000'0000");
					return;
				}

				// FIXME: This isn't very clean right now, but it works.

				this->write((binary >> 63 & 1) ? '1' : '0');
				this->write((binary >> 62 & 1) ? '1' : '0');
				this->write((binary >> 61 & 1) ? '1' : '0');
				this->write((binary >> 60 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 59 & 1) ? '1' : '0');
				this->write((binary >> 58 & 1) ? '1' : '0');
				this->write((binary >> 57 & 1) ? '1' : '0');
				this->write((binary >> 56 & 1) ? '1' : '0');

				this->write(' ');

				this->write((binary >> 55 & 1) ? '1' : '0');
				this->write((binary >> 54 & 1) ? '1' : '0');
				this->write((binary >> 53 & 1) ? '1' : '0');
				this->write((binary >> 52 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 51 & 1) ? '1' : '0');
				this->write((binary >> 50 & 1) ? '1' : '0');
				this->write((binary >> 49 & 1) ? '1' : '0');
				this->write((binary >> 48 & 1) ? '1' : '0');

				this->write(' ');

				this->write((binary >> 47 & 1) ? '1' : '0');
				this->write((binary >> 46 & 1) ? '1' : '0');
				this->write((binary >> 45 & 1) ? '1' : '0');
				this->write((binary >> 44 & 1) ? '1' : '0');
				
				this->write('\'');

				this->write((binary >> 43 & 1) ? '1' : '0');
				this->write((binary >> 42 & 1) ? '1' : '0');
				this->write((binary >> 41 & 1) ? '1' : '0');
				this->write((binary >> 40 & 1) ? '1' : '0');

				this->write(' ');

				this->write((binary >> 39 & 1) ? '1' : '0');
				this->write((binary >> 38 & 1) ? '1' : '0');
				this->write((binary >> 37 & 1) ? '1' : '0');
				this->write((binary >> 36 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 35 & 1) ? '1' : '0');
				this->write((binary >> 34 & 1) ? '1' : '0');
				this->write((binary >> 33 & 1) ? '1' : '0');
				this->write((binary >> 32 & 1) ? '1' : '0');
				
				this->puts(" : ");

				this->write((binary >> 31 & 1) ? '1' : '0');
				this->write((binary >> 30 & 1) ? '1' : '0');
				this->write((binary >> 29 & 1) ? '1' : '0');
				this->write((binary >> 28 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 27 & 1) ? '1' : '0');
				this->write((binary >> 26 & 1) ? '1' : '0');
				this->write((binary >> 25 & 1) ? '1' : '0');
				this->write((binary >> 24 & 1) ? '1' : '0');

				this->write(' ');

				this->write((binary >> 23 & 1) ? '1' : '0');
				this->write((binary >> 22 & 1) ? '1' : '0');
				this->write((binary >> 21 & 1) ? '1' : '0');
				this->write((binary >> 20 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 19 & 1) ? '1' : '0');
				this->write((binary >> 18 & 1) ? '1' : '0');
				this->write((binary >> 17 & 1) ? '1' : '0');
				this->write((binary >> 16 & 1) ? '1' : '0');

				this->write(' ');

				this->write((binary >> 15 & 1) ? '1' : '0');
				this->write((binary >> 14 & 1) ? '1' : '0');
				this->write((binary >> 13 & 1) ? '1' : '0');
				this->write((binary >> 12 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 11 & 1) ? '1' : '0');
				this->write((binary >> 10 & 1) ? '1' : '0');
				this->write((binary >> 9 & 1) ? '1' : '0');
				this->write((binary >> 8 & 1) ? '1' : '0');

				this->write(' ');

				this->write((binary >> 7 & 1) ? '1' : '0');
				this->write((binary >> 6 & 1) ? '1' : '0');
				this->write((binary >> 5 & 1) ? '1' : '0');
				this->write((binary >> 4 & 1) ? '1' : '0');

				this->write('\'');

				this->write((binary >> 3 & 1) ? '1' : '0');
				this->write((binary >> 2 & 1) ? '1' : '0');
				this->write((binary >> 1 & 1) ? '1' : '0');
				this->write((binary >> 0 & 1) ? '1' : '0');
			}

			inline void debug_puts(const char *category, const char *path, int line, const char *str) {
				this->write('[');
				this->puts(category);
				this->write(']');
				this->write(' ');
				this->puts(str);
				this->puts("\n\r\t(at ");
				this->puts(path);
				this->puts(":");
				this->putd(line);
				this->puts(")\n\r");
			}

			inline void write(uint8_t byte) {
				while ((inbyte(_port + 5) & 0x20) == 0);
				outbyte(_port, byte);
			}

			inline uint8_t read() {
				while ((inbyte(_port + 5) & 0x01) == 0);
				return inbyte(_port);
			}

		private:
			uint16_t _port;
	};
}