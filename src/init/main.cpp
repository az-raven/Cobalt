#include <stdint.h>
#include <stddef.h>
#include <init/limine_stub.h>
#include <string.h>
#include <kernel.h>

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void _start(void) {
    // Fetch the first framebuffer.
    struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[0];
 
    for (size_t y = 0; y < framebuffer->height; y++) {
        for (size_t x = 0; x < framebuffer->width; x++) {
            uint32_t *fb_addr = (uint32_t *)framebuffer->address;
            fb_addr[y * (framebuffer->pitch/4) + (x*framebuffer->bpp/(32))] = 0xFFFF2222;
        }
    }

    while(true) {
        ;
    }
}