#pragma once

#include <stdint.h>

static inline void outbyte(uint16_t port, uint8_t value) {
	asm volatile("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

static inline void outword(uint16_t port, uint16_t value) {
	asm volatile("outw %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

static inline void outdouble(uint16_t port, uint32_t value) {
	asm volatile("outl %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

static inline void outquad(uint16_t port, uint64_t value) {
	asm volatile("outq %0, %1" : : "a"(value), "Nd"(port) : "memory");
}

static inline uint8_t inbyte(uint16_t port) {
	uint8_t v;
	asm volatile("inb %1, %0" : "=a"(v) : "Nd"(port) : "memory");
	return v;
}

static inline uint16_t inword(uint16_t port) {
	uint16_t v;
	asm volatile("inw %1, %0" : "=a"(v) : "Nd"(port) : "memory");
	return v;
}

static inline uint32_t indouble(uint16_t port) {
	uint32_t v;
	asm volatile("inl %1, %0" : "=a"(v) : "Nd"(port) : "memory");
	return v;
}

static inline uint64_t inquad(uint16_t port) {
	uint64_t v;
	asm volatile("inq %1, %0" : "=a"(v) : "Nd"(port) : "memory");
	return v;
}