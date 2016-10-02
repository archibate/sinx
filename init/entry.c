#include <kernel.h>
#include <init/entry.h>
#include <init/init.h>
#include <tty/tty.h>

void _xmain(void)
{
	_entry();
}

void _entry(void)
{
	(* (short *) 0xB8000) = 0x0C03;

	kernel_modinit();
	//tt_modinit();
	tty_modinit();

	__asm__ ("cli;hlt");
	for (;;);
}

void kernel_init(void)
{
}

