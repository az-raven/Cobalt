#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "lib/irq.cpp"
#include "lib/mm.cpp"

void sched_init() {

}

extern "C"
void sched_tick_irq(void) {
    // TODO
    eoi(0);
}