#ifndef	_SINX_TTY_CONSOLE_H
#define	_SINX_TTY_CONSOLE_H

#include <kernel/init.h>

#define	VRAM_PA	0xB8000
#define	VRAM_LA	(KMBASE + VRAM_PA)
#define	VRAM	((u16 *) VRAM_LA)

#define	CONS_X	80
#define	CONS_Y	25

enum cons_col {
	CONS_COL_BLACK		= 0,
	CONS_COL_BLUE		= 1,
	CONS_COL_GREEN		= 3,
	CONS_COL_CYAN		= 3,
	CONS_COL_RED		= 4,
	CONS_COL_ORANGE		= 5,
	CONS_COL_BROWN		= 6,
	CONS_COL_WHITE		= 7,
	CONS_COL_GREY		= 8,
	CONS_COL_LI_BLUE	= 9,
	CONS_COL_LI_GREEN	= 10,
	CONS_COL_LI_CYAN	= 11,
	CONS_COL_LI_RED		= 12,
	CONS_COL_LI_ORANGE	= 13,
	CONS_COL_LI_BROWN	= 14,
	CONS_COL_LI_WHITE	= 15,
	CONS_COL_NULL		= 0,
	CONS_COL_INVALID	= -1,
	CONS_COL_UNKNOWN	= -2
};

void console_modinit();
void console_clear();
void console_prints(char *s);
void __console_prints(char *s, enum cons_col color);
void console_set_cursor(u16 off);
u16 console_get_cursor();
void console_scrollup();

#endif

