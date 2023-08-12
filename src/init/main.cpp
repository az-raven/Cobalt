#include <stdint.h>
#include <stddef.h>
#include <init/limine_stub.h>
#include <string.h>

#include <kernel.h>
#include <util/todo.h>
#include <util/option.h>
#include <hardware/framebuffer.h>
#include <graphics/coordinate.h>
#include <graphics/logo.h>

using Cobalt::Graphics::Coordinate;
using Cobalt::Graphics::Framebuffer;
using Cobalt::Kernel::main;
using Cobalt::Utility::Option;

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

void wait_a_sec() {
    for (size_t i = 0; i < 10000000000000000; i++) {
        asm volatile("nop");
    }
}

extern "C"
void _start(void) {
    // welcome, take off your shoes, get comfy

    Option<limine_framebuffer_response *> fb_info = framebuffer_request.response != NULL ? 
        Option<limine_framebuffer_response *>::some(framebuffer_request.response) :
        Option<limine_framebuffer_response *>::none();

    Option<limine_memmap_response *> memmap_info = memmap_request.response != NULL ?
        Option<limine_memmap_response *>::some(memmap_request.response) :
        Option<limine_memmap_response *>::none();

    Option<Framebuffer> primary_framebuffer = Option<Framebuffer>::none();
    if (fb_info.has_value()) [[likely]] {
        uint64_t num_fbs = fb_info.value()->framebuffer_count;

        if (num_fbs > 0) [[likely]] {
            for (uint64_t i = 0; i < num_fbs; i++) {
                if (fb_info.value()->framebuffers[i]->memory_model ==LIMINE_FRAMEBUFFER_RGB) [[likely]] {
                    primary_framebuffer = Option<Framebuffer>::some(
                        Framebuffer(
                            (uint8_t *)fb_info.value()->framebuffers[i]->address,
                            fb_info.value()->framebuffers[i]->width,
                            fb_info.value()->framebuffers[i]->height,
                            fb_info.value()->framebuffers[i]->pitch,
                            fb_info.value()->framebuffers[i]->bpp
                        )
                    );
                    break;
                }
            }
        }
    }
    
    const size_t x_offset = primary_framebuffer.has_value() ? (primary_framebuffer.value().width() - 360) / 2 : 0;
    const size_t y_offset = primary_framebuffer.has_value() ? (primary_framebuffer.value().height() - 360) / 2 : 0;
    const uint32_t grey37 = 0x00373737;
    const uint32_t grey38 = 0x00383838;
    const uint32_t grey39 = 0x00393939;
    for (size_t y = 1; y < 350; y++) {
        for (size_t x = 0; x < 360; x++) {
            const unsigned char r = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 0];
            const unsigned char g = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 1];
            const unsigned char b = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 2];
            const uint32_t rgb = r << 16 | g << 8 | b;

            const uint32_t masked = rgb & 0x00FFFFFF;
            if (masked == grey37 || masked == grey38 || masked == grey39) {
                continue;
            }

            primary_framebuffer.value().put_pixel(Coordinate(x_offset + x, y_offset + y), rgb);
        }
    }

    // NOTE: Unreachable.
    TODO();
}