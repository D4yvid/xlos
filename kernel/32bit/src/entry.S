.section .text
.global _start
.extern kernel_main

_start:
    # Setup stack
    mov $0x200000, %esp
    mov %esp, %ebp

    # Clear direction flag
    cld

    # Call kernel main
    call kernel_main

    # If kernel_main returns (shouldn't happen), halt
halt:
    cli
    hlt
    jmp halt
