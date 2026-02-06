CC = i686-elf-gcc
LD = i686-elf-ld
AS = i686-elf-as

CSRC = $(wildcard src/*.c)
ASRC = $(wildcard src/*.S)
OBJ = $(ASRC:.S=.o) $(CSRC:.c=.o)

KERNEL = kernel32.elf

CFLAGS = -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -nostdlib \
         -fno-builtin -m32 -I src
LDFLAGS = -T linker.ld -nostdlib -m elf_i386
ASFLAGS = --32

%.o: %.S
	$(AS) $(ASFLAGS) -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(KERNEL)

$(KERNEL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(KERNEL) $(OBJ)

.PHONY: all clean
