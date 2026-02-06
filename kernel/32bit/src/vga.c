#include "include/vga.h"

static u16 *vga_buffer = (u16 *)VGA_MEMORY;
static u32 vga_index = 0;
static u8 vga_current_color = 0x07; // Light grey on black

static u8 make_color(u8 fg, u8 bg) {
    return fg | (bg << 4);
}

static u16 make_vga_entry(char c, u8 color) {
    return (u16)c | ((u16)color << 8);
}

void vga_init(void) {
    vga_index = 0;
    vga_current_color = make_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
}

void vga_clear(void) {
    for (u32 i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = make_vga_entry(' ', vga_current_color);
    }
    vga_index = 0;
}

void vga_set_color(u8 fg, u8 bg) {
    vga_current_color = make_color(fg, bg);
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_index = (vga_index / VGA_WIDTH + 1) * VGA_WIDTH;
    } else if (c == '\r') {
        vga_index = (vga_index / VGA_WIDTH) * VGA_WIDTH;
    } else if (c == '\t') {
        vga_index = (vga_index + 4) & ~3;
    } else {
        vga_buffer[vga_index] = make_vga_entry(c, vga_current_color);
        vga_index++;
    }

    if (vga_index >= VGA_WIDTH * VGA_HEIGHT) {
        // Scroll up
        for (u32 i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
            vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
        }
        for (u32 i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buffer[i] = make_vga_entry(' ', vga_current_color);
        }
        vga_index = VGA_WIDTH * (VGA_HEIGHT - 1);
    }
}

void vga_write(const char *str) {
    for (u32 i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

void vga_write_color(const char *str, u8 fg, u8 bg) {
    u8 old_color = vga_current_color;
    vga_set_color(fg, bg);
    vga_write(str);
    vga_current_color = old_color;
}
