#include "vmm.h"
#include "pmm.h"

#include <debug.h>

// Not static on purpose - see def.h
volatile struct limine_hhdm_request HHDMRequest = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0
    };

volatile struct limine_kernel_address_request KernelAddrRequest = {
    .id = LIMINE_KERNEL_ADDRESS_REQUEST,
    .revision = 0
    };

struct page_map kernel_map;

void vmm_init()
{
    debug_log("Higher half: 0x%llx", HIGHER_HALF);
    debug_log("Kernel virt: 0x%llx", KERNEL_ADDR_VIRT);
    debug_log("Kernel phys: 0x%llx", KERNEL_ADDR_PHYS);

    vmm_setup_page_map(&kernel_map);

    debug_log("A");

    // Map the first 4GiB mandated by the limine spec.
    for (u64 i = 0; i < 4 * GiB; i += PAGE_SIZE)
    {   
        vmm_map_page(&kernel_map, HIGHER_HALF + i, i, PTE_USER | PTE_WRITE | PTE_PRESENT);
        vmm_map_page(&kernel_map, i, i, PTE_USER | PTE_WRITE | PTE_PRESENT); // Also indetity map.
    }
        
    debug_log("B");

    // Map the kernel to the last 2GiB of the virt addr space.
    for (u64 i = 0; i < 2 * GiB; i += PAGE_SIZE)
        vmm_map_page(&kernel_map, KERNEL_ADDR_VIRT + i, KERNEL_ADDR_PHYS + i, PTE_WRITE | PTE_PRESENT);

    debug_log("C");

    // Map the needed memory segments.
    for (u64 idx = 0; idx < pmm_seg_count; idx++)
    {
        if (pmm_segments[idx].type == LIMINE_MEMMAP_USABLE)
            for (u64 i = 0; i < pmm_segments[idx].len; i += PAGE_SIZE)
            {
                vmm_map_page(&kernel_map, HIGHER_HALF + pmm_segments[idx].base + i, pmm_segments[idx].base + i, PTE_USER | PTE_WRITE | PTE_PRESENT);
                vmm_map_page(&kernel_map, pmm_segments[idx].base + i, pmm_segments[idx].base + i, PTE_USER | PTE_WRITE | PTE_PRESENT); // Also indetity map.
            } 
        else if (pmm_segments[idx].type == LIMINE_MEMMAP_FRAMEBUFFER)
            for (u64 i = 0; i < pmm_segments[idx].len; i += PAGE_SIZE)
            {
                vmm_map_page(&kernel_map, HIGHER_HALF + pmm_segments[idx].base + i, pmm_segments[idx].base + i, PTE_USER | PTE_WRITE | PTE_PRESENT);
            } 
    }              

    debug_log("New kernel page map generated.");

    vmm_switch_to(&kernel_map);

    debug_log("Loaded new kernel page map.");
}

u64* vmm_get_next_level(u64 *topLevel, u64 idx, bool allocate)
{
    if ((topLevel[idx] & PTE_PRESENT) != 0)
        return (u64*)(PTE_GET_ADDR(topLevel[idx]) + HIGHER_HALF);      

    if (!allocate)
        return NULL;

    void *nextLevel = pmm_alloc();
    memset(nextLevel, 0, PAGE_SIZE);
    topLevel[idx] = (u64)nextLevel | PTE_PRESENT | PTE_WRITE | PTE_USER; 

    return (u64*)(nextLevel + HIGHER_HALF);
}

void vmm_switch_to(struct page_map *map)
{
    asm volatile (
        "movq %0, %%cr3"
        :
        : "r" ((uptr)map->topLevel - HIGHER_HALF)
        : "memory"
    );
}

void vmm_setup_page_map(struct page_map *map)
{
    map->topLevel = (u64*)(pmm_alloc() + HIGHER_HALF);
    memset(map->topLevel, 0, PAGE_SIZE);
}

void vmm_map_page(struct page_map *map, u64 virt, u64 phys, u64 flags)
{
    u64 pml4_entry = (virt >> 39) & 0x1FF;
    u64 pml3_entry = (virt >> 30) & 0x1FF;
    u64 pml2_entry = (virt >> 21) & 0x1FF;
    u64 pml1_entry = (virt >> 12) & 0x1FF;

    u64 *pml4 = map->topLevel;
    u64 *pml3 = vmm_get_next_level(pml4, pml4_entry, true);
    u64 *pml2 = vmm_get_next_level(pml3, pml3_entry, true);
    u64 *pml1 = vmm_get_next_level(pml2, pml2_entry, true);

    pml1[pml1_entry] = phys | flags;
}

u64 *vmm_virt_to_pte(struct page_map *map, u64 virt, bool allocate)
{
    u64 pml4_entry = (virt >> 39) & 0x1FF;
    u64 pml3_entry = (virt >> 30) & 0x1FF;
    u64 pml2_entry = (virt >> 21) & 0x1FF;
    u64 pml1_entry = (virt >> 12) & 0x1FF;

    u64 *pml4 = map->topLevel;
    u64 *pml3 = vmm_get_next_level(pml4, pml4_entry, allocate);
    if (pml3 == NULL)
        return NULL;
    u64 *pml2 = vmm_get_next_level(pml3, pml3_entry, allocate);
    if (pml2 == NULL)
        return NULL;
    u64 *pml1 = vmm_get_next_level(pml2, pml2_entry, allocate);
    if (pml1 == NULL)
        return NULL;

    return &pml1[pml1_entry];
}

u64 vmm_virt_to_phys(struct page_map *map, u64 virt)
{
    u64 *pte = vmm_virt_to_pte(map, virt, false);
    if (pte == NULL || (PTE_GET_FLAGS(*pte) & PTE_PRESENT) == 0)
        return 0;

    return PTE_GET_ADDR(*pte);
}