.code16

jmp	Start

DiskPacket:
	.byte	0x0A
	.byte	0x00
	.word	0x000F
	.word	0x7E00
	.word	0x0000
	.int	0x00000001
	.int	0x00000000

DriveNumber:
	.word	0x0000

Start:
	# Setup all segments
	movw	%cs,	%ax
	movw	%ax,	%es
	movw	%ax,	%ss
	movw	%ax,	%ds
	movw	%ax,	%fs
	movw	%ax,	%gs

	# Store the drive number to use later
	movw	%dx,	DriveNumber

	# Reset the disk
	mov	$0x0000,	%ax
	int	$0x13
	jc	Exit

	# Read the code in memory
	mov	$0x42,		%ah
	movw	DriveNumber,	%dx
	mov	$DiskPacket,	%si
	int	$0x13
	jc	Exit

	mov	DriveNumber,	%dx
	jmp	0x7E00

Exit:
	cli
	hlt

. = 0x1fe
.byte 0x55
.byte 0xaa
