include ../config.mk

CSRC = $(wildcard src/*.c)
ASRC = $(wildcard src/*.S)
OBJ  = $(ASRC:.S=.So) $(CSRC:.c=.o)

BOOTCODE = bootloader.raw

CCFLAGS = -Wall -Werror -Wno-pedantic -I include -ffreestanding -fno-stack-protector -nostdlib -fno-builtin \
	  -fno-builtin-types
LDFLAGS = -Tlink.ld -nostdlib

TARGET = bootsect.img

%.So: %.S
	$(AS) -o $@ $<

%.o: %.c
	$(CC) -c -o $@ $< $(CCFLAGS)

bootloader: $(OBJ)
	$(LD) $(LDFLAGS) -o $(BOOTCODE) $(OBJ)

stage0:
	@$(AS) -o stage0.tmp stage0.S
	@$(LD) -Ttext=0x7C00 --oformat=binary -o stage0.bin stage0.tmp
	@rm stage0.tmp

build:
	dd if=/dev/zero of=$(TARGET) bs=1024 count=4096

	dd if=stage0.bin of=$(TARGET) conv=notrunc bs=512 count=1
	dd if=$(BOOTCODE) of=$(TARGET) conv=notrunc bs=1 seek=512

	echo -e "n\np\n1\n1\n\nt\n06\nw\nq" | fdisk bootsect.img

clean:
	rm -rf $(BOOTCODE) stage0.bin $(TARGET) $(OBJ)

all: stage0 bootloader build

.PHONY: all build clean stage0 bootloader
