#pragma once
#include <def.h>

#define PTE_PRESENT 1
#define PTE_WRITE 1 << 1
#define PTE_USER 1 << 2

#define PTE_ADDR_MASK 0x000ffffffffff000ull
#define PTE_GET_ADDR(pte) (pte & PTE_ADDR_MASK)
#define PTE_GET_FLAGS(pte) (pte & (~PTE_ADDR_MASK))

struct page_map
{
    u64 *topLevel;
};

void vmm_map_page(struct page_map *map, u64 virt, u64 phys, u64 flags);
u64 *vmm_virt_to_pte(struct page_map *map, u64 virt, bool allocate);
u64 vmm_virt_to_phys(struct page_map *map, u64 virt);

void vmm_init();
u64* vmm_get_next_level(u64 *topLevel, u64 idx, bool allocate);
void vmm_switch_to(struct page_map *map);