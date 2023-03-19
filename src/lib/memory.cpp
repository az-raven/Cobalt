#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/// @brief Performs a byte-by-byte comparison of two buffers.
/// @param a The LHS buffer.
/// @param b The RHS buffer.
/// @param size The number of bytes to compare.
/// @return `true` (1) if the values are the same in the specified region, `false` (0) otherwise.
int memcmp(const void* a, const void* b, size_t size) {
    uint8_t *cast_a = (uint8_t *)a;
    uint8_t *cast_b = (uint8_t *)b;

    for (size_t i = 0; i < size; i++)
        if(cast_a[i] != cast_b[i]) return false;

    return true;
}

/// @brief Copies a buffer SRC into buffer DEST.
/// @param src The source buffer to copy FROM.
/// @param dest The destination buffer to overwrite.
/// @param size The number of bytes to copy.
/// @return A pointer to the beginning of the destination buffer.
void* memcpy(const void* src, void* dest, size_t size) {
    uint8_t *cast_src = (uint8_t *)src;
    uint8_t *cast_dest = (uint8_t *)dest;

    for (size_t i = 0; i < size; i++)
        cast_dest[i] = cast_src[i];

    return dest;
}

/// @brief Moves a buffer SRC into a buffer DEST and sets SRC to 0s along the way.
/// @param src The source buffer to move from and re-initialize.
/// @param dest The destination buffer to overwrite.
/// @param len The number of bytes to move.
/// @return A pointer to the beginning of the destination buffer.
void* memmove(void* src, void* dest, size_t len) {
    uint8_t *cast_src = (uint8_t *)src;
    uint8_t *cast_dest = (uint8_t *)dest;

    for (size_t i = 0; i < len; i++)
    {
        cast_dest[i] = cast_src[i];
        cast_src[i] = 0;
    }

    return dest;
}

/// @brief Sets all bytes in the specified buffer to the specified value.
/// @param a The buffer to overwrite.
/// @param value The value to overwrite all bytes with.
/// @param size The number of bytes to overwrite.
/// @return A pointer to the beginning of the buffer.
void* memset(void* a, int value, size_t size) {
    uint8_t *buf = (uint8_t *)a;

    for (size_t i = 0; i < size; i++)
        buf[i] = value;

    return a;
}