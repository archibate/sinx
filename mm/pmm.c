#include <kernel.h>
#include <mm/pmm.h>
#include <init/init.h>
#include <sched/sched.h>

pgdi_t pgd_kern[MBR_PER_PGTAB] __attribute__((aligned(PAGESZ)));
ptei_t ptes_kern[PTE_KERN_COUNT][MBR_PER_PGTAB] __attribute__((aligned(PAGESZ)));

static inline void pmm_cpu_invlpg(lin_t la);

void pmm_modinit()
{
	ndx_t kern_npgdi0 = lin2npgdi(KMBASE);
	r_t i, j;
	for (i = kern_npgdi0, j = 0;
	     i < kern_npgdi0 + PTE_KERN_COUNT;
	     ++i, ++j)
		pgd_kern[i] = klin2phy((lin_t) (ptes_kern + j))
			    | PAGE_PRESENT | PAGE_WRITEABLE;
	r_t *pte0_kern = (r_t *) ptes_kern;
	for (i = 0; i < PTE_KERN_COUNT * MBR_PER_PGTAB; ++i)
		pte0_kern[i] = (i << 12) | PAGE_PRESENT | PAGE_WRITEABLE;
	phy_t pgd_kern_pa = klin2phy((lin_t) pgd_kern);
	switch_pgd(pgd_kern_pa);

	/*pmm_map(pgd_kern, 0xC0000000, 0x100000, PAGE_PRESENT);
	if ((* (u32 *) 0xC0100000) != (* (u32 *) 0xC0000000))
		__asm__ ("cli;hlt");/**/
}

void switch_pgd(phy_t pgd_pa)
{
	__asm__ ("mov	%0, %%cr3" :: "r" (pgd_pa));
}

int pmm_map(pgdi_t *pgd, lin_t la, phy_t pa, u8 flags)
{
	ndx_t npgdi = lin2npgdi(la);
	ndx_t nptei = lin2nptei(la);
	ptei_t *pte = (ptei_t *) (pgd[npgdi] & PG_MASK);
	if (!pte) {
		pte = (ptei_t *) pmm_alloc_page();
		if (!pte)
			return FALSE;
		pgd[npgdi] = (r_t) pte | PAGE_PRESENT | PAGE_WRITEABLE;
	} else {
		pte = (ptei_t *) kphy2lin((r_t) pte);
	}
	pte[nptei] = (pa & PG_MASK) | flags;
	pmm_cpu_invlpg(la);
	return TRUE;
}

int pmm_unmap(pgdi_t *pgd, lin_t la)
{
	ndx_t npgdi = lin2npgdi(la);
	ndx_t nptei = lin2nptei(la);
	ptei_t *pte = (ptei_t *) (pgd[npgdi] * PG_MASK);
	if (!page_valid(pte))
		return FALSE;
	pte = (ptei_t *) kphy2lin((r_t) pte);
	pte[nptei] = 0;	/* set to NULL */
	pmm_cpu_invlpg(la);
	return TRUE;
}

lin_t pmm_alloc_page()
{
	lin_t la = 0;
	return la;
}

phy_t pmm_get_mapping(pgdi_t *pgd, lin_t la)
{
	phy_t pa = 0;	/* NULL */
	ndx_t npgdi = lin2npgdi(la);
	ndx_t nptei = lin2nptei(la);
	ptei_t *pte = (ptei_t *) (pgd[npgdi] * PG_MASK);
	if (!page_valid(pte))
		goto out;
	pte = (ptei_t *) kphy2lin((r_t) pte);
	if (!page_valid(pte[nptei]))
		goto out;
	pa = pte[nptei] & PG_MASK;
out:
	return pa;
}

static inline void pmm_cpu_invlpg(lin_t la)
{
	__asm__ ("invlpg (%0)" :: "a" (la));
}

