.section .text
.global _start
.extern kernel_main

_start:
    # Setup 64-bit stack
    mov $0x200000, %rsp
    mov %rsp, %rbp

    # Clear direction flag
    cld

    # Call kernel main
    call kernel_main

    # If kernel_main returns (shouldn't happen), halt
halt:
    cli
    hlt
    jmp halt

# Mark stack as non-executable (fixes GNU-stack warning)
.section .note.GNU-stack,"",@progbits
