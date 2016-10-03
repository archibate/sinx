#ifndef	_SINX_MM_PMM_H
#define	_SINX_MM_PMM_H

#include <init/init.h>

#ifndef	KMBASE
#define	KMBASE	0xC0000000
#endif

#define	PAGE_PRESENT	0x1
#define	PAGE_WRITEABLE	0x2
#define	PAGE_FORUSER	0x4
#define	PAGESZ		4096
#define	PG_MASK		0xFFFFF000
#define	MBR_PER_PGTAB	(PAGESZ / sizeof(r_t))
#define	PTE_KERN_COUNT	0x40
#define	lin2npgdi(la)	(((la) >> 22) & 0x3FF)
#define	lin2nptei(la)	(((la) >> 12) & 0x3FF)
#define	lin2pgoff(la)	((la) & 0xFFF)
#define	klin2phy(la)	((la) - KMBASE)
#define	kphy2lin(pa)	((pa) + KMBASE)
#define	page_valid(pg)	(pg)

typedef r_t addr_t;	/* Address */
typedef addr_t lin_t;	/* Linear Address */
typedef addr_t phy_t;	/* Physical Address */
typedef r_t pg_t;	/* Page Descriptor */
typedef pg_t pgdi_t;	/* PGD Item */
typedef pg_t ptei_t;	/* PTE Item */

void pmm_modinit();
void switch_pgd(phy_t pgd_pa);
int pmm_map(pgdi_t *pgd, lin_t la, phy_t pa, u8 flags);
int pmm_unmap(pgdi_t *pgd, lin_t la);
lin_t pmm_alloc_page();
phy_t pmm_get_mapping(pgdi_t *pgd, lin_t la);

#endif

