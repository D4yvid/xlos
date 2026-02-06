# xLOS Testing Guide

This guide explains how to build and test the xLOS kernel using QEMU.

## Prerequisites

### Required Tools
- `gcc` - GNU C Compiler (for building kernels)
- `ld` - GNU Linker
- `as` - GNU Assembler
- `objcopy` - Object file converter
- `make` - Build automation tool
- `qemu-system-i386` - QEMU for 32-bit x86 (for testing)
- `qemu-system-x86_64` - QEMU for 64-bit x86-64 (for testing)

### Installing QEMU

On Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install qemu-system-x86
```

On Fedora/RHEL:
```bash
sudo dnf install qemu-system-x86
```

On macOS (with Homebrew):
```bash
brew install qemu
```

## Building the Kernels

### Quick Build (Using System GCC)

The easiest way to build and test:

```bash
./build-test-image.sh
```

This script will:
1. Build the 32-bit kernel using system GCC
2. Build the 64-bit kernel using system GCC
3. Convert ELF binaries to raw binary format
4. Create placeholder disk images

### Manual Build

**32-bit kernel:**
```bash
cd kernel/32bit
make -f Makefile.test
objcopy -O binary kernel32.elf kernel32.bin
```

**64-bit kernel:**
```bash
cd kernel/64bit
make -f Makefile.test
objcopy -O binary kernel64.elf kernel64.bin
```

### Build with Cross-Compiler Toolchains (Recommended for Production)

If you have the proper cross-compiler toolchains installed:

**32-bit kernel:**
```bash
cd kernel/32bit
make  # Uses i686-elf-gcc
```

**64-bit kernel:**
```bash
cd kernel/64bit
make  # Uses x86_64-elf-gcc
```

## Testing with QEMU

### Testing 32-bit Kernel

Run the 32-bit kernel in QEMU:

```bash
./run-32bit.sh
```

**What you should see:**
```
xLOS Kernel (32-bit)
====================

Architecture: i686 (32-bit protected mode)
Status: Running
Features:
  - Protected mode enabled
  - GDT configured
  - 4GB address space

Kernel initialized successfully!
```

**Exit QEMU:** Press `Ctrl+A` then `X`

### Testing 64-bit Kernel

Run the 64-bit kernel in QEMU:

```bash
./run-64bit.sh
```

**What you should see:**
```
xLOS Kernel (64-bit)
====================

Architecture: x86_64 (64-bit long mode)
Status: Running
Features:
  - Long mode enabled
  - 64-bit GDT configured
  - Paging enabled (4-level)
  - Full 64-bit address space

Kernel initialized successfully!
```

**Exit QEMU:** Press `Ctrl+A` then `X`

### Alternative Testing Methods

**With VGA output (graphical window):**

32-bit:
```bash
qemu-system-i386 -kernel kernel/32bit/kernel32.elf -m 128M
```

64-bit:
```bash
qemu-system-x86_64 -kernel kernel/64bit/kernel64.elf -m 128M
```

**With debugging enabled:**

32-bit:
```bash
qemu-system-i386 -kernel kernel/32bit/kernel32.elf -m 128M -d int,cpu_reset -no-reboot
```

64-bit:
```bash
qemu-system-x86_64 -kernel kernel/64bit/kernel64.elf -m 128M -d int,cpu_reset -no-reboot
```

## Troubleshooting

### QEMU Not Found

If you get "command not found" errors:
```bash
sudo apt-get install qemu-system-x86
```

### Kernel Build Errors

**32-bit compilation errors:**
Make sure you have 32-bit library support:
```bash
sudo apt-get install gcc-multilib
```

**Missing tools:**
```bash
sudo apt-get install build-essential binutils
```

### QEMU Hangs or Doesn't Display Output

Try using the `-nographic` option:
```bash
qemu-system-i386 -kernel kernel/32bit/kernel32.elf -nographic -m 128M
```

Or enable serial output:
```bash
qemu-system-i386 -kernel kernel/32bit/kernel32.elf -serial mon:stdio -m 128M
```

## Understanding the Output

### Kernel Boot Sequence

1. **QEMU loads kernel**: QEMU's `-kernel` option loads the ELF file and jumps to the entry point (0x100000)
2. **Entry assembly**: Sets up stack at 0x200000
3. **Kernel main**: C code initializes VGA and displays information
4. **Halt loop**: Kernel enters infinite HLT loop

### Memory Layout in QEMU

```
0x00000000 - 0x000FFFFF: Lower memory (1MB)
0x00100000: Kernel entry point
0x00200000: Kernel stack
```

### VGA Text Mode

The kernel uses VGA text mode (0xB8000) to display:
- 80x25 character display
- 16 colors (foreground/background)
- Direct framebuffer access

## Advanced Testing

### Testing with GDB

Start QEMU with GDB server:
```bash
qemu-system-i386 -kernel kernel/32bit/kernel32.elf -s -S
```

In another terminal:
```bash
gdb kernel/32bit/kernel32.elf
(gdb) target remote :1234
(gdb) break kernel_main
(gdb) continue
```

### Creating Full Bootable Images

To test with the bootloader (requires cross-compiler toolchains):

1. Build bootloader:
```bash
cd boot/32bit
make all
```

2. The bootable image will be at `boot/32bit/bootsect32.img`

3. Test with QEMU:
```bash
qemu-system-i386 -drive file=boot/32bit/bootsect32.img,format=raw -m 128M
```

## Known Issues

1. **GNU-stack warnings**: Harmless warnings about executable stack (will be fixed in future commits)
2. **Cross-compiler requirement**: For bootloader testing, cross-compiler toolchains are required
3. **BIOS mode only**: Currently only supports BIOS boot (no UEFI)

## Next Steps

After successful kernel testing:
1. Add more kernel features (interrupts, memory management)
2. Implement proper kernel loading in bootloader
3. Add filesystem support
4. Implement multitasking

## Resources

- [QEMU Documentation](https://www.qemu.org/docs/master/)
- [OSDev Wiki](https://wiki.osdev.org/)
- [Intel x86 Manuals](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-sdm.html)
