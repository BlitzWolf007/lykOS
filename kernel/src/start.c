#include <def.h>

#include <graphics/draw.h>
#include <debug.h>

#include <arch/x86_64/idt.h>
#include <arch/x86_64/smp.h>

#include <memory/pmm.h>
#include <memory/vmm.h>

#include <acpi/sysdesc.h>

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

    idt_init();
    
    pmm_init();
    vmm_init();

    sysdesc_init();

    smp_init();

    debug_log("Are we still alive?");

    hcf();
}