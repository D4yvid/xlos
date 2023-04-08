#include "include/16.h"
#include "include/types.h"

#define FRAMEBUFFER ((i8 *)0xB8000)
#define FBWIDTH 80
#define FBHEIGHT 24

#define INFO_LABEL	"[INFO] "
#define WARN_LABEL	"[WARN] "
#define FAIL_LABEL	"[FAIL] "

#define INFO(msg) Print(INFO_LABEL msg)
#define WARN(msg) Print(WARN_LABEL msg)
#define FAIL(msg) Print(FAIL_LABEL msg)
#define INFOLN(msg) Print(INFO_LABEL msg "\r\n")
#define WARNLN(msg) Print(WARN_LABEL msg "\r\n")
#define FAILLN(msg) Print(FAIL_LABEL msg "\r\n")

__NORETURN__ void Panic(const char *msg);

void SetupVideo();
void FillBackground(Color color);
void PutChar(i8 chr);
void EnableA20();

void Print(const i8 *string);
void PrintNumber(u32 number);
void PrintBinary(i8 number);
void PrintAt(const i8 *string, i32 line, i32 column);

void PutChar(i8 chr)
{
	__asm__("int	$0x10" : : "a"(0x0e00 | chr));
}

void PrintAt(const i8 *string, i32 line, i32 column)
{
	i32 i, y = line, x;

        for (i = 0; *(string + i); i++) {
                x = column + i;

                FRAMEBUFFER[((y * FBWIDTH) + x) * 2] = (0x00 << 8) | *(string + i);
        }
}

void Print(const i8 *string)
{
	while (*string)
	{
		PutChar(*string++);
	}
}

void PrintNumber(u32 number)
{
	if (number <= 0)
	{
		PutChar('0');
		return;
	}

	char digits[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int  currentDigit = 11;
	int  numberSize = 0;

	while (number > 0)
	{
		i8 l = (number % 10) + 0x30;
		digits[--currentDigit] = l;
		number /= 10;
		numberSize += 1;
	}

	digits[11] = 0x00;

	Print(&digits[11 - numberSize]);
}

void PrintBinary(i8 number)
{
	for (int i = 8; i >= 0; i--)
	{
		i8 l = (number >> i) & 0x1 ? '1' : '0';
		PutChar(l);
	}
}

void SetupVideo()
{
	__asm__("mov	$0x03,	%al");
	__asm__("mov	$0x00,	%ah");
	__asm__("int	$0x10");
}

u8 CheckA20()
{
	u16 *signature = (u16 *) 0x7DFE;

	if (*signature != *((u16 *) 0xFFFF7E0E)) {
		return 1;
	}

	return 0;
}

void A20Wait1()
{
	int x = inb(0x64);

	if (x != 2)
		__asm__("jmp	A20Wait1");
}

void A20Wait2()
{
	int x = inb(0x64);

	if (x != 1)
		__asm__("jmp	A20Wait2");
}

void EnableA20()
{
	/** Check if it is not already enabled */
	if (CheckA20()) {
		INFOLN("A20 Gate already enabled");
		return;
	}

	INFOLN("A20 Gate not enabled, trying to enable...");

	u16 res;

	__asm__("int	$0x15" : "=a"(res) : "a"(0x2403));

	if (CheckA20()) {
		INFOLN("A20 Gate enabled with interrupt 0x15");
		return;
	}

	u32 ret;

	cli();

	A20Wait1();
	outb(0x64, 0xAD);

	A20Wait1();
	outb(0x64, 0xD0);

	A20Wait2();
	ret = inb(0x64);

	A20Wait1();
	outb(0x64, 0xD1);

	A20Wait1();
	outb(0x64, ret | 0x2);

	A20Wait1();
	outb(0x64, 0xAE);

	A20Wait1();
	sti();

	for (int i = 0; i < 0xFF; i++)
	{
		if (CheckA20()) {
			INFOLN("A20 Gate enabled by keyboard-controller");
			return;
		}
	}
	
	int i = inb(0x92);

	if (i != 2) {
		outb(0x92, (i | 0x02) & 0xFE);

		for (int i = 0; i < 0xFF; i++)
		{
			if (CheckA20()) {
				INFOLN("A20 Gate enabled by keyboard-controller");
				return;
			}
		}
	}

	Panic("Your system does not support A20 Gate, that is necessary to run the system!");
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

__NORETURN__ void Entry()
{
	SetupVideo();
	EnableA20();

	PartitionTable *table = (PartitionTable *) 0x7DBE;

	if (table->entries[0].type == T_EMPTY) {
		Panic("The first partition in the disk does not exist!");
	}

	if (table->entries[0].type != T_FAT16EX) {
		Panic("The first partition need to be a FAT16 Filesystem!");
	}

	INFOLN("Partition 1 in /DEVICE/DISK0:");
	INFOLN("  Type: FAT16");
	INFO("  Size: ");
	PrintNumber(table->entries[0].partitionSizeInSectors * 512);
	Print(" bytes\r\n");

	for (;;);
}
