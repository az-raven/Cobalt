// TODO: aarch64/x86_32 support
.set ARCH_X86_64, 1

/* Constants for the multiboot header. */
.set ALIGN,     1<<0                    /* align loaded modules on page boundaries */
.set MEMINFO,   1<<1                    /* provide memory map */
.set FLAGS,     4                       /* Multiboot 'flag' field */
.set MAGIC,     0x1BADB002              /* Multiboot magic number */
.set CHECKSUM,  -(MAGIC + FLAGS)        /* Checksum for multiboot */

/* Begin multiboot header */
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.skip 24

/* Graphics Fields */
// Linear Graphics Mode
.long 0
// Width
.long 640
// Height
.long 480
// Depth
.long 16

/* Kernel stack */
.section .bss
.align 16
stack_bottom:
.skip 16384     # 16 KiB
stack_top:

.ifdef ARCH_X86_64
.include "src/arch/i686/boot.s"
.endif
