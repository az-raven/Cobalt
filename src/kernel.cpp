#include "kernel.h"

#include <util/todo.h>

using Cobalt::Graphics::Framebuffer;
using Cobalt::Graphics::Coordinate;
using Cobalt::Utility::Option;

namespace Cobalt::Kernel {
    void wait_a_sec() {
        for (size_t i = 0; i < 100000000; i++) {
            asm volatile("nop");
        }
    }

    void main(
        Option<Framebuffer> primary_framebuffer
    ) {
        if (primary_framebuffer.has_value())
        {
            Framebuffer fb = primary_framebuffer.value();

            for (size_t x = 0; x < fb.width(); x++) {
                for (size_t y = 0; y < fb.height(); y++) {
                    fb.put_pixel(Coordinate(x, y), 0x00ff00);
                }
                wait_a_sec();
            }
        }

        TODO();
    }
}
