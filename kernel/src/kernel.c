#include <def.h>

#include <graphics/draw.h>
#include <debug.h>

#include <memory/pmm.h>
#include <memory/vmm.h>

// Halt and catch fire function.
static void hcf()
{
    asm ("cli");
    for (;;) {
        asm ("hlt");
    }
}

void _start()
{
    draw_init();

    pmm_init();
    vmm_init();

    debug_log("Are we still alive?");

    hcf();
}