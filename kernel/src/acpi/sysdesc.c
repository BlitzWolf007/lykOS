#include "sysdesc.h"
#include <limine.h>
#include <debug.h>
#include <utils.h>

static volatile struct limine_rsdp_request rsdp_request =
{
    .id = LIMINE_RSDP_REQUEST,
    .revision = 0
};

static bool sysdesc_ext;

static struct rsdt *rsdt;
static struct xsdt *xsdt;

void sysdesc_init()
{
    void *p = (struct rsdp*)rsdp_request.response->address;

    if (((struct rsdp*)p)->revision == 0)
    {
        sysdesc_ext = false;
        debug_log("RSDP addr: 0x%llx", p);

        rsdt = (struct rsdt*)((struct rsdp*)p)->rsdt_addr;
        debug_log("RSDT addr: 0x%llx", rsdt);
    }        
    else 
    {
        sysdesc_ext = true;
        debug_log("XSDP addr: 0x%llx", p);

        xsdt = (struct xsdt *)((struct xsdp *)p)->xsdt_addr;
        debug_log("XSDT addr: 0x%llx", xsdt);
    }
}

struct sdt_hdr* sysdesc_lookup(char *signature)
{
    if (!sysdesc_ext)
    {
        u64 entries = (rsdt->hdr.length - sizeof(rsdt->hdr)) / 4;

        for (u64 i = 0; i < entries; i++)
        {
            struct sdt_hdr *hdr = (struct sdt_hdr*)rsdt->ptr_to_sdt[i];
            if (memcmp(hdr->signature, signature, 4) == 0)
                return hdr;
        }
    }
    else
    {
        u64 entries = (xsdt->hdr.length - sizeof(xsdt->hdr)) / 8;

        for (u64 i = 0; i < entries; i++)
        {
            struct sdt_hdr *hdr = (struct sdt_hdr*)xsdt->ptr_to_sdt[i];
            if (memcmp(hdr->signature, signature, 4) == 0)
                return hdr;
        }
    }
}
