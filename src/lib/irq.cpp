#pragma once

#include <stdint.h>

#include "paging.cpp"
#include "serial.cpp"
#include "gdt.cpp"
#include "io.h"

typedef struct _IdtEntry {
    uint16_t    base_lo;
    uint16_t    segment;
    uint8_t     reserved;
    uint8_t     flags;
    uint16_t    base_hi;
} __attribute__((packed)) IdtEntry;

extern "C" void load_idt(uint32_t offset, uint16_t size);

extern "C" void gp_fault_asm(void);

extern "C" void pit_interrupt_asm(void);
extern "C" void keyboard_irq_asm(void);
extern "C" void syscall_asm(void);
extern "C" void no_handler(void);

static IdtEntry IDT[256];

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

/* reinitialize the PIC controllers, giving them specified vector offsets
   rather than 8h and 70h, as configured by default */

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04    /* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08	/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C	/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#define PIC_EOI		0x20		/* End-of-interrupt command code */

void eoi(uint8_t irq) {
    if(irq >= 8)
        outbyte(PIC2_COMMAND, PIC_EOI);

    outbyte(PIC1_COMMAND, PIC_EOI);
}

void remap_pic(int offset1, int offset2)
{
	unsigned char a1, a2;

	a1 = inbyte(PIC1_DATA);                        // save masks
	a2 = inbyte(PIC2_DATA);

	outbyte(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outbyte(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outbyte(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outbyte(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outbyte(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outbyte(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();

	outbyte(PIC1_DATA, ICW4_8086);
	io_wait();
	outbyte(PIC2_DATA, ICW4_8086);
	io_wait();

	outbyte(PIC1_DATA, a1);   // restore saved masks.
	outbyte(PIC2_DATA, a2);
}

static void idt_set_gate(uint8_t index, uint32_t base, uint16_t segment, uint8_t flags) {
    IDT[index].base_lo = base & 0xFFFF;
    IDT[index].base_hi = (base >> 16) & 0xFFFF;

    IDT[index].segment = segment;
    IDT[index].reserved = 0;
    IDT[index].flags = flags | 0x60;
}

extern "C" void key_pressed_irq(void) {
    uint8_t c = inbyte(0x60);
    eoi(1);
}

extern "C" void gp_fault_c(void) {
    // TODO: Add more information for logging and terminate offending processes.
    serial_writestring("General Protection Fault Occurred!");
    __asm__ volatile("cli");
    eoi(13);
    while (true) {
        __asm__ volatile("hlt");
    }
}

uint16_t IRQ_get_mask() {
    return inbyte(PIC1_DATA) | (inbyte(PIC2_DATA) << 8);
}

void IRQ_set_mask(uint16_t mask) {
    outbyte(PIC1_DATA, mask & 0xFFFF);
    outbyte(PIC2_DATA, (mask >> 8) & 0xFFFF);
}

void IRQ_set_unblocked(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inbyte(port) | (1 << IRQline);
    outbyte(port, value);        
}

void IRQ_set_blocked(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = inbyte(port) & ~(1 << IRQline);
    outbyte(port, value);        
}

extern "C" void no_handler_c() {
    serial_writestring("[IRQ] Unrecognized interrupt executed!\n\r");

    outbyte(PIC1_COMMAND, PIC_EOI);
    outbyte(PIC2_COMMAND, PIC_EOI);
}


void idt_init() {
    // Remap PIC
    remap_pic(0x20, 0x28);

    IRQ_set_mask(0);

    for (size_t i = 0; i < 256; i++)
        idt_set_gate((uint8_t)i, (uint32_t)no_handler, 0x08, 0x8E);

    // Exceptions
    idt_set_gate(13, (uint32_t)gp_fault_asm, 0x08, 0x8E);

    // IRQs
    idt_set_gate(0x20, (uint32_t)pit_interrupt_asm, 0x08, 0xEE);
    idt_set_gate(0x21, (uint32_t)keyboard_irq_asm, 0x08, 0x8E);

    // Interrupts
    idt_set_gate(0x80, (uint32_t)syscall_asm, 0x08, 0xEE);

    // Load the interrupt descriptor table register.
    load_idt((uint32_t)IDT, sizeof(IDT));

    // Enable interrupts.
    asm volatile("sti");

    serial_writestring("[IRQ] Interrupts enabled.\n\r");
}