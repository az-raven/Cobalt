#pragma once

#include "serial.cpp"

#include <stdint.h>

static uint32_t TSS[0x68] = {0};

static uint64_t GDT[6];

void create_descriptor(uint8_t *target, uint32_t limit, uint32_t base, uint8_t access_byte, uint8_t flags) {

    // Encode the limit
    target[0] = limit & 0xFF;
    target[1] = (limit >> 8) & 0xFF;
    target[6] = (limit >> 16) & 0x0F;

    // Encode the base
    target[2] = base & 0xFF;
    target[3] = (base >> 8) & 0xFF;
    target[4] = (base >> 16) & 0xFF;
    target[7] = (base >> 24) & 0xFF;

    // Encode the access byte
    target[5] = access_byte;

    // Encode the flags
    target[6] |= (flags << 4);
}

extern "C" void set_gdt(uint16_t limit, uint8_t *base);
extern "C" void refresh_cs();

void gdt_init() {
    create_descriptor((uint8_t *)GDT, 0, 0, 0, 0);                 // Null Descriptor

    create_descriptor((uint8_t *)(GDT + 1), 0xFFFFF, 0, 0x9A, 0xC);  // Kernel Mode Code Segment
    create_descriptor((uint8_t *)(GDT + 2), 0xFFFFF, 0, 0x92, 0xC);  // Kernel Mode Data Segment

    create_descriptor((uint8_t *)(GDT + 3), 0xFFFFF, 0, 0xFA, 0xC);  // User Mode Code Segment
    create_descriptor((uint8_t *)(GDT + 4), 0xFFFFF, 0, 0xF2, 0xC);  // User Mode Data Segment

    create_descriptor((uint8_t *)TSS, sizeof(TSS), 0, 0x89, 0x9);  // Task State Segment

    serial_writestring("[GDT] Created all descriptors!\n\r");

    set_gdt(sizeof(uint64_t) * 6, (uint8_t *)GDT);

    serial_writestring("[GDT] Loaded GDT!\n\r");

    serial_writestring("[GDT] Refreshing CS...\n\r");

    refresh_cs();

    serial_writestring("[GDT] No bootloop? Cool! CS refreshed successfully!\n\r");
}