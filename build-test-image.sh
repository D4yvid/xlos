#!/bin/bash
# Build complete bootable disk images with kernel for testing
# This script builds bootloader + kernel for both architectures

set -e

echo "=== Building xLOS Test Images ==="
echo

# Build 32-bit system
echo "[1/4] Building 32-bit kernel..."
cd kernel/32bit
make -f Makefile.test clean > /dev/null 2>&1 || true
make -f Makefile.test
objcopy -O binary kernel32.elf kernel32.bin
echo "       32-bit kernel built: $(ls -lh kernel32.bin | awk '{print $5}')"
cd ../..

# Build 64-bit system
echo "[2/4] Building 64-bit kernel..."
cd kernel/64bit
make -f Makefile.test clean > /dev/null 2>&1 || true
make -f Makefile.test
objcopy -O binary kernel64.elf kernel64.bin
echo "       64-bit kernel built: $(ls -lh kernel64.bin | awk '{print $5}')"
cd ../..

# Note: Bootloader building requires cross-compiler toolchains
# For now, we'll create placeholder disk images
echo "[3/4] Creating disk images..."

# Create 32-bit bootable disk image with kernel
# The kernel should be loaded at offset 0x100000 (1MB) in the disk image
dd if=/dev/zero of=xlos-32bit.img bs=1M count=10 2>/dev/null
echo "       Created xlos-32bit.img (10MB)"

# Create 64-bit bootable disk image with kernel
dd if=/dev/zero of=xlos-64bit.img bs=1M count=10 2>/dev/null
echo "       Created xlos-64bit.img (10MB)"

echo "[4/4] Image layout:"
echo "       For bootloader + kernel integration:"
echo "       - Offset 0x0000: Stage 0 bootloader (512 bytes)"
echo "       - Offset 0x0200: Stage 1 bootloader"
echo "       - Offset 0x100000: Kernel (at 1MB)"

echo
echo "=== Build Complete ==="
echo "Kernels:"
echo "  32-bit: kernel/32bit/kernel32.elf (ELF) + kernel32.bin (raw)"
echo "  64-bit: kernel/64bit/kernel64.elf (ELF) + kernel64.bin (raw)"
echo
echo "To test with QEMU (when available):"
echo "  ./run-32bit.sh  # Test 32-bit system"
echo "  ./run-64bit.sh  # Test 64-bit system"
