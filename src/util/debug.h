#pragma once

#define debugger asm volatile("xchg %bx, %bx")