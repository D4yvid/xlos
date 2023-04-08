.code16

.extern	Entry

.section .text
.global _start
_start:
	mov	%dx,	%ax
	jmp	Entry

	jmp	.
