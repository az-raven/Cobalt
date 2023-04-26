#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/// 64-Bit standardized entry point for all architectures.
extern "C"
void kernel_main() {

    while(true) {
        asm volatile("hlt");
    }
}