#pragma once

#define TODO(what)                                                     \
	do {                                                               \
			asm("cli; hlt;");								   		   \
	} while (true)
