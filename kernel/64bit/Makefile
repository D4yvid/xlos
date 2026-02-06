CC = x86_64-elf-gcc
LD = x86_64-elf-ld
AS = x86_64-elf-as

CSRC = $(wildcard src/*.c)
ASRC = $(wildcard src/*.S)
OBJ = $(ASRC:.S=.o) $(CSRC:.c=.o)

KERNEL = kernel64.elf

CFLAGS = -Wall -Wextra -Werror -ffreestanding -fno-stack-protector -nostdlib \
         -fno-builtin -m64 -mcmodel=large -mno-red-zone -I src
LDFLAGS = -T linker.ld -nostdlib -m elf_x86_64
ASFLAGS = --64

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
