#ifndef	_SINX_KERNEL_INIT_H
#define	_SINX_KERNEL_INIT_H

#define	KMBASE	0xC0000000

void /*__attribute__((section(".init.text")))*/ _xmain(void);
void /*__attribute__((section(".init.text")))*/ kernel_init(void);

#endif

