#pragma once
#include <def.h>

#define RGB(r, g, b) (r << 16) | (g << 8) | b

struct canvas
{
    u32 *buffer;
    u64 width;
    u64 height;
    u64 pitch;
};

extern struct canvas main_canvas;

void draw_init();
void draw_pixel(struct canvas *c, u64 x, u64 y, u32 color);
void draw_char(struct canvas *c, u64 x, u64 y, u32 color, char ch);
void draw_str(struct canvas *c, u64 x, u64 y, u32 color, char *str);