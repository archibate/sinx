#include <kernel.h>
#include <kernel/init.h>
#include <tty/console.h>

void _xmain(void)
{
	kernel_init();
}

void kernel_init(void)
{
	(* (short *) 0xB8000) = 0x0C03;

	console_init();

	__asm__ ("cli;hlt");
	for (;;);
}

