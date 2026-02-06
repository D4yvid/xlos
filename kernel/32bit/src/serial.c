#include "include/types.h"
#include "include/serial.h"

#define SERIAL_PORT 0x3F8

static inline u8 serial_inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %w1, %b0" : "=a" (ret) : "Nd" (port));
    return ret;
}

static inline void serial_outb(u16 port, u8 val) {
    __asm__ volatile ("outb %b0, %w1" : : "a" (val), "Nd" (port));
}

void serial_init(void) {
    serial_outb(SERIAL_PORT + 1, 0x00);    // Disable interrupts
    serial_outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB
    serial_outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (38400 baud)
    serial_outb(SERIAL_PORT + 1, 0x00);
    serial_outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
    serial_outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO
    serial_outb(SERIAL_PORT + 4, 0x0B);    // Enable IRQs, RTS/DSR set
}

static u8 serial_ready(void) {
    return serial_inb(SERIAL_PORT + 5) & 0x20;
}

void serial_putchar(char c) {
    while (!serial_ready());
    serial_outb(SERIAL_PORT, c);
}

void serial_write(const char *str) {
    while (*str) {
        serial_putchar(*str++);
    }
}
