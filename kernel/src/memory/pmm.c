#include "pmm.h"
#include <def.h>
#include <limine.h>
#include <debug.h>

static volatile struct limine_memmap_request memmap_request =
{
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

u64 *free_list = 0;
struct pmm_memmap_seg pmm_segments[32];
u64 pmm_seg_count = 0;

void pmm_init()
{
    if (memmap_request.response == NULL 
     || memmap_request.response->entry_count < 1)
    {
        // TODO: exception
    }

    for (int i = 0; i < memmap_request.response->entry_count; i++)
    {
        struct limine_memmap_entry *entry = memmap_request.response->entries[i];

        pmm_segments[pmm_seg_count].base = entry->base;
        pmm_segments[pmm_seg_count].len = entry->length;
        pmm_segments[pmm_seg_count].type = entry->length;
        pmm_seg_count++;

        if (entry->type == LIMINE_MEMMAP_USABLE)
        {
            debug_log("Free memory segment: base 0x%llx, len 0x%llx", entry->base, entry->length);
            for (u64 addr = entry->base; addr < entry->base + entry->length; addr += PAGE_SIZE)
                pmm_free((void*)addr);
        }        
    }
}

void pmm_free(void *addr)
{
    *(u64*)addr = (u64)free_list;
    free_list = (u64*)addr;
}

void* pmm_alloc()
{
    void *ret = (void*)free_list;
    if (ret == NULL)
    {
        // TODO: exception
    }

    free_list = (u64*)*free_list;

    return ret;
}