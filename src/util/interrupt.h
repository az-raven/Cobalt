#pragma once

#include <stdint.h>
#include <util/io.h>

#define INTERRUPT __attribute__((naked))
#define iretq asm volatile("iretq")

#define EOI(irq)              \
	do {                      \
		if (irq >= 8)         \
			outb(0xA0, 0x20); \
		outb(0x20, 0x20);     \
	} while (0)
