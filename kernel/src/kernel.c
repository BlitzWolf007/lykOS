#include <def.h>

#include <graphics/draw.h>
#include <debug.h>

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

    pmm_init();
    vmm_init();

    sysdesc_init();

    void *test = sysdesc_lookup("MCFG");
    debug_log("Test: 0x%llx", (u64)test);
    void *test1 = sysdesc_lookup("MADT");
    debug_log("Test: 0x%llx", (u64)test1);

    debug_log("Are we still alive?");

    hcf();
}