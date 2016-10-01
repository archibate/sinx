#ifndef	_SINX_TTY_CONSOLE_H
#define	_SINX_TTY_CONSOLE_H

#include <kernel/init.h>

#define	VRAM_PA	0xB8000
#define	VRAM_LA	(KMBASE + VRAM_PA)
#define	VRAM	((u16 *) VRAM_LA)

#define	CONS_X	80
#define	CONS_Y	25

void console_init();
void console_clear();
void console_prints(char *s);
void console_set_cursor(u16 off);
u16 console_get_cursor();
void console_scrollup();

#endif

