# xLOS

A hobby Operating System for x86 platform with dual-architecture bootloader support.

## Architecture

xLOS supports both 32-bit and 64-bit bootloaders:

### 32-bit Bootloader (boot/32bit/)
- Starts in 16-bit real mode
- Transitions to 32-bit protected mode
- Sets up GDT (Global Descriptor Table)
- Enables A20 gate for extended memory access
- Identity maps memory for kernel loading

### 64-bit Bootloader (boot/64bit/)
- Starts in 16-bit real mode
- Transitions through 32-bit protected mode
- Sets up paging with 4-level page tables (PML4)
- Enables PAE (Physical Address Extension)
- Enters 64-bit long mode
- Provides full 64-bit addressing capability

## Build System

### Prerequisites

You need to install cross-compiler toolchains:

**For 32-bit bootloader:**
- \`i686-elf-gcc\`
- \`i686-elf-ld\`
- \`i686-elf-as\`

**For 64-bit bootloader:**
- \`x86_64-elf-gcc\`
- \`x86_64-elf-ld\`
- \`x86_64-elf-as\`

### Building

Build both bootloaders:
\`\`\`bash
make boot
\`\`\`

Build specific architecture:
\`\`\`bash
cd boot
make 32bit  # Build 32-bit bootloader only
make 64bit  # Build 64-bit bootloader only
\`\`\`

### Output Files

**32-bit bootloader:**
- \`boot/32bit/stage0.bin\` - First-stage bootloader (512 bytes)
- \`boot/32bit/bootloader32.raw\` - Second-stage bootloader
- \`boot/32bit/bootsect32.img\` - Complete disk image (4MB)

**64-bit bootloader:**
- \`boot/64bit/stage0.bin\` - First-stage bootloader (512 bytes)
- \`boot/64bit/bootloader64.raw\` - Second-stage bootloader
- \`boot/64bit/bootsect64.img\` - Complete disk image (4MB)

## Project Structure

\`\`\`
xLOS/
├── boot/
│   ├── 32bit/              # 32-bit protected mode bootloader
│   │   ├── Makefile
│   │   ├── stage0.S        # First-stage (16-bit real mode)
│   │   ├── link.ld         # Linker script
│   │   └── src/
│   │       ├── init.S      # Protected mode transition
│   │       ├── init.c      # Main bootloader logic
│   │       └── include/
│   │           └── types.h # Type definitions
│   ├── 64bit/              # 64-bit long mode bootloader
│   │   ├── Makefile
│   │   ├── stage0.S        # First-stage (16-bit real mode)
│   │   ├── link.ld         # Linker script
│   │   └── src/
│   │       ├── init.S      # Long mode transition with paging
│   │       ├── init.c      # Main bootloader logic
│   │       └── include/
│   │           └── types.h # Type definitions
│   └── Makefile            # Top-level boot Makefile
├── kernel/                 # Kernel directory (future)
├── config.mk               # Architecture documentation
├── Makefile                # Root Makefile
└── README.md              # This file
\`\`\`

## Boot Process

### 32-bit Boot Sequence
1. BIOS loads stage0 (MBR) at 0x7C00
2. stage0 loads second-stage bootloader at 0x7E00
3. init.S sets up GDT and enables protected mode
4. init.c performs system initialization:
   - Enables A20 gate
   - Validates partition table
   - Displays system information

### 64-bit Boot Sequence
1. BIOS loads stage0 (MBR) at 0x7C00
2. stage0 loads second-stage bootloader at 0x7E00
3. init.S performs mode transitions:
   - Sets up GDT
   - Enters 32-bit protected mode
   - Sets up 4-level paging (PML4 → PDPT → PDT → PT)
   - Enables PAE (CR4.PAE = 1)
   - Enables long mode (EFER.LME = 1)
   - Enables paging (CR0.PG = 1)
   - Jumps to 64-bit code
4. init.c performs system initialization in 64-bit mode

## Features

- **Dual Architecture Support**: Both 32-bit and 64-bit bootloaders
- **Clean Separation**: Architecture-specific code is properly isolated
- **Modern Standards**: Uses proper protected/long mode transitions
- **FAT16 Support**: Validates FAT16 partition tables
- **A20 Gate Handling**: Multiple fallback methods for compatibility
- **Direct VGA Access**: Text mode framebuffer at 0xB8000

## Technical Details

### Memory Layout
- **0x7C00-0x7DFF**: Stage 0 bootloader (512 bytes)
- **0x7E00-0x7FFF**: Stage 1 bootloader (second-stage)
- **0x7DBE-0x7FFF**: Partition table
- **0x90000**: Stack pointer (both architectures)
- **0x1000-0x5000**: Page tables (64-bit only)

### 32-bit Bootloader Specifics
- **GDT**: 3 entries (null, code, data)
- **Code Segment**: Base 0x0, Limit 0xFFFFF, 32-bit, Read/Execute
- **Data Segment**: Base 0x0, Limit 0xFFFFF, 32-bit, Read/Write
- **A20 Gate**: Keyboard controller method with fallback
- **Memory Access**: Full 4GB addressing

### 64-bit Bootloader Specifics
- **GDT**: 3 entries (null, code, data) with long mode flag
- **Page Tables**: 4-level paging (512GB virtual address space)
  - PML4 at 0x1000
  - PDPT at 0x2000
  - PDT at 0x3000
  - PT at 0x4000
- **Identity Mapping**: First 2MB (0x0-0x200000)
- **Memory Access**: Full 64-bit addressing capability

## License

This is a hobby project for educational purposes.
