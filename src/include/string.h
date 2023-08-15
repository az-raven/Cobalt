#pragma once

#include <stdint.h>
#include <stddef.h>

extern "C" void *memcpy(void *, const void *, size_t);
extern "C" void *memset(void *, int, size_t);
extern "C" void *memmove(void *, const void *, size_t);
extern "C" int memcmp(const void *, const void *, size_t);
extern "C" size_t strlen(const char *);