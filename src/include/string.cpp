#include "string.h"

extern "C" inline
void *memcpy(void *dest, const void *src, size_t n) {
	size_t remaining_bytes;
	size_t qwords = n / 8;

	uint8_t *dest_bytes = (uint8_t *)dest;
	const uint8_t *src_bytes = (uint8_t *)src;

	for (size_t i = 0; i < qwords; i++) {
		((uint64_t *)dest_bytes)[i] = ((uint64_t *)src_bytes)[i];
	}

	dest_bytes += qwords * 8;
	src_bytes += qwords * 8;

	remaining_bytes = n % 8;
	size_t dwords = remaining_bytes / 4;

	for (size_t i = 0; i < dwords; i++) {
		((uint32_t *)dest_bytes)[i] = ((uint32_t *)src_bytes)[i];
	}

	dest_bytes += dwords * 4;
	src_bytes += dwords * 4;

	remaining_bytes = remaining_bytes % 4;
	size_t words = remaining_bytes / 2;

	for (size_t i = 0; i < words; i++) {
		((uint16_t *)dest_bytes)[i] = ((uint16_t *)src_bytes)[i];
	}

	dest_bytes += words * 2;
	src_bytes += words * 2;

	remaining_bytes = remaining_bytes % 2;
	size_t bytes = remaining_bytes;

	for (size_t i = 0; i < bytes; i++) {
		dest_bytes[i] = src_bytes[i];
	}
 
    return dest;
}

extern "C" inline
void *memset(void *dest, int c, size_t remaining_bytes) {
	uint8_t *dest_bytes = (uint8_t *)dest;
	size_t qwords = remaining_bytes / 8;

	uint8_t byte = (uint8_t)c;
	uint64_t qword =
		((uint64_t)byte << 56) |
		((uint64_t)byte << 48) |
		((uint64_t)byte << 40) |
		((uint64_t)byte << 32) |
		((uint64_t)byte << 24) |
		((uint64_t)byte << 16) |
		((uint64_t)byte << 8) |
		((uint64_t)byte << 0);
	uint32_t dword =
		((uint32_t)byte << 24) |
		((uint32_t)byte << 16) |
		((uint32_t)byte << 8) |
		((uint32_t)byte << 0);
	uint16_t word =
		((uint16_t)byte << 8) |
		((uint16_t)byte << 0);

	for (size_t i = 0; i < qwords; i++)
	{
		((uint64_t *)dest_bytes)[i] = dword;
	}

	dest_bytes += qwords * 8;
	remaining_bytes %= 8;

	size_t dwords = remaining_bytes / 4;

	for (size_t i = 0; i < dwords; i++)
	{
		((uint32_t *)dest_bytes)[i] = dword;
	}

	dest_bytes += dwords * 4;
	remaining_bytes %= 4;

	size_t words = remaining_bytes / 2;

	for (size_t i = 0; i < words; i++)
	{
		((uint16_t *)dest_bytes)[i] = word;
	}

	dest_bytes += words * 2;
	remaining_bytes %= 2;

	for (size_t i = 0; i < remaining_bytes; i++) {
        dest_bytes[i] = (uint8_t)c;
    }
 
    return dest;
}

extern "C" inline
void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *pdest = (uint8_t *)dest;
    const uint8_t *psrc = (const uint8_t *)src;
 
    if (src > dest) {
        for (size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }
 
    return dest;
}

extern "C" inline
int memcmp(const void *s1, const void *s2, size_t n) {
    const uint8_t *p1 = (const uint8_t *)s1;
    const uint8_t *p2 = (const uint8_t *)s2;
 
    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }
 
    return 0;
}
