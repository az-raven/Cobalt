#include <stdint.h>
#include <stddef.h>
#include <init/limine_stub.h>
#include <string.h>

#include <kernel.h>

#include <util/todo.h>
#include <util/option.h>

#include <hardware/serial.h>

#include <graphics/splash/splash_screen.h>
#include <graphics/framebuffer.h>

#include <arch/idt.h>

#include <util/debug.h>

#include <mm/info.h>

using Cobalt::Arch::Interrupts::fixme_irq_nop;
using Cobalt::Arch::Interrupts::InterruptDescriptorTable;
using Cobalt::Graphics::Framebuffer;
using Cobalt::Graphics::SplashScreen;
using Cobalt::Hardware::SerialPort;
using Cobalt::Kernel::main;
using Cobalt::Utility::Option;

using namespace Cobalt::Memory::Info;

#pragma GCC diagnostic push
// NOTE: `response` is intentionally uninitialized, as Limine will set it.
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
// NOTE: `response` is intentionally uninitialized, as Limine will set it.
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};
#pragma GCC diagnostic pop

static InterruptDescriptorTable IDT = InterruptDescriptorTable();

template<typename OUT, typename IN>
OUT ForceCast( IN in )
{
    union
    {
        IN  in;
        OUT out;
    }
    u = { in };

    return u.out;
};


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


    // FIXME: This is fallible - handle errors.
    SerialPort serial_port = SerialPort::at(0x3F8).value();

    SplashScreen splash = SplashScreen(primary_framebuffer.value());

    splash.initial_render();

    if (memmap_info.has_value()) {
        const limine_memmap_response *memmap = memmap_info.value();
        uint64_t total_usable = 0;
        uint64_t bad_memory = 0;

        serial_port.puts("Memory map info from Limine [ALL NUMBERS IN HEX (base 16)]:\n\r");
        for (uint64_t i = 0; i < memmap->entry_count; i++) {
            const limine_memmap_entry entry = *memmap->entries[i];

            serial_port.puts("- [ 0x");
            serial_port.putd_hex(entry.base);
            serial_port.puts(" - 0x");
            serial_port.putd_hex(entry.base + entry.length);
            serial_port.puts(" ] (0x");
            serial_port.putd_hex(entry.length);
            serial_port.puts(" bytes)\n\r");

            serial_port.puts("  - Type: ");
            switch (entry.type) {
                case LIMINE_MEMMAP_USABLE:
                    serial_port.puts("Usable (0).\n\r");
                    total_usable += entry.length;
                    break;
                case LIMINE_MEMMAP_RESERVED:
                    serial_port.puts("Reserved (1).\n\r");
                    break;
                case LIMINE_MEMMAP_ACPI_RECLAIMABLE:
                    serial_port.puts("ACPI Reclaimable (2).\n\r");
                    break;
                case LIMINE_MEMMAP_ACPI_NVS:
                    serial_port.puts("ACPI NVS (3).\n\r");
                    break;
                case LIMINE_MEMMAP_BAD_MEMORY:
                    serial_port.puts("Bad Memory (4).\n\r");
                    bad_memory += entry.length;
                    break;
                case LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE:
                    serial_port.puts("Bootloader Reclaimable (5).\n\r");
                    break;
                case LIMINE_MEMMAP_KERNEL_AND_MODULES:
                    serial_port.puts("Kernel and Modules (6).\n\r");
                    break;
                case LIMINE_MEMMAP_FRAMEBUFFER:
                    serial_port.puts("Framebuffer (7).\n\r");
                    break;
                default:
                    serial_port.puts("Malformed (?).\n\r");
                    break;
            }
        }
        serial_port.puts("\n\r");

        serial_port.puts("Total usable memory: 0x");
        serial_port.putd_hex(total_usable);
        serial_port.puts(" bytes\n\r");

        serial_port.puts("Total bad memory: 0x");
        serial_port.putd_hex(bad_memory);
        serial_port.puts(" bytes\n\r");
    } else {
        serial_port.puts("No memory map info from Limine.\n\r");
    }

    splash.update_status("MEMORY CHECK OK!");

    serial_port.puts("Kernel end address is at 0x");
    serial_port.putd_hex((uint64_t)&KERNEL_END);
    serial_port.puts("\n\r");

    IDT.load();

    splash.update_status("IDT LOADED OK!");

    debugln("init", "Calling...");
    splash.update_status("STARTING KERNEL!");
    main();

    // NOTE: Unreachable.
    TODO();
}