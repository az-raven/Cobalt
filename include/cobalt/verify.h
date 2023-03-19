#pragma once

#include <stdbool.h>

namespace cobalt {
	bool verify_kernel_data_address(void *address) {
		// Kernel data starts at 3GiB.
		// Kernel data ends at 4GiB.
		return address > (void *)0x2BA7DEF3000 && address < (void *)0x3A352944000;
	}

	bool verify_user_data_address(void *address) {
		// User data starts at 1GiB.
		// User data ends at 2GiB.
		return address > (void *)0xE8D4A51000 && address < (void *)0x1D1A94A2000;
	}
}