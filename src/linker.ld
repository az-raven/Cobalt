OUTPUT_FORMAT(elf64-x86-64) /* In the case of a future arm64 port, this should be updated. */
OUTPUT_ARCH(i386:x86-64)

ENTRY(_start)

PHDRS
{
	text	PT_LOAD		FLAGS((1 << 0) | (1 << 2)) ; 	/* Execute & Read */
	rodata	PT_LOAD		FLAGS((1 << 2)) ;				/* Read */
	data 	PT_LOAD		FLAGS((1 << 1) | (1 << 2)) ;	/* Write & Read */
	dynamic	PT_DYNAMIC	FLAGS((1 << 1) | (1 << 2)) ;	/* Dynamic PHDR for relocations */
}

SECTIONS
{
	. = 0xffffffff80000000;
	
	.text : {
		*(.text .text.*)
	} :text

	. += CONSTANT(MAXPAGESIZE);

	.rodata : {
		*(.rodata .rodata.*)
	} :rodata

	. += CONSTANT(MAXPAGESIZE);

	.data : {
		*(.data .data.*)
	} :data

	.dynamic : {
		*(.dynamic)
	} :data :dynamic

	.bss : {
		*(.bss .bss.*)
		*(COMMON)
	} :data

	KERNEL_END = .;

	/DISCARD/ : {
		*(.eh_frame)
		*(.note .note.*)
	}
}