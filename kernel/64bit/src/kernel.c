#include "include/types.h"
#include "include/vga.h"

__NORETURN__ void kernel_main(void) {
    vga_init();
    vga_clear();

    vga_write_color("xLOS Kernel (64-bit)\n", VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_write("====================\n\n");

    vga_write("Architecture: x86_64 (64-bit long mode)\n");
    vga_write("Status: Running\n");
    vga_write("Features:\n");
    vga_write("  - Long mode enabled\n");
    vga_write("  - 64-bit GDT configured\n");
    vga_write("  - Paging enabled (4-level)\n");
    vga_write("  - Full 64-bit address space\n\n");

    vga_write_color("Kernel initialized successfully!\n", VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);

    // Halt
    while (1) {
        __asm__ volatile("hlt");
    }
}
