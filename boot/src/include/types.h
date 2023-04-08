#ifndef __TYPES_H__
#define __TYPES_H__

__asm__(".code16gcc");

typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned int	u32;
typedef unsigned long	u64;

typedef	char	i8;
typedef short	i16;
typedef int	i32;
typedef long	i64;

typedef float	f32;
typedef double	f64;

#define __NORETURN__ __attribute__((noreturn))
#define __ASMFN__ __attribute__((naked))

#define inb(port) ({							\
	u8 _r;							\
	__asm__ volatile ("inb %w1, %b0" : "=a" (_r) : "Nd" (port));	\
	_r;								\
})

#define outb(port, val) 						\
    __asm__ volatile ("outb %b0, %w1" : : "a" (val), "Nd" (port));

#define cli() __asm__("cli")
#define sti() __asm__("sti")

typedef enum
{
	FG_BLACK		= 0x00,
	FG_BLUE			= 0x01,
	FG_GREEN		= 0x02,
	FG_CYAN			= 0x03,
	FG_RED			= 0x04,
	FG_VIOLET		= 0x05,
	FG_BROWN		= 0x06,
	FG_WHITE		= 0x07,
	FG_GRAY			= 0x08,
	FG_BRIGHT_BLUE		= 0x09,
	FG_BRIGHT_GREEN 	= 0x0a,
	FG_BRIGHT_CYAN		= 0x0b,
	FG_BRIGHT_RED		= 0x0c,
	FG_BRIGHT_VIOLET	= 0x0d,
	FG_BRIGHT_YELLOW	= 0x0e,
	FG_BRIGHT_WHITE		= 0x0f,
	BG_BLACK		= 0x00,
	BG_BLUE			= 0x10,
	BG_GREEN		= 0x20,
	BG_CYAN			= 0x30,
	BG_RED			= 0x40,
	BG_VIOLET		= 0x50,
	BG_BROWN		= 0x60,
	BG_WHITE		= 0x70,
	BG_GRAY			= 0x80,
	BG_BRIGHT_BLUE		= 0x90,
	BG_BRIGHT_GREEN 	= 0xa0,
	BG_BRIGHT_CYAN		= 0xb0,
	BG_BRIGHT_RED		= 0xc0,
	BG_BRIGHT_VIOLET	= 0xd0,
	BG_BRIGHT_YELLOW	= 0xe0,
	BG_BRIGHT_WHITE		= 0xf0
} Color;

typedef enum
{
	T_EMPTY		= 0x0,
	T_FAT12		= 0x1,
	T_FAT16		= 0x4,
	T_EXTENDED	= 0x5,
	T_FAT16EX	= 0x6,
	T_NTFS		= 0x7,
	T_FAT32		= 0xB,
	T_FAT3213	= 0xC,
	T_FAT16EX13	= 0xE,
	T_EXTENDED13	= 0xF
} PartitionType;

typedef struct
{
	u8	bootIndicator;

	struct
	{
		u8	cylinder;
		u8	head;
		u8	sector;
	} startingCHSValues;

	u8 type;

	struct
	{
		u8	cylinder;
		u8	head;
		u8	sector;
	} endingCHSValues;

	u32	startingSector;
	u64	partitionSizeInSectors;
} PartitionEntry;

typedef struct
{
	PartitionEntry entries[4];
} PartitionTable;

#endif /** __TYPES_H__ */
