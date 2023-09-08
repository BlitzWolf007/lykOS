#include "debug.h"
#include <utils.h>

#include <graphics/draw.h>
#include <devices/serial.h>

u64 row = 0;

void debug_log(const char *format, ...)
{
    va_list list;
    va_start(list, format);

    char buf[128] = {0};
    vsprintf(&buf[0], format, list);

    // Leave one pixel space between lines.
    draw_str(&main_canvas, 0, row * 9, 0xFFFFFF, &buf[0]);
    serial_log(&buf[0]);

    row++;
    va_end(list);
}

int vsprintf(char *str, const char *format, va_list list)
{
    int i;
    for (i = 0; format[i] != '\0'; i++)
    {
        if (format[i] == '%')
        {
            char buf[128];
            i++;
            if (format[i] == 's')
                strcat(&str[0], va_arg(list, const char *));
            else if (format[i] == 'd')
                strcat(&str[0], itoa(va_arg(list, int), buf, 10));
            else if (format[i] == 'x')
                strcat(&str[0], itoa(va_arg(list, uint64_t), buf, 16));
            else if (format[i] == 'c')
            {
                char c = va_arg(list, char);
                strncat(&str[0], &c, 1);
            }                
            else if (format[i] == 'l')
            {
                i++;
                if (format[i] == 'l')
                {
                    i++;
                    if (format[i] == 'u')
                        strcat(&str[0], ulltoa(va_arg(list, uint64_t), buf, 10));
                    else if (format[i] == 'x')
                        strcat(&str[0], ulltoa(va_arg(list, uint64_t), buf, 16));
                }
            }
        }
        else
            strncat(&str[0], &format[i], 1);
    }

    return strlen(str);
}