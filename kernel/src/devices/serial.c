#include "serial.h"
#include <def.h>

// THIS IS FOR QEMU ONLY
// So that is why im skipping important steps.

static void outb(u16 port, u8 val)
{
    asm volatile("outb %0, %1"
                 :
                 : "a"(val), "Nd"(port));
}


void serial_log(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        outb(0x3F8, str[i]);
    outb(0x3F8, '\n');
}
