#define TODO() \
	do { asm("cli; hlt;"); } while (true)
