#pragma once

#include <limine.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

#define KiB 0x400ull
#define MiB (KiB * 0x400ul)
#define GiB (MiB * 0x400ul)

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef uintptr_t uptr;

extern volatile struct limine_hhdm_request HHDMRequest;
extern volatile struct limine_kernel_address_request KernelAddrRequest;
#define HIGHER_HALF (HHDMRequest.response->offset)
#define KERNEL_ADDR_VIRT (KernelAddrRequest.response->virtual_base)
#define KERNEL_ADDR_PHYS (KernelAddrRequest.response->physical_base)