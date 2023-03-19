#pragma once

#include <stddef.h>
#include <stdint.h>

#include "mm.cpp"
#include "gdt.cpp"

extern "C" void load_page_directory(uint32_t *);
extern "C" void enable_paging();
extern "C" void remap_k();

uint32_t page_directory[1024] __attribute__((aligned(4096)));

static unsigned int setup_page_table() {
    void *free_handle; // yes, this is an intentional memory leak.
    uint32_t *table = (uint32_t *)kmalloc_aligned(1024 * sizeof(uint32_t), 4096, &free_handle);

    for(int i = 0; i < 1024; i++) {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes ;)
        table[i] = (i * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    }

    return (unsigned int)table | 3;
}

void paging_init() {
    for (int i = 0; i < 1024; i++)
        page_directory[i] = setup_page_table();

    load_page_directory(page_directory);
    enable_paging();

    serial_writestring("[PAGING] Refreshing CS...\n\r");

    refresh_cs();

    serial_writestring("[PAGING] No bootloop? Cool! CS refreshed successfully!\n\r");
}