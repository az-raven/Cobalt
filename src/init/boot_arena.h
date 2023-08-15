#pragma once

// This provides a relatively small region of allocatable
// memory that can be used during the boot process, before
// other memory regions are added to the Memory Manager.

#include <stdint.h>

namespace Cobalt::Memory {
	const uint8_t BOOT_MEMORY_ARENA[0x8000];
}