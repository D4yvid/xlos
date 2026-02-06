#!/bin/bash
# Run 64-bit xLOS in QEMU
# This script loads the kernel directly at 1MB using QEMU's -kernel option

KERNEL="kernel/64bit/kernel64.elf"

if [ ! -f "$KERNEL" ]; then
    echo "Error: Kernel not found at $KERNEL"
    echo "Run ./build-test-image.sh first"
    exit 1
fi

echo "Starting 64-bit xLOS in QEMU..."
echo "Kernel: $KERNEL"
echo "Press Ctrl+A then X to exit QEMU"
echo

# Run QEMU with the kernel loaded at 1MB
# -kernel loads ELF kernels at their specified entry point
# Note: This requires CPU with 64-bit support
qemu-system-x86_64 \
    -kernel "$KERNEL" \
    -nographic \
    -m 128M \
    -no-reboot \
    -serial mon:stdio

# Alternative with VGA output (graphical window):
# qemu-system-x86_64 -kernel "$KERNEL" -m 128M
