#ifndef __SERIAL_H__
#define __SERIAL_H__

void serial_init(void);
void serial_putchar(char c);
void serial_write(const char *str);

#endif
