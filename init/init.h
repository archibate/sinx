#ifndef	_SINX_INIT_INIT_H
#define	_SINX_INIT_INIT_H

#define	KMBASE	0xC0000000
#define	CODE_SELECTOR	0x0008
#define	DATA_SELECTOR	0x0010

void /*__init_text*/ kernel_modinit(void);

#endif

