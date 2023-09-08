#include "draw.h"
#include <limine.h>
#include "font.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0
};

struct canvas main_canvas;

void draw_init()
{
    main_canvas.buffer = (u32*)framebuffer_request.response->framebuffers[0]->address;
    main_canvas.width = framebuffer_request.response->framebuffers[0]->width;
    main_canvas.height = framebuffer_request.response->framebuffers[0]->height;
    main_canvas.pitch = framebuffer_request.response->framebuffers[0]->pitch;
}

void draw_pixel(struct canvas *c, u64 x, u64 y, u32 color)
{
    c->buffer[y * c->pitch + x] = color;
}

void draw_char(struct canvas *c, u64 x, u64 y, u32 color, char ch)
{
    u64 offset = y * (c->pitch / 4) + x;

    for (int j = 0; j < 8; j++)
    {
        for (int i = 0; i < 8; i++)
            if ((font_8x8[(u64)ch][j] >> i) & 1)
                c->buffer[offset + i] = color;
        offset += c->pitch / 4;
    }        
}

void draw_str(struct canvas *c, u64 x, u64 y, u32 color, char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
        draw_char(c, x + i * 8, y, color, str[i]);
}
