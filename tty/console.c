#include <kernel.h>
#include <console.h>

void console_modinit()
{
	console_clear();
	/*__console_prints("console module initialized "
			 "successfully\n", CONS_COL_LI_GREEN);*/
}

void console_clear()
{
	console_set_cursor(0);
	u16 *p;
	for (p = VRAM; p < VRAM + CONS_X * CONS_Y; ++p)
		*p = 0x0700;
}

void console_prints(char *s)
{
	__console_prints(s, 0x07);
}

void __console_prints(char *s, enum cons_col color)
{
	u16 *p = VRAM + console_get_cursor();
	color <<= 8;
	char c;
	while (c = *s++) {
		switch (c) {
		case '\n':
			if (p < VRAM + CONS_X * (CONS_Y - 1))
				p += CONS_X;
			else
				console_scrollup();
		case '\r':
			p -= (p - VRAM) % CONS_X;
			continue;
		}
		*p++ = c | color;
	}
	console_set_cursor(p - VRAM);
}

void console_set_cursor(u16 off)
{
	__asm__ (	"movw	$0x03D4, %%dx\n"
			"movb	$0x0F, %%al\n"
			"outb	%%al, %%dx\n"
			"incw	%%dx\n"
			"movw	%%cx, %%ax\n"
			"outb	%%al, %%dx\n"
			"decw	%%dx\n"
			"movb	$0x0E, %%al\n"
			"outb	%%al, %%dx\n"
			"incw	%%dx\n"
			"movb	%%ah, %%al\n"
			"outb	%%al, %%dx\n"
			:: "cx" (off));
}

u16 console_get_cursor()
{
	u16 res;
	__asm__ (	"movw	$0x3D4, %%dx\n"
			"movb	$0x0E, %%al\n"
			"outb	%%al, %%dx\n"
			"incw	%%dx\n"	
			"inb	%%dx, %%al\n"
			"movb	%%al, %%ah\n"
			"decw	%%dx\n"	
			"movb	$0x0F, %%al\n"
			"outb	%%al, %%dx\n"
			"incw	%%dx\n"	
			"inb	%%dx, %%al\n"
			: "=ax" (res) : );
	return res;
}

void console_scrollup()		/* 上滚一行 */
{
	u16 *p;
	for (p = VRAM; p < VRAM + CONS_X * (CONS_Y - 1); ++p)
		*p = *(p + CONS_X);
	for (; p < VRAM + CONS_X * CONS_Y; ++p)
		*p = 0x0700;
}

