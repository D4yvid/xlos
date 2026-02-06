#!/bin/bash
# Run 32-bit xLOS in QEMU
# This script loads the kernel directly at 1MB using QEMU's -kernel option

KERNEL="kernel/32bit/kernel32.elf"

if [ ! -f "$KERNEL" ]; then
    echo "Error: Kernel not found at $KERNEL"
    echo "Run ./build-test-image.sh first"
    exit 1
fi

echo "Starting 32-bit xLOS in QEMU..."
echo "Kernel: $KERNEL"
echo "Press Ctrl+A then X to exit QEMU"
echo

# Run QEMU with the kernel loaded at 1MB
# -kernel loads ELF kernels at their specified entry point
qemu-system-i386 \
    -kernel "$KERNEL" \
    -nographic \
    -m 128M \
    -no-reboot \
    -serial mon:stdio

# Alternative with VGA output (graphical window):
# qemu-system-i386 -kernel "$KERNEL" -m 128M
