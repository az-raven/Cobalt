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

    call kernel_main

1:  hlt
    jmp 1b

/*
    Set the size of `_start` for debugging and stack traces.
*/
.size _start, . - _start

.section .init_only
.align 4096
page_directory_init:
    .skip
