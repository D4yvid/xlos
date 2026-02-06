.code16

.extern	Entry

# GDT for protected/long mode
gdt_start:
gdt_null:
	.long 0x0
	.long 0x0

gdt_code:
	.word 0xFFFF       # Limit (bits 0-15)
	.word 0x0          # Base (bits 0-15)
	.byte 0x0          # Base (bits 16-23)
	.byte 0b10011010   # Access byte
	.byte 0b10101111   # Flags + Limit (bits 16-19) - long mode
	.byte 0x0          # Base (bits 24-31)

gdt_data:
	.word 0xFFFF       # Limit (bits 0-15)
	.word 0x0          # Base (bits 0-15)
	.byte 0x0          # Base (bits 16-23)
	.byte 0b10010010   # Access byte
	.byte 0b10101111   # Flags + Limit (bits 16-19)
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

	# Far jump to enter 32-bit protected mode
	ljmp	$CODE_SEG, $protected_mode

.code32
protected_mode:
	# Setup segment registers for 32-bit
	mov	$DATA_SEG,	%ax
	mov	%ax,		%ds
	mov	%ax,		%es
	mov	%ax,		%fs
	mov	%ax,		%gs
	mov	%ax,		%ss

	# Setup stack
	mov	$0x90000,	%esp

	# Build page tables for identity mapping
	# PML4 at 0x1000
	mov	$0x1000,	%edi
	mov	%edi,		%cr3
	xor	%eax,		%eax
	mov	$4096,		%ecx
	rep	stosl
	mov	%cr3,		%edi

	# PML4[0] -> PDPT
	mov	$0x2003,	(%edi)
	add	$0x1000,	%edi
	# PDPT[0] -> PDT
	mov	$0x3003,	(%edi)
	add	$0x1000,	%edi
	# PDT[0] -> PT
	mov	$0x4003,	(%edi)
	add	$0x1000,	%edi

	# Build identity mapping for first 2MB
	mov	$0x00000003,	%ebx
	mov	$512,		%ecx

set_entry:
	mov	%ebx,		(%edi)
	add	$0x1000,	%ebx
	add	$8,		%edi
	loop	set_entry

	# Enable PAE
	mov	%cr4,		%eax
	or	$0x20,		%eax
	mov	%eax,		%cr4

	# Enable long mode
	mov	$0xC0000080,	%ecx
	rdmsr
	or	$0x100,		%eax
	wrmsr

	# Enable paging
	mov	%cr0,		%eax
	or	$0x80000000,	%eax
	mov	%eax,		%cr0

	# Load 64-bit GDT
	lgdt	gdt_descriptor

	# Far jump to 64-bit mode
	ljmp	$CODE_SEG, $long_mode

.code64
long_mode:
	# Setup segment registers for 64-bit
	mov	$DATA_SEG,	%ax
	mov	%ax,		%ds
	mov	%ax,		%es
	mov	%ax,		%fs
	mov	%ax,		%gs
	mov	%ax,		%ss

	# Setup 64-bit stack
	mov	$0x90000,	%rsp
	mov	%rsp,		%rbp

	# Retrieve drive number (zero-extend)
	pop	%rdi
	and	$0xFFFF,	%rdi

	# Call C entry point
	call	Entry

	# Infinite loop
hang:
	cli
	hlt
	jmp	hang
