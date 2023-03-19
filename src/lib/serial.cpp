#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "io.h"

#define PORT 0x3f8  // COM1

bool init_serial() {
    outbyte(PORT + 1, 0x00);    // Disable interrupts
    outbyte(PORT + 3, 0x80);    // Enable DLAB
    outbyte(PORT + 0, 0x01);    // Set divisor to HI (115200 baud, high byte);
    outbyte(PORT + 1, 0x00);    // Set divisor to HI (115200 baud, high byte);
    outbyte(PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    outbyte(PORT + 2, 0xC7);    // FIFO
    outbyte(PORT + 4, 0x0B);    // IRQs enabled
    outbyte(PORT + 4, 0x1E);    // Set Loopback Mode, for test
    outbyte(PORT + 0, 0xAE);    // Test chip

    // Check response
    if(inbyte(PORT + 0) != 0xAE) {
        // Serial port is faulty
        return false;
    }

    // set to normal operation
    outbyte(PORT + 4, 0x0F);
    return true;
}

static int recv_poll() {
    return inbyte(PORT + 5) & 1;
}

char read_serial() {
    while (recv_poll() == 0);

    return inbyte(PORT);
}

static int xmit_poll() {
    return inbyte(PORT + 5) & 0x20;
}

void write_serial(char in) {
    while (xmit_poll() == 0);

    outbyte(PORT, in);
}

void serial_writestring(const char *message) {
    for (size_t i = 0; message[i]; i++) {
        write_serial(message[i]);
    }
}