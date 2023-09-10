#include "smp.h"
#include <limine.h>
#include <debug.h>

static volatile struct limine_smp_request smp_request = 
{
    .id = LIMINE_SMP_REQUEST,
    .revision = 0
};

u64 ready_core_count = 0;

__attribute__((noreturn)) static void cpu_core_init(struct limine_smp_info *info)
{
    debug_log("A new core?");

    ready_core_count++;
    for (;;)
        asm ("hlt");
}

void smp_init()
{
    for (u64 i = 0; i < smp_request.response->cpu_count; i++)
    {
        if (smp_request.response->cpus[i]->lapic_id == smp_request.response->bsp_lapic_id)
        {
            ready_core_count++;
            continue;
        }

        smp_request.response->cpus[i]->goto_address = &cpu_core_init;

        // Wait for the previous core setup to finish.
        while (ready_core_count <= i)
            ;
    }
}
