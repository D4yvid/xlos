#include "include/types.h"

#define FRAMEBUFFER ((i8 *)0xB8000)
#define FBWIDTH 80
#define FBHEIGHT 24

#define INFO_LABEL	"[INFO] "
#define WARN_LABEL	"[WARN] "
#define FAIL_LABEL	"[FAIL] "

static i32 cursor_y = 0;

#define INFO(msg) PrintAt(INFO_LABEL msg, cursor_y, 0); cursor_y++
#define WARN(msg) PrintAt(WARN_LABEL msg, cursor_y, 0); cursor_y++
#define FAIL(msg) PrintAt(FAIL_LABEL msg, cursor_y, 0); cursor_y++

__NORETURN__ void Panic(const char *msg);

void EnableA20();
void PrintNumber(u32 number);
void PrintAt(const i8 *string, i32 line, i32 column);

void PrintAt(const i8 *string, i32 line, i32 column)
{
	i32 i, y = line, x;

	for (i = 0; *(string + i); i++) {
		x = column + i;

		if (x >= FBWIDTH) {
			x = 0;
			y++;
		}

		if (y >= FBHEIGHT) {
			break;
		}

		FRAMEBUFFER[((y * FBWIDTH) + x) * 2] = *(string + i);
		FRAMEBUFFER[((y * FBWIDTH) + x) * 2 + 1] = 0x07; // Light gray on black
	}
}

void PrintNumber(u32 number)
{
	if (number == 0) {
		char zero[2] = {'0', 0};
		PrintAt(zero, cursor_y, 20);
		return;
	}

	char digits[12] = { 0 };
	int  currentDigit = 11;
	int  numberSize = 0;

	while (number > 0) {
		i8 l = (number % 10) + 0x30;
		digits[--currentDigit] = l;
		number /= 10;
		numberSize += 1;
	}

	digits[11] = 0x00;

	PrintAt(&digits[11 - numberSize], cursor_y, 20);
}

u8 CheckA20()
{
	volatile u32 *low = (u32*)0x000000;
	volatile u32 *high = (u32*)0x100000;

	u32 original = *low;
	*low = 0x12345678;
	*high = 0x87654321;

	u8 result = (*low != *high);
	*low = original;

	return result;
}

void EnableA20()
{
	// Check if A20 is already enabled
	if (CheckA20()) {
		INFO("A20 Gate already enabled (32-bit protected mode)");
		return;
	}

	INFO("Enabling A20 Gate via keyboard controller...");

	// Disable keyboard
	while (inb(0x64) & 2);
	outb(0x64, 0xAD);

	// Read from input
	while (inb(0x64) & 2);
	outb(0x64, 0xD0);
	while (!(inb(0x64) & 1));
	u8 status = inb(0x60);

	// Write to output
	while (inb(0x64) & 2);
	outb(0x64, 0xD1);
	while (inb(0x64) & 2);
	outb(0x60, status | 2);

	// Enable keyboard
	while (inb(0x64) & 2);
	outb(0x64, 0xAE);
	while (inb(0x64) & 2);

	if (CheckA20()) {
		INFO("A20 Gate enabled successfully");
	} else {
		Panic("Failed to enable A20 Gate");
	}
}

__NORETURN__ void Panic(const char *msg)
{
	for (int i = 0; i < FBWIDTH; i++)
	{
		for (int j = 0; j < FBHEIGHT + 1; j++)
		{
			u8 color = BG_RED | FG_BRIGHT_WHITE;

			if (j == 3 && (i >= 33 && i <= 45))
			{
				color = BG_BRIGHT_WHITE | FG_RED;
			}

			((i16 *) (FRAMEBUFFER))[((j * FBWIDTH) + i)] = (i16) ((color << 8)) | ' ';
		}
	}

	PrintAt("FATAL ERROR", 3, 34);
	PrintAt("A fatal error occurred and the system will need to reboot!", 5, 10);
	PrintAt("Error: ", 6, 10);
	PrintAt(msg, 6, 17);
	PrintAt("Press CTRL + ALT + DEL to reboot.", 20, 10);

	for (;;);
}

__NORETURN__ void Entry(u32 drive)
{
	// Clear screen
	for (int i = 0; i < FBWIDTH * FBHEIGHT * 2; i++) {
		FRAMEBUFFER[i] = 0;
	}

	INFO("xLOS 32-bit Bootloader");
	INFO("Running in 32-bit protected mode");

	EnableA20();

	PartitionTable *table = (PartitionTable *) 0x7DBE;

	if (table->entries[0].type == T_EMPTY) {
		Panic("The first partition in the disk does not exist!");
	}

	if (table->entries[0].type != T_FAT16EX) {
		Panic("The first partition need to be a FAT16 Filesystem!");
	}

	INFO("Partition 1 in /DEVICE/DISK0:");
	PrintAt("  Type: FAT16", cursor_y, 0);
	cursor_y++;
	PrintAt("  Size: ", cursor_y, 0);
	PrintNumber(table->entries[0].partitionSizeInSectors * 512);
	PrintAt(" bytes", cursor_y, 30);
	cursor_y++;

	for (;;);
}
