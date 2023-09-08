#pragma once
#include <def.h>

#define PAGE_SIZE 0x1000

void pmm_init();

void pmm_free(void *addr);
void *pmm_alloc();

struct pmm_memmap_seg
{
    u64 base;
    u64 len;
    u64 type;
};
extern struct pmm_memmap_seg pmm_segments[32];
extern u64 pmm_seg_count;
