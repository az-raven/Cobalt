#include "kernel.h"

#include <util/todo.h>
#include <hardware/serial.h>

using Cobalt::Graphics::Framebuffer;
using Cobalt::Graphics::Coordinate;
using Cobalt::Utility::Option;

namespace Cobalt::Kernel {
    void main(
        // Option<Framebuffer> primary_framebuffer
    ) {
        

        debugln("main", "Hello, world!");

        TODO();
    }
}
