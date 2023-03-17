#ifndef HACKNET_KERNEL_SEC
#define HACKNET_KERNEL_SEC

#include <stdint.h>

// Self-hosters - you'll need to create this file yourself and #define STACK_CHK_GUARD manually.
// Change it between builds!
#include "SECURITY.h"

#include "lib/serial.cpp"
#include "lib/io.h"

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

extern "C"
__attribute__((noreturn))
void __stack_chk_fail(void) {
    __asm__ volatile("cli");

    // FIXME: We should never panic or restart unless something ridiculous happens like a machine check exception
    //        otherwise, we should take every possible action to ensure that the kernel **never** panics.

    serial_writestring("Kernel overrun occurred - restarting\n\r");

    io_wait_long();
    serial_writestring("Restart in 5...\n\r");
    io_wait_long();
    serial_writestring("4...\n\r");
    io_wait_long();
    serial_writestring("3...\n\r");
    io_wait_long();
    serial_writestring("2...\n\r");
    io_wait_long();
    serial_writestring("1...\n\r");
    serial_writestring("!! RESTARTING MUST KERNEL NOW !!\n\r");

    io_wait_long();

    __asm__ volatile("ljmp 0x0");
    while(true);
}

#endif