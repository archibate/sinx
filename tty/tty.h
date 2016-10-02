#ifndef	_SINX_TTY_TTY_H
#define	_SINX_TTY_TTY_H

#include <tty/console.h>

#define	TTYS_MAX	6

typedef u16 twh_t;	/* TTY Widget or Height */
struct tty {
	u16 *buf;
	u32 cur;
	twh_t sx;
	twh_t sy;
};

#define	__tty_putstr(tty, s, color) ({ \
	 	struct tty *_tty = tty; \
	 	char *_s = s; \
	 	int _color = color; \
		while (*_s) \
			__tty_putdata(_tty, *_s++ | _color); \
	 })

void tty_modinit();
struct tty *tty_init(struct tty *tty, u16 *buf, twh_t sx, twh_t sy);
void tty_clear(struct tty *tty);
void tty_scrollup(struct tty *tty);
void tty_set_cursor(struct tty *tty, twh_t x, twh_t y);
void tty_putstr(struct tty *tty, char *s);
void tty_putstr_color(struct tty *tty, char *s, enum cons_col color);
void tty_cputstr(struct tty *tty, char *s);
void tty_cputstr_color(struct tty *tty, char *s, enum cons_col color);

#endif

