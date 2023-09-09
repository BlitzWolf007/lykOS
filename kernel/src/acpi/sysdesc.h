#pragma once
#include <def.h>

struct rsdp
{
    char signature[8];
    u8 checksum;
    char oem_id[6];
    u8 revision;
    u32 rsdt_addr;
} __attribute__((packed));

struct xsdp
{
    struct rsdp rsdp;

    u32 length;
    u32 xsdt_addr;
    u8 ext_checksum;
    u8 rsv[3];
} __attribute__((packed));

struct sdt_hdr
{
    char signature[4];
    u32 length;
    u8 revision;
    u8 checksum;
    char oem_id[6];
    char oem_table_id[8];
    u32 oem_revision;
    u32 creator_id;
    u32 creator_revision;
} __attribute__((packed));

struct rsdt
{
    struct sdt_hdr hdr;
    u32 ptr_to_sdt[0];
} __attribute__((packed));

struct xsdt
{
    struct sdt_hdr hdr;
    u64 ptr_to_sdt[0];
} __attribute__((packed));

void sysdesc_init();
struct sdt_hdr* sysdesc_lookup(char *signature);