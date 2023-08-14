#include <stdint.h>
#include <stddef.h>
#include <init/limine_stub.h>
#include <string.h>

#include <kernel.h>

#include <util/todo.h>
#include <util/option.h>

#include <hardware/framebuffer.h>
#include <hardware/serial.h>

#include <graphics/fonts/basic8x8mono.h>
#include <graphics/coordinate.h>
#include <graphics/logo.h>

#include <arch/idt.h>

#include <util/debug.h>


using Cobalt::Arch::Interrupts::fixme_irq_nop;
using Cobalt::Arch::Interrupts::InterruptDescriptorTable;
using Cobalt::Graphics::Coordinate;
using Cobalt::Graphics::Framebuffer;
using Cobalt::Graphics::Fonts::Basic8x8Mono;
using Cobalt::Hardware::SerialPort;
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

static volatile struct limine_kernel_address_request kernel_address_request = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
};

void wait_a_sec() {
    for (size_t i = 0; i < 10000000000000000; i++) {
        asm volatile("nop");
    }
}

static InterruptDescriptorTable IDT = InterruptDescriptorTable();

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

    // FIXME: Whole bunch of unhandled errors here. I'm aware.
    const size_t x_offset = primary_framebuffer.has_value() ? (primary_framebuffer.value().width() - 360) / 2 : 0;
    const size_t y_offset = primary_framebuffer.has_value() ? (primary_framebuffer.value().height() - 360) / 2 : 0;

    primary_framebuffer.value().rect(
        Coordinate(0, y_offset),
        Coordinate(primary_framebuffer.value().width(), y_offset + 360),
        0x00'000050
    );

    primary_framebuffer.value().rect(
        Coordinate(0, y_offset + 360),
        Coordinate(primary_framebuffer.value().width(), y_offset + 360 + 50),
        0x00'FF0000
    );

    const uint32_t grey37 = 0x00'373737;
    const uint32_t grey38 = 0x00'383838;
    const uint32_t grey39 = 0x00'393939;

    for (size_t y = 1; y < 350; y++) {
        for (size_t x = 0; x < 360; x++) {
            const unsigned char r = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 0];
            const unsigned char g = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 1];
            const unsigned char b = COBALT_LOGO_BMP[(360*(360 - y) + x)*3 + 2];
            const uint32_t rgb = r << 16 | g << 8 | b;

            const uint32_t masked = rgb & 0x00'FFFFFF;
            if (masked == grey37 || masked == grey38 || masked == grey39) {
                continue;
            }

            primary_framebuffer.value().pixel(Coordinate(x_offset + x, y_offset + y), rgb);
        }
    }

    Basic8x8Mono font = Basic8x8Mono();

    primary_framebuffer.value().text(
        Coordinate(x_offset / 2, y_offset + 365),
        &font,
        8,
        5,
        "WELCOME TO COBALT OS",
        0x00'FFFFFF
    );

    primary_framebuffer.value().text(
        Coordinate(x_offset + 36, y_offset + 360 + 55),
        &font,
        8,
        3,
        "SETTING UP...",
        0x00'FFFFFF
    );

    // FIXME: This is fallible - handle errors.
    SerialPort serial_port = SerialPort::at(0x3F8).value();

    debugln("init", "good-ish morning!");

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

    IDT.load();

    debugln("init", "Calling...");
    main();

    // NOTE: Unreachable.
    TODO("Kernel::main() returned - this should be unreachable!");
}