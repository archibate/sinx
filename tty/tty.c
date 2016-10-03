#include <kernel.h>
#include <tty/tty.h>
#include <tty/console.h>

struct tty ttys[TTYS_MAX];
struct tty *curr_tty;

static inline void __tty_set_cursor(struct tty *tty, twh_t x, twh_t y);

void tty_modinit()
{
	console_modinit();
	/* 将 tty0 设为控制台 */
	tty_init(ttys + 0, VRAM, CONS_X, CONS_Y);
	curr_tty = ttys + 0;
	tty_cputstr(curr_tty, "Hello, World!\nI'm the Kernel developer: Peng Yubin!\n");
}

struct tty *tty_init(struct tty *tty, u16 *buf, twh_t sx, twh_t sy)
{
	tty->sx = sx;
	tty->sy = sy;
	tty->buf = buf;
	tty_clear(tty);
	return tty;
}

#define	type_check_vt(v, t)
#define	assert_qk(x)

void __tty_putdata(struct tty *tty, u16 data)
{
	assert_qk(tty->sy != 0);
	switch (data & 0xFF) {	/* 得到低位上的 ASCII 码 */
	case '\n':
		if ((signed) tty->cur <= tty->sx * (tty->sy - 1))
			tty->cur += tty->sx;
		else
			tty_scrollup(tty);
	case '\r':
		tty->cur -= tty->cur % tty->sx;
		return;
	}
	tty->buf[tty->cur++] = data;
}

void tty_putstr(struct tty *tty, char *s)
{
	__tty_putstr(tty, s, 0x0700);
}

void tty_putstr_color(struct tty *tty, char *s, enum cons_col color)
{
	__tty_putstr(tty, s, color << 8);
}

void tty_cputstr(struct tty *tty, char *s)
{
	tty->cur = console_get_cursor();
	__tty_putstr(tty, s, 0x0700);
	console_set_cursor(tty->cur);
}

void tty_cputstr_color(struct tty *tty, char *s, enum cons_col color)
{
	tty->cur = console_get_cursor();
	__tty_putstr(tty, s, color << 8);
	console_set_cursor(tty->cur);
}

void tty_clear(struct tty *tty)
{
	tty->cur = 0;
	u16 *p_end = tty->buf + tty->sx * tty->sy;
	for (u16 *p = tty->buf; p < p_end; ++p)
		*p = 0x0700;
}

void tty_scrollup(struct tty *tty)
{
	twh_t sx = tty->sx;
	u16 *p_end = tty->buf + sx * (tty->sy - 1);
	u16 *p;
	for (p = tty->buf; p < p_end; ++p)
		*p = *(p + sx);
	for (p_end += sx; p < p_end; ++p)
		*p = 0x0700;
}

void tty_set_cursor(struct tty *tty, twh_t x, twh_t y)
{
	__tty_set_cursor(tty, x, y);
}

static inline void __tty_set_cursor(struct tty *tty, twh_t x, twh_t y)
{
	tty->cur = y * tty->sx + x;
}

