/* Called by GRUB */
.code32
.section .text
.global _start
.type _start, @function
_start:
    /* 
        welcome to the rice fields of kernel space
    */
    mov $stack_top, %esp

    // to long mode we shall go
    mov $0xC0000080, %ecx
    rdmsr
    or $1<<8, %eax
    wrmsr

    ljmpl $0x8, $_x86_64_long_mode

.code64
_x86_64_long_mode:
    // welcome to long mode
    call kernel_main

/*
    Set the size of `_start` for debugging and stack traces.
*/
.size _start, . - _start

.section .init_only
.align 4096
page_directory_init:
    .skip 