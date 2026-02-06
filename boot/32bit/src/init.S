.code16

.extern	Entry

# GDT for protected mode
gdt_start:
gdt_null:
	.long 0x0
	.long 0x0

gdt_code:
	.word 0xFFFF       # Limit (bits 0-15)
	.word 0x0          # Base (bits 0-15)
	.byte 0x0          # Base (bits 16-23)
	.byte 0b10011010   # Access byte
	.byte 0b11001111   # Flags + Limit (bits 16-19)
	.byte 0x0          # Base (bits 24-31)

gdt_data:
	.word 0xFFFF       # Limit (bits 0-15)
	.word 0x0          # Base (bits 0-15)
	.byte 0x0          # Base (bits 16-23)
	.byte 0b10010010   # Access byte
	.byte 0b11001111   # Flags + Limit (bits 16-19)
	.byte 0x0          # Base (bits 24-31)

gdt_end:

gdt_descriptor:
	.word gdt_end - gdt_start - 1
	.long gdt_start

.set CODE_SEG, gdt_code - gdt_start
.set DATA_SEG, gdt_data - gdt_start

.section .text
.global _start
_start:
	# Save drive number
	mov	%dx,	%ax
	push	%ax

	# Load GDT
	lgdt	gdt_descriptor

	# Enable protected mode
	mov	%cr0,	%eax
	or	$0x1,	%eax
	mov	%eax,	%cr0

	# Far jump to flush pipeline and enter 32-bit mode
	ljmp	$CODE_SEG, $protected_mode

.code32
protected_mode:
	# Setup segment registers
	mov	$DATA_SEG,	%ax
	mov	%ax,		%ds
	mov	%ax,		%es
	mov	%ax,		%fs
	mov	%ax,		%gs
	mov	%ax,		%ss

	# Setup stack
	mov	$0x90000,	%esp
	mov	%esp,		%ebp

	# Retrieve drive number
	pop	%eax

	# Call C entry point
	push	%eax
	call	Entry

	# Infinite loop
hang:
	cli
	hlt
	jmp	hang
